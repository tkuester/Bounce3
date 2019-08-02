/**
 * long_press.ino
 * 
 * Some times, you want to know if a button was pressed for
 * a long period of time. This can help guard against accidental
 * presses, like when setting the time. Here's a simple example,
 * involving something called a "Finite State Machine".
 * 
 * +--------------+                         +--------------+
 * |              | --- (long press ) --- > |              |
 * |  sm = false  |                         |  sm = true   |
 * |              | < --- (short press) --- |              |
 * +--------------+                         +--------------+
 * 
 * There are some tricky caviats to this method, however. Firstly,
 * heldFor() makes a second call to millis(). If you call it more
 * than once per loop, it may return a different value. This is
 * where the "atomic" feature is handy -- but that is an exercise
 * left to the reader.
 * 
 * Here's another tricky thought: how do you make sure the user
 * releases the button before switching states? Remember -- the
 * state only updates when you call poll(). But furthermore...
 * does it even matter if the user releases the button?
 */
#include <Bounce3.h>

const byte pin_num = 2;
Bounce3 myButton(pin_num);

bool set_mode;

void setup() {
  // Initialize things
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  set_mode = false;

  // Wait 1 second for the user to open the serial console,
  // and greet them.
  delay(1000);
  Serial.println("I'm alive!");
}

void loop() {
  myButton.poll();

  if(set_mode) {
    if(myButton.pressed()) {
      Serial.println("Leaving set mode");
      digitalWrite(LED_BUILTIN, LOW);
      set_mode = false;
    }
  } else {
    if(myButton.pressed()) {
      Serial.println("Not so fast! You aren't getting "
                     "to set mode that easily!");
    }
    if(myButton.heldFor() > 3000) {
      Serial.println("Fine, you held it for long enough!");
      digitalWrite(LED_BUILTIN, HIGH);
      set_mode = true;
    }
  }
}
