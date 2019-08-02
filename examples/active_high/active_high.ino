/**
 * active_low.ino
 *
 * Some times you wanna be different. Personally, I think pull-up
 * resistors are the way to go, but maybe you're running with
 * hardware that uses pull-down resistors.
 *
 * Although the Arduino itself doesn't have an internal pull-down
 * resistor, other platforms (like the Feather M0) may. If
 * INPUT_PULLDOWN is defined, we'll use it! If not, you'll need
 * to put an external resistor down on the board.
 *
 * Remember to connect your push button to the correct supply!
 * If your micro runs on 3.3V, don't connect it to 5V!
 *
 * -------+
 * Arduino|           (Push button)
 *        |           __I__
 *     D2 |------+----o   o------[ VDD (5 V or 3.3V) ]
 *        |      |
 *        |      Z
 *        |      Z (10 kOhm, if needed)
 *        |      Z 
 *        |      |
 *        |      |
 *        |     === (GND)
 *        |      =
 *        |
 */

#include <Bounce3.h>

const byte pin_num = 2;
Bounce3 myButton(pin_num, B3_ACTIVE_HIGH);

// If you only want to get one count per press, try setting Bounce3
// to run in one-shot mode!
//Bounce3 myButton(pin_num, B3_ACTIVE_HIGH | B3_MODE_ONESHOT);

// Let's keep track of how many times the button has been
// pressed. We'll put this variable in global scope, so it can keep
// it's value for each call to loop().
unsigned int count;

void setup() {
  // Initialize things
  Serial.begin(115200);
  count = 0;

  // Wait 1 second for the user to open the serial console,
  // and greet them.
  delay(1000);
  Serial.println("I'm alive!");
}

void loop() {
  // Call this at the start of every loop!
  myButton.poll();

  // Put your code here. Remember, programming for
  // microcontrollers is weird. You want your loop() function
  // to execute as quickly as possible, so try not to spend
  // too much time doing things.

  // Check to see if your button got pressed like this!
  if(myButton.pressed()) {
    count += 1;
    Serial.print("Button was pressed: count = ");
    Serial.println(count, DEC);
  }

  else if(myButton.released()) {
    Serial.print("Button was pressed for ");
    Serial.print(myButton.released(), DEC);
    Serial.println(" ms!");
    Serial.println("===========");
  }
}
