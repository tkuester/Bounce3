/**
 * repeat_advanced.ino
 *
 * One big advantage of repeat mode is scrolling through
 * options faster in a menu, or incrementing the time faster.
 * This example shows advanced use of the pressed() function
 * to make your firmware more user friendly!
 */
#include <Bounce3.h>

const byte pin_num = 2;
Bounce3 myButton(pin_num);

unsigned int count;

void setup() {
  Serial.begin(115200);
  count = 0;

  delay(1000);
  Serial.println("I'm alive!");
}

void loop() {
  myButton.poll();

  // pressed() will return > 0 if an event happens. This means
  // you can simply check for presses like this:
  //
  // if (btn.pressed())
  //
  // However, the actual value it returns provides insight as to
  // what type of press this is.
  //
  // Please note that pressed() is idempotent. You can check it
  // as many times as you want. Only calling poll() will update
  // the value.

  // Check for a button press
  if(myButton.pressed()) {

    // What type of button press are we talking about?
    switch(myButton.pressed()) {
      // This is the very first press, and happens instantly
      case B3_PRESS:
        count += 1;
        break;
  
      // You will get this after a 1 second delay
      case B3_REPEAT_SLOW:
        // Skip to the next closest value of 10
        count += 10 - (count % 10);
        break;
  
      // After a few slow repeats, it starts to speed up!
      case B3_REPEAT_FAST:
        // Skip to the next closest value of 100
        count += 100 - (count % 100);
        break;
    }
    
    Serial.println(count);
  }
}
