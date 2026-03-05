// PressureMatToSerial.ino
const uint8_t MAT_PIN = 2;  // digital pin connected to mat or trigger
const unsigned long DEBOUNCE_MS = 3000; // to prevent accidental triggers

bool lastState = HIGH;  // using INPUT_PULLUP, HIGH = not pressed, LOW = pressed
unsigned long lastChangeTime = 0;

void setup() {
  Serial.begin(115200);            // serial to computer
  pinMode(MAT_PIN, INPUT_PULLUP);  // internal pull-up enabled
  lastState = digitalRead(MAT_PIN);
  Serial.println("Pressure mat monitor started.");
}

void loop() {
  bool raw = digitalRead(MAT_PIN);
  unsigned long now = millis();

  // simple debounce: only accept changes that persist for DEBOUNCE_MS
  if (raw != lastState && (now - lastChangeTime) > DEBOUNCE_MS) {
    lastChangeTime = now;
    lastState = raw;

    if (lastState == LOW) {
      // mat pressed
      Serial.println("On1");
      // optionally include timestamp or count:
      // Serial.print("MAT: PRESSED at "); Serial.println(now);
    } else {
      // mat released
      Serial.println("Off1");
    }
  }

  // Short delay to avoid busy loop
  delay(5);
}
