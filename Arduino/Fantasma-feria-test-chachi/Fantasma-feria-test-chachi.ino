// Motor A Derecha
#define IN1 A3
#define IN2 A4

// Motor B Izquierda
#define IN3 A5
#define IN4 A2

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
  char name;                    // id del sensor
  unsigned int pin;             // pin del sensor
  bool lastState;               // ultimo estado del sensor
  bool signalPresent;           // presencia de la señal
  unsigned long lastChangeTime; // tiempo de cambio de señal del sensor
};

int lstat = 0;

bool check;

Sensor sensors[SENSOR_COUNT] = {
  { 'l', 9, HIGH, false, 0 },
  { 'm', 7, HIGH, false, 0 },
  { 'r', 8, HIGH, false, 0 }
};

#define SensorLeft 10
#define SensorMiddle 11
#define SensorRight 12

void setup() {
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  Serial.begin(9600);
  
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
  // Leer sensores de línea
  izq = digitalRead(SensorLeft);
  der = digitalRead(SensorRight);
  cen = digitalRead(SensorMiddle);
  checkPos();
  
  Serial.print("Linea - I:");
  Serial.print(izq);
  Serial.print(" C:");
  Serial.print(cen);
  Serial.print(" D:");
  Serial.println(der);

  // SEGUIMIENTO DE LÍNEA PRINCIPAL - funciona siempre, con o sin TSOP
  if (izq && !cen && !der) {
    // Solo sensor izquierdo activo - girar izquierda
    left(stpsuave, velman);
    //Serial.println("GIRO IZQ: Solo sensor izquierdo");
  } else if (!izq && !cen && der) {
    // Solo sensor derecho activo - girar derecha
    right(stpsuave, velman);
    //Serial.println("GIRO DER: Solo sensor derecho");
  } else if (izq && cen && !der) {
    // Izquierdo y centro activos - girar suave izquierda
    left(stpsuave, velman);
    //Serial.println("GIRO SUAVE IZQ: Izq + Centro");
  } else if (!izq && cen && der) {
    // Derecho y centro activos - girar suave derecha
    right(stpsuave, velman);
    //Serial.println("GIRO SUAVE DER: Der + Centro");
  } else if (!izq && cen && !der) {
    // Solo centro activo - seguir adelante
    forward(stp, velade);
    //Serial.println("ADELANTE: Solo centro");
  } 
  // BIFURCACIONES - usar TSOP para decidir
  else if (izq && der && cen) {
    // Los 3 sensores activos - bifurcación completa
    //Serial.println("BIFURCACION: 3 sensores activos");
    manejarBifurcacion();
  } else if (izq && !cen && der) {
    // Solo laterales activos - bifurcación en T
    //Serial.println("BIFURCACION: Solo laterales activos");
    manejarBifurcacion();
  } else if (!izq && !cen && !der) {
    // Ningún sensor activo - parar
    Parar();
    //Serial.println("PARAR: Ningun sensor activo");
  } else {
    // Cualquier otro caso - seguir adelante por defecto
    forward(stp, velade);
    //Serial.println("ADELANTE: Caso por defecto");
  }
}

void manejarBifurcacion() {
  // Solo usar TSOP si hay señal disponible
  if (ri == 1 || lstat == 1) {
    //Serial.println("Girando DERECHA en bifurcacion por TSOP");
    right(stpsuave * 3, velman);
  } else if (le == 1 || lstat == 2) {
    //Serial.println("Girando IZQUIERDA en bifurcacion por TSOP");
    left(stpsuave * 3, velman);
  } else {
    // Sin señal TSOP - comportamiento por defecto en bifurcación
    //Serial.println("Bifurcacion sin TSOP - Adelante por defecto");
    forward(stp, velade);
  }
}

void checkPos() {
  unsigned long currentTime = millis();
  bool anySensorActive = false;

  // Actualizar estado de todos los sensores TSOP
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
    }
  }

  // Leer estados actuales de TSOP
  bool leftActive = sensors[0].signalPresent;
  bool middleActive = sensors[1].signalPresent;
  bool rightActive = sensors[2].signalPresent;

  // Reiniciar variables
  le = 0;
  ri = 0;
  fw = 0;
  bw = 0;

  // Verificar si hay algún sensor TSOP activo
  anySensorActive = leftActive || middleActive || rightActive;

  // Si no hay sensores TSOP activos, activar bw
  if (!anySensorActive) {
    bw = 1;
    return;
  }

  // Asignar direcciones basadas en los sensores TSOP activos
  if (leftActive) {
    le = 1;
    lstat = 2;
  }
  if (rightActive) {
    ri = 1;
    lstat = 1;
  }
  if (middleActive) {
    fw = 1;
    lstat = 0;
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