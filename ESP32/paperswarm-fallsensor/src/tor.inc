#if defined(ARDUINO_ARCH_ESP8266)
#   include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#   include <WiFi.h>
#endif

#include <mydebug.h>


void wifi_setup() {
    WiFi.begin("torii", "6283185307");
    while (WiFi.status() != WL_CONNECTED) {
        DEBUGF("~");
        delay(100);
    };
    DEBUG("Wifi connected:", WiFi.localIP().toString());
}

void wifi_loop() { }
