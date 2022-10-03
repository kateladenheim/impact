// breathe library
#include <Arduino.h>

#include "breathe.h"


#ifdef ESP32
inline void analogWrite(int pin, int value) {
    ledcWrite(pin, 255-value);
}
#endif

Breathe::Breathe(int pin, bool invert) {
    pinMode(pin, OUTPUT);
#   ifdef ESP32
        ledcAttachPin(pin, pin); // assign RGB led pins to channels
        ledcSetup(pin, 12000, 8); // 12 kHz PWM, 8-bit resolution
#   endif

    _pin = pin;
    _invert = invert;

    // default values for status LED 
    this->setPeriod(13, 10);
    if (invert)
        setValue(250, 2);
    else
        setValue(0, 1);
}


void Breathe::setPeriod(int period, int duty) {
    _period = (1 << period) - (1 << duty);
    _duty = (1 << duty) - 1;
    _flip = 1 << (duty - 1);
}

void Breathe::setValue(int base, int shift) {
    _base = base;
    _shift = shift;
}

void Breathe::on() {
    digitalWrite(_pin, !_invert);
}

void Breathe::off() {
    digitalWrite(_pin, _invert);
}

void Breathe::loop() {
  uint32_t time = millis();

  if (time & _period) 
    analogWrite(_pin, _base);
  else {
    uint32_t dt = time & _duty;
    if (dt & _flip) 
        dt = (~dt) & (_duty - _flip);
    dt >>= _shift;
    dt = _invert ? _base - dt : _base + dt;
    analogWrite(_pin, dt);
  }
}
