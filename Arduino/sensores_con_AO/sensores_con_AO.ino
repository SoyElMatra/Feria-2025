// SENSORES EN PINES ANALÓGICOS
#define SensorLeft A0
#define SensorMiddle A1  
#define SensorRight A2

// Umbral para 0/1
int umbral = 600;  // Mayor a 700 = 1, Menor a 700 = 0

// Variables digitales
int L, C, R;

void setup() {
  Serial.begin(9600);
  Serial.println("=== SENSORES DIGITAL (0/1) ===");
  Serial.println("Umbral: >700 = 1, <700 = 0");
  Serial.println("Formato: [L C R]");
  Serial.println("----------------");
}

void loop() {
  // Leer sensores analógicos y convertir a digital
  L = (analogRead(SensorLeft) > umbral) ? 1 : 0;
  C = (analogRead(SensorMiddle) > umbral) ? 1 : 0;
  R = (analogRead(SensorRight) > umbral) ? 1 : 0;
  
  // Mostrar en formato [0 1 0]
  Serial.println("[" + String(L) + " " + String(C) + " " + String(R) + "]");
  
  delay(300);
}