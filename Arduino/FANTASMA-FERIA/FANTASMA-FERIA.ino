// =======================
// Pines del Arduino
// =======================

// Motor derecho
#define IN1 A3
#define IN2 A4

// Motor izquierdo
#define IN3 A5
#define IN4 A2

// Sensores de línea
#define SensorLeft    12
#define SensorMiddle  11
#define SensorRight   10

// =======================
// Variables
// =======================
bool izq = false;
bool cen = false;
bool der = false;

int velade = 190;    // Velocidad de avance (no se usa digital ON/OFF, queda para compatibilidad)
int velman = 100;    // Velocidad de giro
int stp = 50;       // tiempo que dura la accion
int stpsuave = 10;  // en caso de estar medio complicado con alguna curva

// =======================
// Setup
// =======================
void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);

  Parar();
}

// =======================
// Loop
// =======================
void loop() {
  // Leer sensores (invertimos porque el sensor LOW = línea negra)
  izq = digitalRead(SensorLeft);
  cen = digitalRead(SensorMiddle);
  der = digitalRead(SensorRight);

  // Lógica de seguimiento de línea
  if (izq && cen && der) {
    Parar(); // Ningún sensor sobre línea
  } 
  else if (izq && !cen && !der) {
    Izquierda(stpsuave); // Solo derecha y centro sobre línea
  } 
  else if (izq && cen && !der) {
    Izquierda(stpsuave); // Solo derecha sobre línea
  } 
  else if (!izq && cen && !der) {
    Adelante(stp); // Izquierda y derecha sobre línea, centro perdido
  } 
  else if (!izq && cen && der) {
    Derecha(stpsuave); // Solo izquierda sobre línea
  } 
  else if (!izq && !cen && der) {
    Derecha(stpsuave); // Izquierda y centro sobre línea
  } 
  else if (!izq && !cen && !der) {
    Parar(); // Línea detectada en todos
  }
}

// =======================
// Funciones de movimiento
// =======================
void Adelante(int st) {
  digitalWrite(IN1, HIGH);  // Motor derecho adelante
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Motor izquierdo adelante
  digitalWrite(IN4, LOW);
  delay(st);
  Parar();
}

void Atras(int st) {
  digitalWrite(IN1, LOW);   // Motor derecho atrás
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);   // Motor izquierdo atrás
  digitalWrite(IN4, HIGH);
  delay(st);
  Parar();
}

void Derecha(int st) {
  digitalWrite(IN1, HIGH);  // Motor derecho adelante
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Motor izquierdo atrás
  digitalWrite(IN4, HIGH);
  delay(st);
  Parar();
}

void Izquierda(int st) {
  digitalWrite(IN1, LOW);   // Motor derecho atrás
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);  // Motor izquierdo adelante
  digitalWrite(IN4, LOW);
  delay(st);
  Parar();
}

void Parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
