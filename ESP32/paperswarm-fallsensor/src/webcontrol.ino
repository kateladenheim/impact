#define SERIAL_ENABLED
#define BLINK_MILLIS 500

#include <Arduino.h>

#include <Preferences.h>
#include <ArduinoJson.h>
#include <arduino-timer.h>
#include <breathe.h>
#include <mydebug.h>

#include "inout.inc"
#include "esphub.inc"
// #include "tor.inc"
#include "autowifi.inc"


Preferences prefs;
Breathe b;
auto timer = timer_create_default();

void blink(int ms = BLINK_MILLIS) {
    LED_ON;
    timer.in(ms ? ms : BLINK_MILLIS, [](void*) -> bool {
        LED_OFF; return false;
    });
}

/*********************************
 * Defines / declarations go here
 *********************************/

#define NUM_FSRS 5
const int fsrPin[NUM_FSRS] = {A2, A3, A4, 33, 32};
int fsrReading[NUM_FSRS];     // the analog reading from the FSR resistor divider

#define THRESHOLD 3100

#define FALL_TIME 3         // number of consecutive loops during which trigger=true for a fall to be detected
#define RESET_TIME 100      // number of consecutive loops during which trigger=false for the system to reset

int on_time = 0;
int off_time = 0;
bool fallen = false;

/*********************************
 * End defines / declarations
 *********************************/


void setup() {
    delay(1000);
    DEBUG_START;
    Serial.setDebugOutput(true);

    DEBUGF("\n\n\n=== Start logging ===\n\n");
    DEBUG("ESP powered on, chipID = ", CHIPID);

    prefs.begin("esphub");
    config(&prefs);

    wifi_setup();
    esphub_setup("esphub.mehtank.com", &prefs, blink, config, setpins);

    /*********************************
     * Setup code goes in here
     *********************************/

    for (int i = 0; i < NUM_FSRS; i++) {
        pinMode(fsrPin[i], INPUT);
    }

    /*********************************
     * End setup code
     *********************************/
}

void loop() {
    wifi_loop();
    esphub_loop();
    timer.tick();
    b.loop();

    /*********************************
     * Loop code goes in here
     *********************************/

    for (int i = 0; i < NUM_FSRS; i++) {
        fsrReading[i] = analogRead(fsrPin[i]);
        // DEBUGF("Analog reading fsr%d = %d", i, fsrReading[i]);     // print the raw analog reading
    }

    // conditional logic: trigger if 012, 123, or 234 meet pressure threshold, otherwise do not trigger

    bool trigger = false;
    for (int start = 0; start < 2; start++) {
        for (int delta = 0; delta < 3; delta++) {
            trigger = trigger || (fsrReading[start + delta] > THRESHOLD);
            trigger = trigger || (fsrReading[start + delta + 5] > THRESHOLD);
        }
    }

    if (trigger) {
        off_time = 0;
        on_time += 1;
    } else {
        on_time = 0;
        off_time += 1;
    }

    if (on_time > FALL_TIME && !fallen) {
        DEBUG("Fall triggered!");
        esphub_announce("fall", "impact");
        fallen = true;
    } else if (off_time > RESET_TIME) {
        DEBUG("System reset, waiting for fall");
        fallen = false;
    }

    /*********************************
     * End loop code
     *********************************/

    delay(50);
}