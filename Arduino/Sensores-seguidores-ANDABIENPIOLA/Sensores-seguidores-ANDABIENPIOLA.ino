// ----- Pines de sensores (entrada) -----
const int PIN_SENSOR_IZQ = 8;   // Sensor izquierdo (pin PWM usado como digital)
const int PIN_SENSOR_CEN = 9;  // Sensor central   (pin PWM usado como digital)
const int PIN_SENSOR_DER = 10;  // Sensor derecho   (pin digital)

// ----- Pines de control de motores (salida) -----
const int PIN_MOTOR_IZQ_PWM = 6;  // Velocidad motor izquierdo (PWM)
const int PIN_MOTOR_DER_PWM = 7;  // Velocidad motor derecho   (PWM)

const int PIN_MOTOR_IZQ_RETRO = A4;   // IN1 - Marcha atrás izquierdo
const int PIN_MOTOR_IZQ_AVANCE = A3;  // IN2 - Avance izquierdo
const int PIN_MOTOR_DER_RETRO = A2;   // IN3 - Marcha atrás derecho
const int PIN_MOTOR_DER_AVANCE = A1;  // IN4 - Avance derecho

// ----- Variables globales -----
unsigned char valorSensorIzq;  // LOW = negro, HIGH = blanco
unsigned char valorSensorCen;
unsigned char valorSensorDer;
const int t = 25;
bool brusco = 0;

unsigned char velocidadIzq = 125;  // Velocidad base motor izquierdo
unsigned char velocidadDer = 125;  // Velocidad base motor derecho
int estadoCoche = 0;               // Variable de estado (no utilizada en la lógica principal)

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
  fijarVelocidad(velocidadIzq, velocidadDer);
  detener();  // Estado inicial seguro
}

void loop() {
  leerSensores();
}

void leerSensores() {
  valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
  valorSensorCen = digitalRead(PIN_SENSOR_CEN);
  valorSensorDer = digitalRead(PIN_SENSOR_DER);
  Serial.println(brusco);
  if (valorSensorCen == HIGH) {
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
      //} else if (valorSensorIzq == LOW and valorSensorDer == LOW) {
      //bifurcacion();
    } else {
      brusco = 1;
      girar();
      brusco = 0;
    }
  }
}

void girar() {
  if (brusco == 1) {
    if (estadoCoche == 1) {
      Serial.println("IzqBrusco");
      giroIzqBrusco();
      while (valorSensorIzq == LOW) {
        valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
        valorSensorCen = digitalRead(PIN_SENSOR_CEN);
        valorSensorDer = digitalRead(PIN_SENSOR_DER);
        if (valorSensorCen == HIGH or valorSensorDer == HIGH) {
          break;
        }
      }
    }
    if (estadoCoche == 2) {
      Serial.println("DerBrusco");
      giroDerBrusco();
      while (valorSensorDer == LOW) {
        valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
        valorSensorCen = digitalRead(PIN_SENSOR_CEN);
        valorSensorDer = digitalRead(PIN_SENSOR_DER);
        if (valorSensorCen == HIGH or valorSensorIzq == HIGH) {
          break;
        }
      }
    }

  } else {
    if (estadoCoche == 1) {
      Serial.println("Izq");
      giroIzq();
      /*while (valorSensorIzq == LOW) {
        valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
        valorSensorCen = digitalRead(PIN_SENSOR_CEN);
        valorSensorDer = digitalRead(PIN_SENSOR_DER);
        if (valorSensorCen == HIGH or valorSensorDer == HIGH) {
          break;
        }
      }*/
    } else if (estadoCoche == 2) {
      Serial.println("Der");
      giroDer();
      /*while (valorSensorDer == LOW) {
        valorSensorIzq = digitalRead(PIN_SENSOR_IZQ);
        valorSensorCen = digitalRead(PIN_SENSOR_CEN);
        valorSensorDer = digitalRead(PIN_SENSOR_DER);
        if (valorSensorCen == HIGH or valorSensorIzq == HIGH) {
          break;
        }
      }*/
    }
  }
}



void avanzar() {
  fijarVelocidad(velocidadIzq, velocidadDer);
  digitalWrite(PIN_MOTOR_DER_RETRO, LOW);
  digitalWrite(PIN_MOTOR_DER_AVANCE, HIGH);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, LOW);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, HIGH);
}



void giroDer() {
  fijarVelocidad(velocidadIzq + t, velocidadDer);
  digitalWrite(PIN_MOTOR_DER_RETRO, LOW);
  digitalWrite(PIN_MOTOR_DER_AVANCE, LOW);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, LOW);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, HIGH);
}



void giroDerBrusco() {
  fijarVelocidad(velocidadIzq + t, velocidadDer);
  digitalWrite(PIN_MOTOR_DER_RETRO, HIGH);
  digitalWrite(PIN_MOTOR_DER_AVANCE, LOW);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, LOW);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, HIGH);
}



void giroIzq() {
  fijarVelocidad(velocidadIzq, velocidadDer + t);
  digitalWrite(PIN_MOTOR_DER_RETRO, LOW);
  digitalWrite(PIN_MOTOR_DER_AVANCE, HIGH);
  digitalWrite(PIN_MOTOR_IZQ_RETRO, LOW);
  digitalWrite(PIN_MOTOR_IZQ_AVANCE, LOW);
}



void giroIzqBrusco() {
  fijarVelocidad(velocidadIzq, velocidadDer + t);
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