# Test

```
// PressureMatToSerial.ino
const uint8_t MAT_PIN = 2;  // digital pin connected to mat or trigger
const unsigned long DEBOUNCE_MS = 3000; // to prevent accidental triggers

bool lastState = HIGH;  // using INPUT_PULLUP, HIGH = not pressed, LOW = pressed
unsigned long lastChangeTime = 0;
```

This is the global configuration and variable declaration section at the top of the sketch — everything here is available to both setup() and loop().

**const uint8_t MAT_PIN = 2;**
Declares a constant named MAT_PIN with a value of 2, meaning the pressure mat is wired to digital pin 2. Using uint8_t (an unsigned 8-bit integer) is a memory-efficient choice for storing a pin number since it only needs values 0–255. const ensures this value can never be accidentally changed at runtime.

**const unsigned long DEBOUNCE_MS = 3000;**
Sets a debounce window of 3000 milliseconds (3 seconds). Debouncing prevents rapid, unintended re-triggers — in this context, it means once the mat is triggered, the code will ignore further state changes for 3 full seconds. This is a long debounce compared to typical button use (usually 20–50ms), suggesting the mat is meant to detect deliberate, sustained events rather than quick taps.

**bool lastState = HIGH;**
A boolean variable that tracks the mat's previous state, initialized to HIGH. The comment explains the logic: because INPUT_PULLUP is used, HIGH means the mat is not pressed and LOW means it is pressed. This inverted logic is standard for pull-up circuits. This variable will be compared against the current reading in loop() to detect changes.

**unsigned long lastChangeTime = 0;**
Stores the timestamp (in milliseconds) of the last detected state change. Initialized to 0 since no change has occurred yet. This will be compared against millis() later to enforce the debounce window — if not enough time has passed since lastChangeTime, new triggers get ignored.


```
void setup() {
  Serial.begin(115200);            // serial to computer
  pinMode(MAT_PIN, INPUT_PULLUP);  // internal pull-up enabled
  lastState = digitalRead(MAT_PIN);
  Serial.println("Pressure mat monitor started.");
}
```

**void setup()**

The Arduino setup function — runs once when the board powers on or resets. Used for initialization.

**Serial.begin(115200);**

Opens a serial communication channel between the Arduino and your computer at a baud rate of 115200 bits per second. This lets you send debug messages you can read in the Serial Monitor.

**pinMode(MAT_PIN, INPUT_PULLUP)**

Configures the pin defined by MAT_PIN as a digital input with the internal pull-up resistor enabled. The pull-up resistor keeps the pin reading HIGH by default — so when the pressure mat connects it to ground, the pin reads LOW. This is the standard way to wire a simple switch or mat without needing an external resistor.

**lastState = digitalRead(MAT_PIN)**

Reads the current state of MAT_PIN (either HIGH or LOW) and stores it in the variable lastState. This captures the baseline state of the mat at startup, which is important for detecting changes later in the loop() function rather than just the raw state.

**Serial.println("Pressure mat monitor started.")**

Sends a confirmation string to the Serial Monitor followed by a newline. Useful for knowing the board has initialized correctly — a simple "it's alive" sanity check.


