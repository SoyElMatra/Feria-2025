// Motor A Derecha
#define IN1 A3
#define IN2 A4

// Motor B Izquierda
#define IN3 A2
#define IN4 A5

// Pines de sensores
#define SensorLeft 12
#define SensorMiddle 11  
#define SensorRight 10

int izq = 0;
int der = 0;
int cen = 0;

int velade = 170;
int velman = 170;
int stp = 50;
int stpsuave = 10;

// Variables para monitoreo periódico
unsigned long ultimoReporte = 0;
const unsigned long INTERVALO_REPORTE = 2000; // 2 segundos

void setup() {
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  
  Serial.begin(9600);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.println("=== SEGUIDOR DE LINEA ACTIVADO ===");
  Serial.println("1 = sobre línea negra, 0 = fuera de línea");
}

void loop() {
  // Leer sensores - INVERTIR la lógica para que 1 = línea negra
  izq = !digitalRead(SensorLeft);    // 1 = sobre línea
  der = !digitalRead(SensorRight);   // 1 = sobre línea
  cen = !digitalRead(SensorMiddle);  // 1 = sobre línea
  
  // LÓGICA DE SEGUIMIENTO INVERTIDA
  // 1 = sobre línea negra, 0 = fuera de línea
  
  if (izq == 1 && cen == 0 && der == 0) {
    left(stpsuave, velman);
  }
  else if (izq == 1 && cen == 1 && der == 0) {
    left(stpsuave, velman);
  }
  else if (izq == 0 && cen == 1 && der == 0) {
    forward(stp, velade);
  }
  else if (izq == 0 && cen == 0 && der == 1) {
    right(stpsuave, velman);
  }
  else if (izq == 0 && cen == 1 && der == 1) {
    right(stpsuave, velman);
  }
  else if (izq == 1 && cen == 1 && der == 1) {
    Parar();
  }
  else if (izq == 1 && cen == 0 && der == 1) {
    right(stpsuave, velman);
  }
  else if (izq == 0 && cen == 0 && der == 0) {
    right(stpsuave, velman);
  }
  else {
    forward(stp, velade);
  }
  
  // Mostrar estado de sensores solo cada 2 segundos
  if (millis() - ultimoReporte >= INTERVALO_REPORTE) {
    Serial.print("Sensores - I:");
    Serial.print(izq);
    Serial.print(" C:"); 
    Serial.print(cen);
    Serial.print(" D:");
    Serial.println(der);
    ultimoReporte = millis();
  }
  
  delay(50);
}

// FUNCIONES DE MOVIMIENTO (igual que antes)
void forward(int st, int vel) {
  // AMBOS motores ADELANTE
  digitalWrite(IN1, LOW);
  analogWrite(IN2, vel);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, vel);
  delay(st);
}

void right(int st, int vel) {
  // GIRO DERECHA: Motor A atrás, Motor B adelante
  analogWrite(IN1, vel);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, vel);
  delay(st);
}

void left(int st, int vel) {
  // GIRO IZQUIERDA: Motor A adelante, Motor B atrás
  digitalWrite(IN1, LOW);
  analogWrite(IN2, vel);
  analogWrite(IN3, vel);
  digitalWrite(IN4, LOW);
  delay(st);
}

void Parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(100);
}