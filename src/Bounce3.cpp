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

#define LAST_VAL_BIT (1 << 3)
#define PRESSED_BIT  (1 << 4)
#define RELEASED_BIT  (1 << 5)

Bounce3::Bounce3(uint8_t pin) :
    Bounce3(pin,
            B3_MODE_ONESHOT | B3_ACTIVE_LOW,
            INPUT_PULLUP) { }

Bounce3::Bounce3(uint8_t pin, uint8_t mode) :
    Bounce3(pin,
            mode,
            INPUT_PULLUP) { }

Bounce3::Bounce3(uint8_t pin, uint8_t mode, uint8_t pin_mode) :
    pin(pin) {
    // Don't let the user set our internal variables
    this->flags = mode & 0x03;

    // Configure the pin
    pinMode(pin, pin_mode);

    // Update the "last value" based on whether or not the signal
    // is active high or not.
    if ((this->flags & B3_ACTIVE_HIGH) == 0) {
        this->flags |= LAST_VAL_BIT;
    }
}

bool Bounce3::poll(uint32_t ts, uint8_t val) {
    const bool curr_val = val > 0;
    const bool active_val = (flags & B3_ACTIVE_HIGH) > 0;
    const bool last_val = (flags & LAST_VAL_BIT) > 0;

    // Clear the pressed/release flag
    flags &= ~(PRESSED_BIT | RELEASED_BIT);

    // Button state changes (after guard interval)
    if ((curr_val != last_val) && \
       (ts - last_ts) > B3_DEBOUNCE_MS) {
        flags &= 0x3f;
        flags |= (B3_PRESS << 6) | (curr_val == active_val ? PRESSED_BIT : RELEASED_BIT);

        // Housekeeping: Update the last valid value
        if (curr_val) {
            this->flags |= LAST_VAL_BIT;
        } else {
            this->flags &= ~(LAST_VAL_BIT);
        }

        if(curr_val != active_val) {
            gesture_ts = ts - last_ts;
        }

        last_ts = ts;
        return true;
    }

    // Button is active, and not in oneshot mode
    if ((curr_val == active_val) && (flags & B3_MODE_ONESHOT) == 0) {
        if ((ts - last_ts) > B3_FAST_WAIT_MS) {
            if ((ts - gesture_ts) > B3_FAST_INT_MS) {
                gesture_ts = ts;
                flags &= 0x3f;
                flags |= ((B3_REPEAT_FAST << 6) | PRESSED_BIT);
                return true;
            }
        } else if ((ts - last_ts) > B3_SLOW_WAIT_MS) {
            if ((ts - gesture_ts) > B3_SLOW_INT_MS) {
                gesture_ts = ts;
                flags &= 0x3f;
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
