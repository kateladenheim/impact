// No more than 100 numbered pins on any given ESP
#define MAX_PIN_IDS 100
#define NUM_SERVOS 8

#include <Arduino.h>

#ifdef ESP32
    #include <analogWrite.h>
    #define analogWriteFreq analogWriteFrequency
#endif

#include <Preferences.h>
#include <ArduinoJson.h>
#include <mydebug.h>

void setpins(JsonArray nodes) {
    DEBUG("Number of pins to set: ", nodes.size());
    for (int i = 0; i < nodes.size(); i++) {
        const int pin = nodes[i][0];
        char mode = 'O'; // defaults to digitalwrite

        if (nodes[i][1].is<int>()) // Integer between 0 to 180 -> Servo
            mode = 'S';
        else if (nodes[i][1].is<float>())  // float between 0 to 1 -> analogWrite
            mode = 'L';

        unsigned char val;
        DEBUGF("Set pin %i of mode %c... ", pin, mode);

        switch (mode) {
            case 'O': // OUTPUT --> digital gpio
                val = nodes[i][1];
                digitalWrite(pin, val);
                break;
            case 'L': // LED --> analog gpio
                val = 255 * (double)nodes[i][1];
                analogWrite(pin, val);
                break;
            case 'S': // SERVO --> use servo values
                //       min  max
                // val :   0  180
                //  us : 544 2000 (from Servo library)
                // pwm :  46  204 (above scaled to 3000us period)
                // frac: .18  .66 (above scaled to 333hz frequency)
                //  me :  35  215
                val = 35 + (unsigned char)nodes[i][1];
                analogWrite(pin, val);
                break;
        }
        DEBUG("value = ", (int)val);
    }
}

void configpin(int pin, const char* mode) {
    if (mode[0] == 'A' or mode[0] == 'a' or 
        mode[0] == 'I' or mode[0] == 'i') {
        // Input or Analog modes
        pinMode(pin, INPUT);
    } else {
        pinMode(pin, OUTPUT);
    }
}

void config(Preferences* prefs) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, prefs->getString("config", ""));
    JsonArray nodes = doc.as<JsonArray>();

    DEBUG("Number of config entries: ", nodes.size());
    for (int i = 0; i < nodes.size(); i++) {
        const int pin = nodes[i][0];
        const char* mode = nodes[i][1];
        DEBUGF("Setting pin %i to mode %s\n", pin, mode);
        configpin(pin, mode);
    }
    analogWriteFreq(333);
}
