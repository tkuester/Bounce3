/**
 * @file Bounce3.h
 * @mainpage Bounce3 Debouncing Library
 *
 * Bounce3 was written to address some shortcomings of various poll based
 * Arduino debouncing libraries--for novice programmers and advanced users
 * alike.
 *
 * The chief feature of this library is a small, unchanging RAM footprint.
 * Each button object only consumes ten bytes of RAM, and this does not change
 * when moving from 8-bit to 32-bit platforms.
 *
 * In addition, it has a "repeat" mode, which allows the programmer to easily
 * add functionality similar to setting a clock. When the user holds the
 * button down for an extended period of time, the library will report
 * multiple presses.
 *
 * @section example Example Code
 *
 * While **Bounce3** is quite configurable and powerful, it remains simple
 * to use.
 *
 * @code
 * #include <Bounce3.h>
 *
 * // Connected between D2 and GND
 * Bounce3 myButton(2);
 *
 * void setup() {
 *   Serial.begin(115200);
 * }
 * 
 * void loop() {
 *   myButton.poll();
 *   
 *   // Your code here
 *
 *   if(myButton.pressed()) {
 *     Serial.println("Button pushed!");
 *   }
 * }
 * @endcode
 *
 * @section deps Hardware Dependencies
 * For common usage, this library requires a functioning `millis()` call.
 * If this call is not working, or does not exist, you can pass in the
 * number of milliseconds since boot via the atomic functions.
 *
 * This also uses the `pinMode()` and `digitalRead()` commands from the
 * standard Arduino library.
 *
 * This library was written to be platform independent, and has been
 * tested to work on the Arduino Uno, the Feather M0, and the ESP32.
 *
 * @section author Author
 * <a href="https://github.com/tkuester/">Tim Kuester</a>
 *
 * @section license License
 * MIT Licensed, 2019
 */
#pragma once
#include <stdint.h>

/**
 * @defgroup B3_TIMING Timing values
 * @brief Override these values to change how the timing feels
 * @{
 */

/**
 * @def B3_DEBOUNCE_MS
 * @brief How long the debounce interval is
 */
#ifndef B3_DEBOUNCE_MS
#define B3_DEBOUNCE_MS (50UL)
#endif


/**
 * @def B3_SLOW_WAIT_MS
 * @brief How long to wait in milliseconds before repeating slowly
 */
#ifndef B3_SLOW_WAIT_MS
#define B3_SLOW_WAIT_MS (1000UL)
#endif

/**
 * @def B3_SLOW_INT_MS
 * @brief How long to wait between slow repeats.
 */
#ifndef B3_SLOW_INT_MS
#define B3_SLOW_INT_MS (1000UL)
#endif

/**
 * @def B3_FAST_WAIT_MS
 * @brief How long to wait before transitioning to a faster repeat.
 *
 * This timer counts from the initial button press, so it
 * should be larger than #B3_SLOW_WAIT_MS unless you only want
 * a fast repeat.
 */
#ifndef B3_FAST_WAIT_MS
#define B3_FAST_WAIT_MS (B3_SLOW_WAIT_MS + (B3_SLOW_INT_MS * 4))
#endif

/**
 * @def B3_FAST_INT_MS
 * @brief How long to wait between fast repeats.
 */
#ifndef B3_FAST_INT_MS
#define B3_FAST_INT_MS (250UL)
#endif
/** @} */

/**
 * @defgroup B3_MODE config values
 * @brief Bitwise or these together for the constructor mode
 * @{
 */
/** @brief Only register the press and release */
#define B3_MODE_ONESHOT     (1 << 0)
/** @brief Register the press, and repeat if held down */
#define B3_MODE_REPEAT      (0 << 0)
/** @brief The button has a pulldown resistor */
#define B3_ACTIVE_HIGH      (1 << 1)
/** @brief The button has a pullup resistor */
#define B3_ACTIVE_LOW       (0 << 1)
/** @} */

/**
 * @defgroup B3_PRESSED Button pressed values
 * @brief Return values for Bounce3::pressed()
 * @{
 */

/** @brief Return value if the button was just pressed */
#define B3_PRESS            (1)

/** @brief Return value if the button is held, and is repeating slowly */
#define B3_REPEAT_SLOW      (2)

/** @brief Return value if the button is held, and is repeating quickly */
#define B3_REPEAT_FAST      (3)
/** @} */

/**
 * @brief An object that debounces a single GPIO
 */
class Bounce3 {
  private:
    uint8_t pin;
    uint8_t flags;
    uint32_t last_ts;
    uint32_t gesture_ts;

  public:
    /**
     * @param pin Which digital pin to read
     * @param mode A bitwise-or of @ref B3_MODE (default: `#B3_MODE_ONESHOT | #B3_ACTIVE_LOW`)
     * @param pin_mode If you wish to specify pinMode (default: INPUT_PULLUP)
     */
    Bounce3(uint8_t pin, uint8_t mode, uint8_t pin_mode);
    /** @overload Bounce3::Bounce3(uint8_t pin, uint8_t mode, uint8_t pin_mode) */
    Bounce3(uint8_t pin, uint8_t mode);
    /** @overload Bounce3::Bounce3(uint8_t pin, uint8_t mode, uint8_t pin_mode) */
    Bounce3(uint8_t pin);

    /**
     * @brief Update the button state.
     *
     * @param ts The timestamp to use. (default: millis())
     * @param val The pin reading. (default: digitalRead(pin))
     *
     * @return true, if the button state changes
     */
    bool poll(uint32_t ts, uint8_t val);
    /** @overload bool Bounce3::poll(uint32_t ts, uint8_t val) */
    bool poll(void);

    /**
     * @brief Check if the button was pressed this loop.
     *
     * If the button was not pressed, the function returns zero. A non-zero
     * value indicates the button was pressed, so you can evaluate it as a
     * boolean.
     *
     * If the instance is configured in one-shot mode, it will only return
     * #B3_PRESS once per debounced button press.
     *
     * If the instance is configured in repeat mode, it will return
     * #B3_REPEAT_SLOW or #B3_REPEAT_FAST to allow the code to differentiate
     * between the first press and repeat presses (if desired).
     *
     * @return 0 - Not pressed
     * @return #B3_PRESS - Was just pressed
     * @return #B3_REPEAT_SLOW - A virtual "press" on slow repeat
     * @return #B3_REPEAT_FAST - A virtual "press" on fast repeat
     */
    uint8_t pressed(void);

    /**
     * @return true if the button is currently held down.
     */
    bool held(void);

    /**
     * @param millis Atomic timestamp
     * @return How long the button has been held down in ms
     * @return 0, if the button is not currently presesd
     */
    uint32_t heldFor(uint32_t millis);
    /** @overload Bounce3::heldFor(uint32_t millis) */
    uint32_t heldFor(void);

    /**
     * @return The number of milliseconds the button was held down
     * @return or 0 if the button was not just released.
     */
    uint32_t released(void);

    /**
     * @return true if the button was just pressed or released
     */
    bool changed(void);
};
