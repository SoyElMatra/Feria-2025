const int irEmitterPin = 3;
const int timeIR = 25;
void setup() {
  pinMode(irEmitterPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  tone(irEmitterPin, 38000, timeIR);
  delay(timeIR);
}

