const int irEmitterPin = 3;
const int BURST_DURATION = 15;   // ms de pulso IR continuo
const int PAUSE_DURATION = 25;   // ms entre pulsos
const int CARRIER_FREQ = 38000;  // Frecuencia portadora

void setup() {
  pinMode(irEmitterPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Emisor IR mejorado iniciado");
}

void sendBurst() {
  unsigned long startTime = millis();
  
  while(millis() - startTime < BURST_DURATION) {
    // Generar onda cuadrada de 38kHz manualmente
    digitalWrite(irEmitterPin, HIGH);
    delayMicroseconds(13);  // Medio período a 38kHz (26μs ciclo completo)
    digitalWrite(irEmitterPin, LOW);
    delayMicroseconds(13);
  }
}

void loop() {
  sendBurst();
  delay(PAUSE_DURATION);
}