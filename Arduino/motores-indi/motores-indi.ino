// Pines de motores
#define IN1 A3
#define IN2 A4
#define IN3 A2
#define IN4 A5

void setup() {
  Serial.begin(9600);
  
  // Configurar pines de motores como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.println("=== PRUEBA INDIVIDUAL DE MOTORES ===");
  Serial.println("Probaremos cada motor y cada pin por separado");
  delay(2000);
}

void loop() {
  Serial.println("\n--- Probando Motor A (Pines A3 y A4) ---");
  pruebaMotorA();
  delay(3000);
  
  Serial.println("\n--- Probando Motor B (Pines A2 y A5) ---");
  pruebaMotorB();
  delay(3000);
  
  Serial.println("\n--- Probando Ambos Motores ---");
  pruebaAmbosMotores();
  delay(3000);
}

void pruebaMotorA() {
  Serial.println("Motor A - Adelante");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  // Motor A adelante
  delay(2000);
  
  Serial.println("Motor A - Atrás");
  digitalWrite(IN1, HIGH);  // Motor A atrás
  digitalWrite(IN2, LOW);
  delay(2000);
  
  Serial.println("Motor A - Parar");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(1000);
}

void pruebaMotorB() {
  Serial.println("Motor B - Adelante");
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Motor B adelante
  delay(2000);
  
  Serial.println("Motor B - Atrás");
  digitalWrite(IN3, HIGH);  // Motor B atrás
  digitalWrite(IN4, LOW);
  delay(2000);
  
  Serial.println("Motor B - Parar");
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(1000);
}

void pruebaAmbosMotores() {
  Serial.println("Ambos motores adelante");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(2000);
  
  Serial.println("Ambos motores parados");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(1000);
}
