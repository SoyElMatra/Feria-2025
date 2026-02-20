// ----- Pines de sensores (entrada) -----
const int PIN_SENSOR_IZQ = 6;   // Sensor izquierdo (pin PWM usado como digital)
const int PIN_SENSOR_CEN = 10;  // Sensor central   (pin PWM usado como digital)
const int PIN_SENSOR_DER = 12;  // Sensor derecho   (pin digital)

// ----- Pines de control de motores (salida) -----
const int PIN_MOTOR_IZQ_PWM = 3;  // Velocidad motor izquierdo (PWM)
const int PIN_MOTOR_DER_PWM = 5;  // Velocidad motor derecho   (PWM)

const int PIN_MOTOR_IZQ_RETRO = A4;   // IN1 - Marcha atrás izquierdo
const int PIN_MOTOR_IZQ_AVANCE = A3;  // IN2 - Avance izquierdo
const int PIN_MOTOR_DER_RETRO = A2;   // IN3 - Marcha atrás derecho
const int PIN_MOTOR_DER_AVANCE = A1;  // IN4 - Avance derecho

// ----- Control de seguimiento del Pacman -----
const int TIMEOUT = 75;      // Tiempo de espera en milisegundos
const int SENSOR_COUNT = 3;  // Número de sensores

struct Sensor {
  const char* name;              // Nombre del sensor
  unsigned int pin;              // Pin del sensor
  bool lastState;                // Último estado del sensor
  bool signalPresent;            // Estado actual del sensor
  unsigned long lastChangeTime;  // Última vez que cambió el estado
};

Sensor sensors[SENSOR_COUNT] = {
  { "Izquierda", A5, HIGH, false, 0 },
  { "Centro", A0, HIGH, false, 0 },
  { "Derecha", 2, HIGH, false, 0 }
};

// ----- Variables globales -----
unsigned char valorSensorIzq;  // LOW = negro, HIGH = blanco
unsigned char valorSensorCen;
unsigned char valorSensorDer;
const int t = 500;
bool brusco = 0;

unsigned char velocidadIzq = 150;  // Velocidad base motor izquierdo (no se usa directamente)
unsigned char velocidadDer = 150;  // Velocidad base motor derecho
int estadoCoche = 0;               // Variable de estado (no utilizada en la lógica principal)

// Última dirección indicada por el Pacman (1=izquierda, 2=derecha)
int ultimaDireccionPacman = 2;  // Por defecto derecha

void configurarSensores() {
  pinMode(PIN_SENSOR_IZQ, INPUT);
  pinMode(PIN_SENSOR_CEN, INPUT);
  pinMode(PIN_SENSOR_DER, INPUT);
}

void configurarMotores() {
  pinMode(PIN_MOTOR_IZQ_RETRO, OUTPUT);
  pinMode(PIN_MOTOR_IZQ_AVANCE, OUTPUT);
  pinMode(PIN_MOTOR_DER_RETRO, OUTPUT);
  pinMode(PIN_MOTOR_DER_AVANCE, OUTPUT);
  pinMode(PIN_MOTOR_IZQ_PWM, OUTPUT);
  pinMode(PIN_MOTOR_DER_PWM, OUTPUT);
}

void fijarVelocidad(unsigned char izquierda, unsigned char derecha) {
  analogWrite(PIN_MOTOR_IZQ_PWM, izquierda);
  analogWrite(PIN_MOTOR_DER_PWM, derecha);
}

void configurarPWM() {
  TCCR2B = 0;
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);

  TCCR0B = 0;
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
  TCCR0B = _BV(CS02) | _BV(CS00);
}

void setup() {
  Serial.begin(9600);
  configurarPWM();
  configurarSensores();
  configurarMotores();
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensors[i].pin, INPUT);                      // Configurar el pin del sensor como entrada
    sensors[i].lastState = digitalRead(sensors[i].pin);  // Leer el estado inicial
    fijarVelocidad(velocidadIzq, velocidadDer);
    detener();  // Estado inicial seguro
  }
}

void loop() {
  leerSensoresPac();
  leerSensoresSeg();
}

void leerSensoresSeg() {
  valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
  valorSensorCen = digitalRead(PIN_SENSOR_CEN);
  valorSensorDer = digitalRead(PIN_SENSOR_DER);
  
  // Detectar bifurcación: ambos sensores laterales en HIGH (sin importar el central)
  if (valorSensorIzq == HIGH && valorSensorDer == HIGH) {
    bifurcacion();
  }
  else if (valorSensorCen == HIGH) {
    if (valorSensorIzq == HIGH) {
      estadoCoche = 2;
      girar();
    } else if (valorSensorDer == HIGH) {
      estadoCoche = 1;
      girar();
    } else {
      avanzar();
    }
  } else {
    if (valorSensorIzq == HIGH) {
      estadoCoche = 2;
      girar();
    } else if (valorSensorDer == HIGH) {
      estadoCoche = 1;
      girar();
    } else {
      // Todos los sensores en LOW: pérdida de línea
      buscarLinea();
    }
  }
}

void leerSensoresPac() {
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

      Serial.print(s.name);
      Serial.print(": ");
      Serial.println(s.signalPresent ? "1" : "0");
    }
  }
}

void girar() {
  if (brusco == 1) {
    if (estadoCoche == 1) {
      giroIzqBrusco();
      while (valorSensorIzq == LOW) {
        valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
        valorSensorCen = digitalRead(PIN_SENSOR_CEN);
        valorSensorDer = digitalRead(PIN_SENSOR_DER);
        if (valorSensorCen == HIGH || valorSensorDer == HIGH) {
          break;
        }
      }
    } else if (estadoCoche == 2) {
      giroDerBrusco();
      while (valorSensorDer == LOW) {
        valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
        valorSensorCen = digitalRead(PIN_SENSOR_CEN);
        valorSensorDer = digitalRead(PIN_SENSOR_DER);
        if (valorSensorCen == HIGH || valorSensorIzq == HIGH) {
          break;
        }
      }
    }
  } else {
    if (estadoCoche == 1) {
      giroIzq();
    } else if (estadoCoche == 2) {
      giroDer();
    }
  }
}

void avanzar() {
  fijarVelocidad(100, 100);  // Velocidad normal reducida a 100
  digitalWrite(PIN_MOTOR_DER_RETRO, LOW);
  digitalWrite(PIN_MOTOR_DER_AVANCE, HIGH);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, LOW);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, HIGH);
}

void giroDer() {
  fijarVelocidad(150, 150);  // Velocidad de giro a 150
  digitalWrite(PIN_MOTOR_DER_RETRO, LOW);
  digitalWrite(PIN_MOTOR_DER_AVANCE, LOW);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, LOW);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, HIGH);
}

void giroDerBrusco() {
  fijarVelocidad(150, 150);  // Velocidad de giro a 150
  digitalWrite(PIN_MOTOR_DER_RETRO, HIGH);
  digitalWrite(PIN_MOTOR_DER_AVANCE, LOW);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, LOW);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, HIGH);
}

void giroIzq() {
  fijarVelocidad(150, 150);  // Velocidad de giro a 150
  digitalWrite(PIN_MOTOR_DER_RETRO, LOW);
  digitalWrite(PIN_MOTOR_DER_AVANCE, HIGH);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, LOW);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, LOW);
}

void giroIzqBrusco() {
  fijarVelocidad(150, 150);  // Velocidad de giro a 150
  digitalWrite(PIN_MOTOR_DER_RETRO, LOW);
  digitalWrite(PIN_MOTOR_DER_AVANCE, HIGH);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, HIGH);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, LOW);
}

void detener() {
  digitalWrite(PIN_MOTOR_DER_RETRO, HIGH);
  digitalWrite(PIN_MOTOR_DER_AVANCE, HIGH);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, HIGH);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, HIGH);
}

// Función para manejar bifurcaciones
void bifurcacion() {
  // Leer sensores Pacman
  bool pacIzq = sensors[0].signalPresent;
  bool pacDer = sensors[2].signalPresent;

  // Guardar dirección
  if (pacDer) {
    ultimaDireccionPacman = 2;
    giroDerBrusco();
  } else {
    ultimaDireccionPacman = 1;
    giroIzqBrusco();
  }

  // Esperar hasta que el sensor central detecte la línea (o timeout)
  unsigned long inicio = millis();
  while (true) {
    valorSensorCen = digitalRead(PIN_SENSOR_CEN);
    if (valorSensorCen == HIGH) {
      break;
    }
    if (millis() - inicio > 1000) {
      break;
    }
    leerSensoresPac();
    delay(10);
  }

  // Avanzar hasta salir de la línea (que el central se ponga LOW)
  inicio = millis();
  while (true) {
    avanzar();  // Mantener avance (ahora a 100)
    valorSensorCen = digitalRead(PIN_SENSOR_CEN);
    if (valorSensorCen == LOW) {
      break;
    }
    if (millis() - inicio > 500) { // Timeout para no quedarse atascado
      break;
    }
    leerSensoresPac();
    delay(10);
  }

  detener();  // Detener para que el loop normal retome el control
}

// Función para buscar la línea cuando se pierde
void buscarLinea() {
  unsigned long inicio = millis();
  while (true) {
    // Leer sensores de línea
    valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
    valorSensorCen = digitalRead(PIN_SENSOR_CEN);
    valorSensorDer = digitalRead(PIN_SENSOR_DER);

    // Si encontramos línea, salir
    if (valorSensorIzq == HIGH || valorSensorCen == HIGH || valorSensorDer == HIGH) {
      return;
    }

    // Timeout de seguridad (1 segundo)
    if (millis() - inicio > 1000) {
      return;
    }

    // Girar en la última dirección del Pacman (usa giros bruscos con velocidad 150)
    if (ultimaDireccionPacman == 1) {
      giroIzqBrusco();
    } else {
      giroDerBrusco();
    }

    delay(10);
    leerSensoresPac();
  }
}