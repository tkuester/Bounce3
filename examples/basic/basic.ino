#include <Bounce3.h>

// To use this sketch, connect a button or switch between pin 2
// and ground, like this! (Really, any pin will do. I just
// happened to choose D2!)
//
// -------+
// Arduino|
//        |
//     D2 |------+
//        |      |
//        |      |
//        |        /   (Switch)
//        |      o/
//        |      |
//        |      |
//        |     === (GND)
//        |      =
//        |

// Standard instantiation just needs to know the pin number.
// You don't even need to call pinMode in setup, I'll do
// that for you behind the scenes!
const byte pin_num = 2;
Bounce3 myButton(pin_num);

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
