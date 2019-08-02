#include <Arduino.h>
#include "Bounce3.h"

// Flags definition:
//
// LSB   MSB
// abcd efgh
// a - 1 == One Shot Mode, 0 == Repeat Mode 
// b - 1 == Active High, 0 == Active Low
// c - Reserved
// d - Last Value
//
// e - Pressed Flag
// f - Released Flag
// g - Press Type 0
// h - Press Type 1

#define LAST_PRESSED_BIT (1 << 3)
#define PRESSED_BIT  (1 << 4)
#define RELEASED_BIT  (1 << 5)

Bounce3::Bounce3(uint8_t pin) :
    Bounce3(pin,
            B3_MODE_ONESHOT | B3_ACTIVE_LOW,
            INPUT_PULLUP) { }

Bounce3::Bounce3(uint8_t pin, uint8_t mode) :
    Bounce3(pin, mode, (mode & B3_ACTIVE_HIGH ? INPUT : INPUT_PULLUP)) { }

Bounce3::Bounce3(uint8_t pin, uint8_t mode, uint8_t pin_mode) :
    pin(pin),
    flags(mode & 0x03), // Don't let the user set our internal variables
    last_ts(0),
    gesture_ts(0)
    {
    // Configure the pin
    pinMode(pin, pin_mode);
}

bool Bounce3::poll(uint32_t ts, uint8_t val) {
    const bool active_val = (flags & B3_ACTIVE_HIGH) > 0;
    const bool curr_pressed = (val > 0) == active_val;
    const bool last_pressed = (flags & LAST_PRESSED_BIT) > 0;
    const bool mode_oneshot = (flags & B3_MODE_ONESHOT) > 0;

    // Clear all the flags
    flags &= 0x0f;

    // Button state changes (after guard interval)
    if ((curr_pressed != last_pressed) && \
       (ts - last_ts) > B3_DEBOUNCE_MS) {
        flags |= (B3_PRESS << 6);
        if (curr_pressed) {
            flags |= PRESSED_BIT | LAST_PRESSED_BIT;
        } else {
            flags ^= (RELEASED_BIT | LAST_PRESSED_BIT);
            gesture_ts = ts - last_ts;
        }

        last_ts = ts;
        return true;
    }

    // Button is active, and not in oneshot mode
    if (curr_pressed && !mode_oneshot) {
        if ((ts - last_ts) > B3_FAST_WAIT_MS) {
            if ((ts - gesture_ts) > B3_FAST_INT_MS) {
                gesture_ts = ts;
                flags |= ((B3_REPEAT_FAST << 6) | PRESSED_BIT);
                return true;
            }
        } else if ((ts - last_ts) > B3_SLOW_WAIT_MS) {
            if ((ts - gesture_ts) > B3_SLOW_INT_MS) {
                gesture_ts = ts;
                flags |= ((B3_REPEAT_SLOW << 6) | PRESSED_BIT);
                return true;
            }
        }
    }

    return false;
}

bool Bounce3::poll(void) {
    return poll(millis(), digitalRead(pin));
}

uint8_t Bounce3::pressed(void) {
    return (flags & PRESSED_BIT) ? (flags >> 6) & 0x03 : 0;
}

uint32_t Bounce3::released(void) {
    if (flags & RELEASED_BIT) {
        return gesture_ts == 0 ? 1 : gesture_ts;
    }
    return 0;
}

bool Bounce3::changed(void) {
    return (flags & (PRESSED_BIT | RELEASED_BIT)) > 0;
}
