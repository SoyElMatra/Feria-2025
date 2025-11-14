// Motor A Derecha
#define IN1 A3
#define IN2 A4
// Motor B Izquierda  
#define IN3 A2
#define IN4 A5

// Definiciones consistentes de sensores - USAR ESTOS PINES
#define SensorLeft 9
#define SensorMiddle 7
#define SensorRight 8

int izq = 0;
int der = 0;
int cen = 0;

int velade = 170;
int velman = 170;
int stp = 50;
int stpsuave = 10;

bool bw, le, ri, fw;
const int TIMEOUT = 75;
const int SENSOR_COUNT = 3;

struct Sensor {
  char name;
  unsigned int pin;
  bool lastState;
  bool signalPresent;
  unsigned long lastChangeTime;
};

int lstat = 0;

Sensor sensors[SENSOR_COUNT] = {
  { 'l', SensorLeft, HIGH, false, 0 },
  { 'm', SensorMiddle, HIGH, false, 0 },
  { 'r', SensorRight, HIGH, false, 0 }
};

void setup() {
  // Configurar sensores de línea
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  
  Serial.begin(9600);
  
  // Configurar pines de motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Inicializar sensores IR
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensors[i].pin, INPUT_PULLUP);
    sensors[i].lastState = digitalRead(sensors[i].pin);
  }
  
  Serial.println("=== SISTEMA INICIALIZADO ===");
  Serial.println("Testeando sensores de linea (1=linea negra, 0=fuera de linea):");
  delay(2000);
}

void loop() {
  // *** CAMBIO PRINCIPAL: LECTURAS INVERTIDAS ***
  // Ahora 1 = sensor sobre línea negra, 0 = fuera de línea
  izq = !digitalRead(SensorLeft);
  der = !digitalRead(SensorRight);
  cen = !digitalRead(SensorMiddle);
  
  checkPos();

  // DEBUG: Mostrar valores de sensores
  Serial.print("LINEA - I:");
  Serial.print(izq);
  Serial.print(" C:"); 
  Serial.print(cen);
  Serial.print(" D:");
  Serial.print(der);
  Serial.print(" | IR - le:");
  Serial.print(le);
  Serial.print(" fw:");
  Serial.print(fw);
  Serial.print(" ri:");
  Serial.println(ri);

  // *** LÓGICA DE SEGUIMIENTO DE LÍNEA CORREGIDA ***
  if (fw == 1 || ri == 1 || le == 1) {
    // Caso 1: Solo sensor izquierdo en línea
    if ((izq) && (!cen) && (!der)) {
      left(stpsuave, velman);
    } 
    // Caso 2: Izquierda y centro en línea
    else if ((izq) && (cen) && (!der)) {
      left(stpsuave, velman);
    } 
    // Caso 3: Solo centro en línea → AVANZAR
    else if ((!izq) && (cen) && (!der)) {
      forward(stp, velade);
    } 
    // Caso 4: Solo derecha en línea
    else if ((!izq) && (!cen) && (der)) {
      right(stpsuave, velman);
    } 
    // Caso 5: Centro y derecha en línea
    else if ((!izq) && (cen) && (der)) {
      right(stpsuave, velman);
    } 
    // *** CAMBIO: Condición corregida con OR (||) en lugar de AND (&&) ***
    // Caso 6: Izquierda y derecha en línea O todos fuera de línea
    else if (((izq) && (!cen) && (der)) || ((!izq) && (!cen) && (!der))) {
      if (ri == 1 || (lstat == 1 && bw == 1)) {
        right(stpsuave, velman);
      } else if (le == 1 || (lstat == 2 && bw == 1)) {
        left(stpsuave, velman);
      } else {
        // Por defecto girar derecha
        right(stpsuave, velman);
      }
    } 
    // Caso 7: Todos los sensores en línea
    else if ((izq) && (cen) && (der)) {
      Parar();
    }
    // Caso 8: Ninguno de los casos anteriores
    else {
      Parar();
    }
  } else {
    // Si no hay señal de los sensores IR, parar
    Parar();
  }
}

void checkPos() {
  unsigned long currentTime = millis();

  for (int i = 0; i < SENSOR_COUNT; i++) {
    Sensor& s = sensors[i];
    bool currentState = digitalRead(s.pin);

    if (currentState != s.lastState) {
      s.lastChangeTime = currentTime;
      s.lastState = currentState;
    }
    bool newState = (currentTime - s.lastChangeTime) <= TIMEOUT;

    if (newState != s.signalPresent) {
      s.signalPresent = newState;
      if (s.signalPresent == 1) {
        Serial.print("SENSOR IR ACTIVO: ");
        Serial.println(s.name);
        switch (s.name) {
          case 'l':
            le = 1;
            ri = 0;
            fw = 0;
            bw = 0;
            lstat = 2; // 2 = izquierda
            break;
          case 'm':
            fw = 1;
            ri = 0;
            le = 0;
            bw = 0;
            lstat = 0; // 0 = adelante
            break;
          case 'r':
            ri = 1;
            le = 0;
            fw = 0;
            bw = 0;
            lstat = 1; // 1 = derecha
            break;
          default:
            ri = 0;
            le = 0;
            fw = 0;
            bw = 1;
            break;
        }
      }
      if (!s.signalPresent) {
        Serial.print("SENSOR IR INACTIVO: ");
        Serial.println(s.name);
        switch (s.name) {
          case 'l':
            le = 0;
            break;
          case 'm':
            fw = 0;
            break;
          case 'r':
            ri = 0;
            break;
        }
      }
    }
  }
}

void forward(int st, int vel) {
  analogWrite(IN2, vel);
  digitalWrite(IN1, 0);
  analogWrite(IN3, vel);
  digitalWrite(IN4, 0);
  delay(st);
}

void right(int st, int vel) {
  analogWrite(IN1, vel);
  digitalWrite(IN2, 0);
  analogWrite(IN3, vel);
  digitalWrite(IN4, 0);
  delay(st);
}

void left(int st, int vel) {
  analogWrite(IN2, vel);
  digitalWrite(IN1, 0);
  analogWrite(IN4, vel);
  digitalWrite(IN3, 0);
  delay(st);
}

void Parar() {
  digitalWrite(IN2, 0);
  digitalWrite(IN1, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
}