# Bounce3

A library written to address some shortcomings of various poll based Arduino
debouncing libraries--for novice programmers and advanced users alike.

Most debouncing libraries seem to follow the same pattern: check for recent
edge changes, and impose a blackout for a short period after. Readability may
differ, but four prominent issues arise.

First, the need for this project arose from struggling to use a poorly
written debouncing library. While I won't name names, `fell()` only reported
true when you let go of the button, not when you pressed it. I burned a lot of
time on this, given that my hardware was active low. Looking through
the code, I wasn't confident it'd capture tricky edge cases either, like
both edges of very short pulses.

Second, the size of the debouncer struct/object can get quite large. Imagine
sacrificing a quarter of your RAM to debouncing six buttons! **Bounce3**
scrapes in at 10 bytes, beating the largest library by a factor of five! One
tradeoff that made this possible was not supporting callbacks. (But hey,
this isn't JavaScript!)

Third, no library provides a native ability to retrigger when the button
is held down for a period of time. **Button2** provides excellent gesture
detection (ie: double click), but retriggering is useful for some very
standard projects, like clocks.

Finally, no library supports "atomic" updates. It may be a bit pedantic, but
I like to check all my inputs at the same time, when possible. This can
remove a few costly calls to `digitalRead` and `millis`, which might speed
up your loop by a few instructions. However, performance was not a goal, and
has not been measured yet. In any circumstance, the way `poll(ts, val)` is
exposed, it allows adapting this to other systems, and writing test cases.

## Example Code

While **Bounce3** is quite configurable and powerful, it remains simple
to use.

```C++
#include <Bounce3.h>

// Connected between D2 and GND
Bounce3 myButton(2);

void setup() {
  Serial.begin(115200);
}

void loop() {
  myButton.poll();

  // Your code here

  if(myButton.pressed()) {
    Serial.println("Button pushed!");
  }
}
```

## Feature Comparison

For the following table, sizes were calculated for the Arduino Uno. They will
most assuredly be larger on 32-bit platforms. Kudos to SLOCCount.

| Library | SLOC | Object Size | Polarity | Retrigger | Duration | Gestures | Callbacks | Atomic |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **Bounce3** | 110 lines | **10 bytes** | Both | **Fast, Slow** | Yes | - | - | **Yes** |
| FTDebouncer | 117 lines | 14 bytes | Both | - | - | - | - | - |
| Bounce2 | 141 lines | 16 bytes | High | - | Yes | - | - | - |
| DebounceFilterLib | 91 lines | 16 bytes | Both | - | - | - | Yes | - |
| ButtonDebounce | 52 lines | 18 bytes | Low | - | - | - | Yes | - |
| Button2 | 150 lines | 38 bytes | Low | - | Yes | **Yes** | Yes | - |
| EasyButton | 193 lines | 43 bytes | Both | - | Yes | - | Yes | - |

## Installation

To install this library, download it as a zipfile by clicking [here](https://github.com/tkuester/Bounce3/archive/master.zip). Open the Arduino IDE, and click (**Sketch** > **Include Library** > **Add .ZIP Library...**), and select the file you downloaded. From there, you can explore the examples under (**File** > **Examples** > **Bounce3**), and include it in your projects by clicking (**Sketch** > **Include Library** > **Bounce3**).

## TODO

 - List library in the Arduino repositories
 - Write test cases, esp. focused on rollover
 - Test on various architectures (Feather M0, ESP32)
 - Generate documentation
 - Tag proper releases
 - Performance testing? Instructions / poll?
 - Currently, overriding the `#define` configuration doesn't work -- but it should. Investigate this.
