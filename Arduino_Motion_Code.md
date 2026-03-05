# Line by Line Arduino Code Explained for Motion Sensor

| [Part 1](#part-1) | [Part 2](#part-2) | [Part 3](#part-3) |
|---|---|---|

## Part 1
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

---
## Part 2
```
void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Configure pins
  pinMode(MOTION_PIN, INPUT_PULLUP);  // Use internal pullup resistor
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("PIR Motion Sensor Ready");
  Serial.println("Waiting for motion...");
  digitalWrite(LED_PIN, LOW);
}
```
Same structure as the pressure mat's setup() — runs once at power-on to initialize everything before the main loop begins.
```
Serial.begin(9600);
```
Opens serial communication at 9600 baud — notably slower than the pressure mat's 115200. For a motion sensor that only sends occasional trigger events, 9600 is perfectly adequate. The tradeoff is that it's slower to transmit, but that won't matter when messages are infrequent.

---
```
pinMode(MOTION_PIN, INPUT_PULLUP);
```
Configures the motion sensor pin as an input with the internal pull-up resistor enabled — same circuit strategy as the pressure mat. This keeps the pin reading HIGH at rest, so a motion event pulls it LOW. Worth noting this is the opposite of typical PIR behavior (which usually outputs HIGH on detection), so this sensor is likely wired with a normally-open relay or open-collector output rather than a direct PIR signal line.

---
```
pinMode(LED_PIN, OUTPUT);
```
Sets pin 13 as an output so the Arduino can drive the built-in LED on and off. Pins default to INPUT on boot, so this line is required before you can use digitalWrite() on it.

---
```
Serial.println("PIR Motion Sensor Ready");
Serial.println("Waiting for motion...");
```
Two startup confirmation messages sent to the Serial Monitor. Splitting them across two lines gives a cleaner readout and signals that initialization completed successfully. Useful during development to confirm the board didn't stall or reset mid-setup.

---
```
digitalWrite(LED_PIN, LOW);
```
Explicitly sets the LED to off at startup. While the LED is typically off by default, this is good defensive practice — it guarantees a known state regardless of what the pin may have been doing before a reset, making the startup behavior predictable.

---
## Part 3
```
void loop() {
  // Read the sensor state (normally open, closes when motion detected)
  // HIGH = motion detected (contact closed, voltage present)
  int reading = digitalRead(MOTION_PIN);
  
  // Debounce the input
  static unsigned long lastDebounceTime = 0;
  static int lastReading = LOW;
  
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the reading has been stable for debounceDelay
    if (reading != lastState) {
      lastState = reading;
      
      if (reading == HIGH) {
        // Motion detected! (contact closed, voltage present)
        if (!motionDetected) {
          motionDetected = true;
          motionStartTime = millis();
          digitalWrite(LED_PIN, HIGH);
          
          Serial.print("On");
        }
      } else {
        // No motion (contact open, pin reads LOW)
        if (motionDetected) {
          motionDetected = false;
          unsigned long motionDuration = millis() - motionStartTime;
          digitalWrite(LED_PIN, LOW);
          
          Serial.println("--- Motion ended ---");
          Serial.print("Duration: ");
          Serial.print(motionDuration);
          Serial.println(" ms");
          Serial.println("Waiting for motion...");
        }
      }
      
      lastMotionTime = millis();
    }
  }
  
  lastReading = reading;
  
  // Small delay to prevent overwhelming the serial monitor
  delay(10);
}
```

This is the most complex loop of the two sketches — it uses a two-stage debounce pattern and carefully separates raw readings from logical state to avoid duplicate triggers.

```
int reading = digitalRead(MOTION_PIN);
```
Takes a fresh raw sample of the sensor pin every loop cycle. This is intentionally kept separate from lastState — reading is the unvalidated electrical signal, while lastState is the accepted, debounced truth.

---
```
static unsigned long lastDebounceTime = 0;
static int lastReading = LOW;
```
Two static local variables — unlike regular locals, static variables persist their values between loop calls without being global. This is a clean way to keep debounce bookkeeping self-contained inside loop() rather than cluttering the global scope. Both initialize only once at first execution.

---
```
if (reading != lastReading) { lastDebounceTime = millis(); }
```
Stage one of the debounce. Any time the raw reading flickers or changes, the debounce clock resets. This means the reading must hold stable for the full debounceDelay duration before being acted on — not just be different from the last accepted state.

---
```
if ((millis() - lastDebounceTime) > debounceDelay)
```
Stage two of the debounce. Only proceeds if the reading has been stable for longer than debounceDelay (50ms). Everything meaningful happens inside this block — it's the gate that separates electrical noise from genuine events.

---
```
if (reading != lastState) { lastState = reading;
```
A secondary change check inside the debounce gate. Ensures the stable reading actually differs from the last accepted state — prevents re-processing the same state on every loop iteration after stabilization. Updates lastState to the newly accepted value.

---
```
if (reading == HIGH)
```
Since INPUT_PULLUP is used, you might expect LOW to mean "active" — but the comment clarifies this sensor closes contact and brings voltage present, so HIGH here means motion detected. The wiring is doing something slightly unconventional, and this comment is doing important work clarifying that.

---
```
if (!motionDetected) {
```
Guards against re-triggering if motion is already being tracked. Without this, every stable HIGH reading would re-fire the trigger — this ensures the "motion started" block runs exactly once per event.

---
```
motionDetected = true;
motionStartTime = millis();
```
Sets the logical flag and captures the start timestamp. motionStartTime will be used later to calculate how long the motion event lasted.

---
```
digitalWrite(LED_PIN, HIGH);
Serial.print("On");
```
Turns the LED on as visual confirmation and sends "On" over serial — notably Serial.print not Serial.println, so no newline is appended. This may be intentional if the receiving system expects a specific format without line endings.

---
```
if (motionDetected) {
```
Mirror guard for the release side — ensures the "motion ended" block only runs once when transitioning from active back to idle, not on every stable LOW reading.

---
```
motionDetected = false;
unsigned long motionDuration = millis() - motionStartTime;
```
Clears the flag and calculates event duration by subtracting the stored start time from now. This is the payoff for having tracked motionStartTime — you get a precise millisecond duration for each event.

---
```
digitalWrite(LED_PIN, LOW);
```
Turns the LED off, mirroring the HIGH set on detection.

---
```
Serial.println("--- Motion ended ---");
Serial.print("Duration: "); Serial.print(motionDuration); Serial.println(" ms");
Serial.println("Waiting for motion...");
```
A multi-line serial report logging the end of the event and its duration. Breaking the duration across two Serial.print() calls lets you concatenate the number and unit label cleanly without string formatting.

---
```
lastMotionTime = millis();
```
Updates the last-motion timestamp after either a start or end event. Placed outside both if/else branches so it fires on any accepted state change — useful if other logic elsewhere needs to know when the sensor last did anything.

---
```
lastReading = reading;
```
Updates the raw reading tracker for the next debounce comparison. This lives outside the debounce gate — it updates every cycle regardless, which is correct since the debounce timer needs to see every raw fluctuation.

---
```
delay(10);
```
A 10ms pause — double the pressure mat's 5ms, but still short enough to be imperceptible. Keeps the loop from hammering digitalRead() and the serial buffer unnecessarily.
