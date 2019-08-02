#pragma once

#include <stdint.h>

#ifndef B3_DEBOUNCE_MS
#define B3_DEBOUNCE_MS (50UL)
#endif

/**
 * B3_SLOW_WAIT_MS
 *
 * How long to wait in milliseconds before repeating slowly
 */
#ifndef B3_SLOW_WAIT_MS
#define B3_SLOW_WAIT_MS (1000UL)
#endif

/**
 * B3_SLOW_INT_MS
 *
 * How long to wait between slow repeats. It is suggested to
 * keep this less than 32768 ms.
 */
#ifndef B3_SLOW_INT_MS
#define B3_SLOW_INT_MS (1000UL)
#endif

/**
 * B3_FAST_WAIT_MS
 *
 * How long to wait before transitioning to a faster repeat.
 * This timer counts from the initial button press, so it
 * should be larger than B3_SLOW_WAIT_MS unless you only want
 * a fast repeat.
 */
#ifndef B3_FAST_WAIT_MS
#define B3_FAST_WAIT_MS (B3_SLOW_WAIT_MS + (B3_SLOW_INT_MS * 4))
#endif

/**
 * B3_FAST_INT_MS
 *
 * How long to wait between fast repeats. It is suggested to
 * keep this less than 32768 ms.
 */
#ifndef B3_FAST_INT_MS
#define B3_FAST_INT_MS (250UL)
#endif

#define B3_MODE_ONESHOT     (1 << 0)
#define B3_MODE_REPEAT      (0 << 0)
#define B3_ACTIVE_HIGH      (1 << 1)
#define B3_ACTIVE_LOW       (0 << 1)

#define B3_PRESS            (1)
#define B3_REPEAT_SLOW      (2)
#define B3_REPEAT_FAST      (3)

class Bounce3 {
  private:
    uint8_t pin;
    uint8_t flags;
    uint32_t last_ts;
    uint32_t gesture_ts;

  public:
    /**
     * Constructor
     *
     * pin - Which digital pin to read
     * mode: A bitwise-or B3_MODE* and B3_ACTIVE* (default: oneshot active low)
     * pin_mode: If you wish to specify pinMode (default: INPUT_PULLUP)
     */
    Bounce3(uint8_t pin, uint8_t mode, uint8_t pin_mode);
    Bounce3(uint8_t pin, uint8_t mode);
    Bounce3(uint8_t pin);

    /**
     * Updates the button state.
     *
     * ts - The timestamp to use. (default: millis())
     * val - The pin reading. (default: digitalRead(pin))
     *
     * Returns: true, if the button state changes
     */
    bool poll(uint32_t ts, uint8_t val);
    bool poll(void);

    /**
     * Determine if the button was pressed this loop. If the button was not
     * pressed, the function returns zero. A non-zero value indicates the
     * button was pressed, so you can evaluate it as a boolean.
     *
     * If the instance is configured in one-shot mode, it will only return
     * B3_PRESS once per debounced button press.
     *
     * If the instance is configured in repeat mode, it will return
     * B3_REPEAT_SLOW or B3_REPEAT_FAST to allow the code to differentiate
     * between the first press and repeat presses (if desired).
     *
     * Return Values:
     * 0 - Not pressed
     * B3_PRESS - Was just pressed
     * B3_REPEAT_SLOW - A virtual "press" on slow repeat
     * B3_REPEAT_FAST - A virtual "press" on fast repeat
     */
    uint8_t pressed(void);

    /**
     * Returns the number of milliseconds the button was
     * held down, or 0 if the button was not just released.
     */
    uint32_t released(void);

    /**
     * Returns true if the button was just pressed or released.
     */
    bool changed(void);
};
