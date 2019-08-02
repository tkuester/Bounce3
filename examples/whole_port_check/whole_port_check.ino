/**
 * whole_port_check.ino
 *
 * Here's a slightly more advanced trick. Let's say you have six
 * buttons. On an Arduino UNO, you can you can read them all in
 * one clock cycle, and feed the results to Bounce3. This might
 * save a few clock cycles, and makes the firmware behave...
 * perhaps more predictably.
 *
 * That being said... while Bounce3 was written to be fairly
 * conservative with CPU, the repeat mode and configuration
 * checks will eat a decent bit of time. Ya win some, ya lose
 * some...
 */
#include <Bounce3.h>

// It should be noted, this code was written specifically with
// AVR8 hardware in mind. The concept can be ported to other
// platforms, though! If you do this, please submit a pull
// request!
#ifndef ARDUINO_ARCH_AVR
#error This example written for the AVR8 platform only!
#endif

Bounce3 buttons[6] = {Bounce3(8),
                      Bounce3(9),
                      Bounce3(10),
                      Bounce3(11),
                      Bounce3(12),
                      Bounce3(13)};
unsigned int count[6] 

void setup() {
  Serial.begin(115200);

  // Ooo, look! memset. If you're reading raw Arduino
  // registers, this shouldn't be too exciting for you.
  memset(count, 0, sizeof(count));

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
