#include <Bounce3.h>

const byte pin_num = 2;

// Standard instantiation just takes the pin number.
// If you want repeat button presses, you need to
// ask for it! :)
Bounce3 myButton(pin_num, B3_MODE_REPEAT);

unsigned int count;

void setup() {
  Serial.begin(115200);
  count = 0;

  delay(1000);
  Serial.println("I'm alive!");
}

void loop() {
  myButton.poll();

  // pressed() will return > 0 if an event happens. You could
  // simply check for that -- or use the more advanced return
  // values. In this case, we use the return values to advance
  // the counter by 10, and then 100.

  // Please note that pressed() is idempotent. You can check it
  // as many times as you want. Only calling poll() will update
  // the value.
  if(myButton.pressed()) {
    switch(myButton.pressed()) {
      case B3_PRESS:
        count += 1;
        break;
  
      case B3_REPEAT_SLOW:
        // Skip to the closest value of 10
        count += 10 - (count % 10);
        break;
  
      case B3_REPEAT_FAST:
        // Skip to the closest value of 100
        count += 100 - (count % 100);
        break;
    }
    
    Serial.println(count);
  }
}
