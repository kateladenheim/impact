#include <Arduino.h>

#ifdef LED_BUILTIN
  const int LED_PIN = LED_BUILTIN;
#else
  const int LED_PIN = 1; // 13;
#endif

#define XSTR(x) STR(x)
#define STR(x) #x
#pragma message "Using pin settings: LED = " XSTR(LED_PIN) 

void setup() {
  pinMode (LED_PIN, OUTPUT);
}
  
void loop() {
  digitalWrite(LED_PIN, !!(millis() & (1 << 9)));
  delay(50);
}
