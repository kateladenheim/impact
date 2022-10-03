#include <Arduino.h>

#ifdef LED_BUILTIN
  const int LED_PIN = LED_BUILTIN;
#else
  const int LED_PIN = 2; // LED connected to pin 2 on esp01s
#endif

#include <breathe.h>

Breathe b(LED_PIN);

void setup() {
}

void loop() {
  b.loop();
  delay(50);
}
