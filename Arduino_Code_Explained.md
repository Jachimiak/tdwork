# Test

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


