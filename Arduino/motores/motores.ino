// Pines de motores
#define IN1 A3
#define IN2 A4
#define IN3 A2
#define IN4 A5

// Pines de sensores de línea
#define SensorLeft 9
#define SensorMiddle 7
#define SensorRight 8

// Variables de sensores
int izq, der, cen;

void setup() {
  // Configurar pines de motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Configurar sensores
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  
  Serial.begin(9600);
  Serial.println("=== MODO PRUEBA INICIADO ===");
  Serial.println("Comandos: 1=Adelante, 2=Atrás, 3=Izquierda, 4=Derecha, 5=Parar");
  Serial.println("Se mostraran lecturas de sensores automaticamente");
}

void loop() {
  // Leer y mostrar sensores continuamente
  leerSensores();
  
  // Control por comandos seriales
  if (Serial.available()) {
    char comando = Serial.read();
    ejecutarComando(comando);
  }
  
  delay(100);
}

void leerSensores() {
  // Leer sensores (invertidos porque 0 = línea negra)
  izq = !digitalRead(SensorLeft);
  der = !digitalRead(SensorRight);
  cen = !digitalRead(SensorMiddle);
  
 
}

void ejecutarComando(char cmd) {
  switch(cmd) {
    case '1': // Adelante
      Serial.println(">> ADELANTE");
      adelante(1000, 250);
      break;
    case '2': // Atrás
      Serial.println(">> ATRAS");
      atras(1000, 250);
      break;
    case '3': // Izquierda
      Serial.println(">> IZQUIERDA");
      izquierda(1000, 250);
      break;
    case '4': // Derecha
      Serial.println(">> DERECHA");
      derecha(1000, 250);
      break;
    case '5': // Parar
      Serial.println(">> PARAR");
      parar();
      break;
    case 't': // Test automático
      Serial.println(">> TEST AUTOMATICO");
      testAutomatico();
      break;
  }
}

// Funciones de movimiento
void adelante(int tiempo, int velocidad) {
  analogWrite(IN2, velocidad);
  digitalWrite(IN1, 0);
  analogWrite(IN3, velocidad);
  digitalWrite(IN4, 0);
  delay(tiempo);
  parar();
}

void atras(int tiempo, int velocidad) {
  analogWrite(IN1, velocidad);
  digitalWrite(IN2, 0);
  analogWrite(IN4, velocidad);
  digitalWrite(IN3, 0);
  delay(tiempo);
  parar();
}

void izquierda(int tiempo, int velocidad) {
  analogWrite(IN2, velocidad);
  digitalWrite(IN1, 0);
  analogWrite(IN4, velocidad);
  digitalWrite(IN3, 0);
  delay(tiempo);
  parar();
}

void derecha(int tiempo, int velocidad) {
  analogWrite(IN1, velocidad);
  digitalWrite(IN2, 0);
  analogWrite(IN3, velocidad);
  digitalWrite(IN4, 0);
  delay(tiempo);
  parar();
}

void parar() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
}

void testAutomatico() {
  Serial.println("Iniciando test automatico...");
  
  adelante(1000, 150);
  delay(500);
  
  atras(1000, 150);
  delay(500);
  
  izquierda(1000, 150);
  delay(500);
  
  derecha(1000, 150);
  delay(500);
  
  parar();
  Serial.println("Test automatico completado");
}