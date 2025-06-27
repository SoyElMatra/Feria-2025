// Motor A Derecba
#define IN1 2
#define IN2 3

// Motor B Izquierda
#define IN3 4
#define IN4 5

int izq = 0;
int der = 0;
int cen = 0;

int velade = 255;
int velman = 255;
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

bool check;

Sensor sensors[SENSOR_COUNT] = {
  { 'l', 7, HIGH, false, 0 },
  { 'm', 8, HIGH, false, 0 },
  { 'r', 9, HIGH, false, 0 }
};

// Guardar el número de pin de Arduino al que está conectado el pin OUT del sensor
#define SensorLeft 10    // input pin of left sensor
#define SensorMiddle 11  // input pin of middle sensor
#define SensorRight 12   // input pin of right sensor

void setup() {
  // Se indica que ese pin va a utilizarse para recibir información
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  // Se activa el monitor serie para mostrar información posteriormente
  Serial.begin(9600);
  // Definimos todos los pines de los motores como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensors[i].pin, INPUT_PULLUP);
    sensors[i].lastState = digitalRead(sensors[i].pin);
  }
}



void loop() {
  // Se lee la información ofrecida por el sensor
  izq = digitalRead(SensorLeft);
  der = digitalRead(SensorRight);
  cen = digitalRead(SensorMiddle);
  checkPos();
  // Se fija si el pacman esta atras suyo para volver
  if (bw == 1) {
    Return(stpsuave);
  }
  // Sigue un algoritmo para ver que hace dependiendo de la direccion del pacman
  else if (fw == 1 or ri == 1 or le == 1) {
    if ((!izq) and (cen) and (der)) {
      left(stpsuave, velman);
    } else if ((!izq) and (!cen) and (der)) {
      left(stpsuave, velman);
    } else if ((izq) and (!cen) and (der)) {
      forward(stp, velade);
    } else if ((izq) and (!cen) and (!der)) {
      right(stpsuave, velman);
    } else if ((izq) and (cen) and (!der)) {
      right(stpsuave, velman);
    } else if (((!izq) and (cen) and (!der)) and ((!izq) and (!cen) and (!der))) {
      if (ri == 1) {
        right(stpsuave, velman);
      } else if (le == 1) {
        left(stpsuave, velman);
      }
    } else if ((izq) and (cen) and (der)) {
      Parar();
    }
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
        Serial.println(s.name);
        switch (s.name) {
          case 'l':
            le = 1;
            fw = 0;
            ri = 0;
            bw = 0;
            break;
          case 'm':
            le = 0;
            fw = 1;
            ri = 0;
            bw = 0;
            break;
          case 'r':
            le = 0;
            fw = 0;
            ri = 1;
            bw = 0;
            break;
          default:
            le = 0;
            fw = 0;
            ri = 0;
            bw = 1;
            break;
        }
      }
    }
  }
}



void forward(int st, int vel) {
  if (check == 1) {
    check = 0;
    Serial.println("ADELANTE");
  }
  analogWrite(IN2, vel);
  digitalWrite(IN1, 0);
  analogWrite(IN4, vel);
  digitalWrite(IN3, 0);
  delay(st);
}



void right(int st, int vel) {
  if (check == 1) {
    check = 0;
    Serial.println("DERECHA");
  }
  analogWrite(IN1, vel);
  digitalWrite(IN2, 0);
  analogWrite(IN4, vel);
  digitalWrite(IN3, 0);
  delay(st);
}



void left(int st, int vel) {
  if (check == 1) {
    check = 0;
    Serial.println("IZQUIERDA");
  }
  analogWrite(IN2, vel);
  digitalWrite(IN1, 0);
  analogWrite(IN3, vel);
  digitalWrite(IN4, 0);
  delay(st);
}



void Parar() {
  check = 1;
  digitalWrite(IN2, 0);
  digitalWrite(IN1, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
}



void Return(int st) {
  if (check == 1) {
    check = 0;
    Serial.println("ATRAS");
  }
  while (bw == 1 or (cen)) {
    if (ri == 1) {
      right(st, 255);
    } else if (le == 1) {
      left(st, 255);
    } else {
      right(st, 255);
    }
  }
}