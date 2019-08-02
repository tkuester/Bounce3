/**
 * debounce_comparison.ino
 *
 * Compares the debouncer library to a naive implementation.
 * Uses an active low button, with the Arduino's internal
 * pullup resistor.
 *
 * Note that this example was coded to work with a button
 * connected to ground, as in basic.ino. You will need to
 * modify the program to work with an active high button!
 */

#include <Bounce3.h>

// To compare against the naive debouncer, we want to be
// configured in one shot mode: only log one press per
// key down. Technically we don't need to specify
// any of the optional variables, but I'm doing it to show
// off different parts of the library.
const byte pin_num = 2;
Bounce3 myButton(pin_num,
                 B3_MODE_ONESHOT | B3_ACTIVE_LOW,
                 INPUT_PULLUP);

// Printing to the screen too often makes the loop run
// longer, and adds "pseudo debouncing". By rate limiting
// how often we print, we can get a more accurate measure
// of how bad the button is bouncing.
long int last_print;
uint8_t last_btn;

// Counters
unsigned int debounce_count;
unsigned int naive_count;

void setup() {
  Serial.begin(115200);

  last_print = 0;
  debounce_count = 0;
  last_btn = HIGH;
}

void loop() {
  unsigned long now = millis();
  uint8_t btn = digitalRead(pin_num);

  // Normally, you'd just call .poll() by itself. However,
  // poll allows us to feed in timestamps and the button
  // value for occasions like this. This way, Bounce3 sees
  // exactly what the naive implementation sees.
  myButton.poll(now, btn);

  if(myButton.pressed()) {
    debounce_count += 1;
  }
  
  // A naive debouncing algorithm
  if(btn != last_btn && btn == LOW) {
    naive_count += 1;
  }
  last_btn = btn;

  // Rate limit printing to the screen
  if((millis() - last_print) > 1000) {
    Serial.print("Bounce: ");
    Serial.print(naive_count, DEC);
    Serial.print(", Debounced: ");
    Serial.println(debounce_count, DEC);
    
    last_print = millis();
  }
}
