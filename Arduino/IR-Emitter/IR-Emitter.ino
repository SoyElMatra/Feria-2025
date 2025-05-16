const int irEmitterPin = 3;  // IR emitter connected to Pin 3 (supports 38kHz with tone())

void setup() {
  pinMode(irEmitterPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("IR Emitter Test");
}

void loop() {
  // Turn the IR signal on at 38kHz for 1 second
  tone(irEmitterPin, 38000, 1000);
  Serial.println("IR Signal: ON");
  delay(4000);

  // Turn the IR signal off
  noTone(irEmitterPin);
  Serial.println("IR Signal: OFF");
  delay(4000);
}

