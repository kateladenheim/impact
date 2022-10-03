#pragma once

#include <Arduino.h>

// Default LED pin:
//      Use LED_BUILTIN_AUX if it is defined for the current board,
//      Otherwise use LED_BUILTIN if it is defined for the current board,
//      Otherwise use pin 2 for esp01s (generic)

#if defined(LED_BUILTIN_AUX)
#   define BREATHE_LED_PIN LED_BUILTIN_AUX
#elif defined(LED_BUILTIN)
#   define BREATHE_LED_PIN LED_BUILTIN
#else
#   define BREATHE_LED_PIN 2
#endif

class Breathe {
    public:
        Breathe(int pin = BREATHE_LED_PIN, bool invert = true);
        void setPeriod(int period, int duty);
        void setValue(int base, int shift);
        void loop();
        void on();
        void off();
    private:
        int _pin;
        int _invert;
        int _period;
        int _duty;
        int _flip;
        int _base;
        int _shift;
};
