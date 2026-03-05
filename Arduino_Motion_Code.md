# Line by Line Arduino Code Explained for Motion Sensor

```
// PIR Motion Sensor with Normally Open Contact
// FrightProps Prop Controller Motion Trigger

const int MOTION_PIN = 2;  // Digital pin connected to the motion sensor
const int LED_PIN = 13;    // Built-in LED for visual feedback

// Variables for debouncing and timing
unsigned long lastMotionTime = 0;
unsigned long motionStartTime = 0;
bool motionDetected = false;
bool lastState = LOW;

// Debounce delay in milliseconds
const unsigned long debounceDelay = 50;
```

This is the configuration and variable declaration section for a PIR (Passive Infrared) motion sensor sketch — notably different from the pressure mat code in that it uses LOW as the default state rather than INPUT_PULLUP.

```
// PIR Motion Sensor with Normally Open Contact
// FrightProps Prop Controller Motion Trigger
```
Comment header identifying the purpose and target hardware. "Normally Open" means the circuit is open (no current flowing) when idle — motion closes it momentarily. FrightProps is a haunted attraction prop controller, so this is likely triggering a scare effect.

---
```
const int MOTION_PIN = 2;
```
Pins the PIR sensor signal wire to digital pin 2. Uses int rather than uint8_t — functionally identical for pin numbers, just a stylistic difference from the mat code.

---
```
const int LED_PIN = 13;
```
Assigns the built-in LED (physically on the Arduino board) to pin 13. This gives instant visual feedback during testing so you can confirm the sensor is triggering without needing the Serial Monitor open.

---
```
unsigned long lastMotionTime = 0;
```
Tracks the timestamp of the most recent motion event. Initialized to 0 since nothing has happened yet. Will be used to measure how long ago motion was last detected — useful for timeouts or cooldown logic in loop().

---
```
unsigned long motionStartTime = 0;
```
Tracks when the current motion event began. Having both lastMotionTime and motionStartTime suggests the code distinguishes between when motion started versus when it was last seen — useful for calculating event duration or sustaining a trigger output.

---
```
bool motionDetected = false;
```
A flag representing whether motion is currently active. Starting false since nothing has been detected yet. This is a higher-level state variable — distinct from the raw pin reading — used to track the logical state of the system rather than the electrical state of the pin.

---
```
bool lastState = LOW;
```
Stores the previous pin reading for change detection, initialized to LOW. Unlike the pressure mat code which initialized to HIGH (because of INPUT_PULLUP), PIR sensors typically output LOW at idle and pulse HIGH when motion is detected — so this default reflects the sensor's natural resting state.

---
```
const unsigned long debounceDelay = 50;
```
Sets a 50ms debounce window — much shorter than the pressure mat's 3000ms. PIR sensors produce cleaner, more deliberate signals than physical switches, so they need far less debounce time. 50ms is enough to filter electrical noise without dulling responsiveness.

