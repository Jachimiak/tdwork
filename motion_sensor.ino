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