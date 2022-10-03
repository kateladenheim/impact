#pragma once

#if defined(ARDUINO_ARCH_ESP8266)
#   define CHIPID ESP.getChipId()
#elif defined(ARDUINO_ARCH_ESP32)
#   define CHIPID (uint32_t) ESP.getEfuseMac()
#endif

#ifndef LED_PIN
  #define LED_PIN LED_BUILTIN
#endif
#define LED_START {pinMode(LED_PIN, OUTPUT); LED_OFF;}
#define LED_ON digitalWrite(LED_PIN, LOW)
#define LED_OFF digitalWrite(LED_PIN, HIGH)
#define LED_BEAT {if (millis() & (1 << 9)) LED_ON; else LED_OFF;}

#ifndef SERIAL_ENABLED
  #define DEBUG_START LED_START
  #define DEBUG_RATE(rate) LED_START
  #define DEBUG1(str) (void)0
  #define DEBUG2(str, var) (void)0
  #define DEBUGF(...) (void)0
#else
  #define DEBUG_START {Serial.begin(115200); LED_START}
  #define DEBUG_RATE(rate) {Serial.begin(rate); LED_START}
  #define DEBUG1(str) Serial.println(str)
  #define DEBUG2(str, var) {Serial.print(str); Serial.println(var);}
  #define DEBUGF(...) {Serial.printf(__VA_ARGS__);}
#endif

#define GET_MACRO(_1,_2,NAME,...) NAME
#define DEBUG(...) GET_MACRO(__VA_ARGS__, DEBUG2, DEBUG1)(__VA_ARGS__)
