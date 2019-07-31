#include <Bounce3.h>

// Here's a neat trick. If you have a bunch of buttons... let's
// say six. If you're trying to conserve CPU cycles, you can read
// them all in one clock cycle, and feed the results to Bounce3.

// That being said... while Bounce3 was written to be fairly
// conservative with CPU, the retrigger code and configuration
// checks will eat a decent bit of time. Ya win some, ya lose
// some...

Bounce3 buttons[6];
unsigned int count[6];

void setup() {
  Serial.begin(115200);

  // Ooo, look! memset. If you're reading raw Arduino
  // registers, this shouldn't be too exciting for you.
  memset(count, 0, sizeof(count));
  
  // Setup pins 8-13. (Also, note! You don't need to instantiate
  // this in global scope.) Remember, it's still important to
  // specify pin number: Bounce3 is calling pinMode for us.
  for(uint8_t i = 0; i < 6; i++) {
    buttons[i] = Bounce3(i + 8);
  }

  Serial.println("I'm alive!");
}

void loop() {
  // I like calling millis() only once per loop, if I can.
  unsigned long loop_ts = millis();
  
  // PINB holds the value of pins D8-D13 in bits [0..5]. We
  // just read them all at once. Much more "atomic".
  byte btns = PINB;

  // Update all the buttons, one at a time.
  for(uint8_t i = 0; i < 6; i++) {
    buttons[i].poll(loop_ts, (btns >> i) & 0x01);

    if(buttons[i].pressed()) {
      Serial.print("Button ");
      Serial.print(i + 8, DEC);
      Serial.println(" pressed!");
    }
    else if(buttons[i].released()) {
      Serial.print("Button ");
      Serial.print(i + 8, DEC);
      Serial.println(" released!");
    }
  }
}
