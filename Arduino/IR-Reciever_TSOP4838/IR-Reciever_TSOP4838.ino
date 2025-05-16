const int irReceiverPin = 4;  // Pin connected to the TSOP4838 output
unsigned long lastIRStateChange = 0;
unsigned long currentMillis = 0;
bool currentIRState = HIGH;
bool previousIRState = HIGH;

void setup() {
  pinMode(irReceiverPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  currentMillis = millis();
  currentIRState = digitalRead(irReceiverPin);

  // Check for state change
  if (currentIRState != previousIRState) {
    lastIRStateChange = currentMillis;
    Serial.println("IR State Changed!");
    Serial.print("Current State: ");
    Serial.println(currentIRState ? "HIGH" : "LOW");
  }

  previousIRState = currentIRState;

  // Update every 100 milliseconds
  delay(100);
}