//sensores ubicacion pacman
#define adelante 9
#define izquierda 8
#define derecha 7
#define atras 6

// Motor A Derecba
#define ENA A0
#define IN1 2
#define IN2 3

// Motor B Izquierda
#define ENB A1
#define IN3 4
#define IN4 5

int fw;
int bw;
int ri;
int le;

int izq = 0;
int der = 0;
int cen = 0;

int velade = 255;
int velman = 255;
int stp = 50;
int stpsuave = 10;

// Guardar el número de pin de Arduino al que está conectado el pin OUT del sensor
#define SensorLeft 10    // input pin of left sensor
#define SensorMiddle 11  // input pin of middle sensor
#define SensorRight 12  // input pin of right sensor

void setup() {
  // Se indica que ese pin va a utilizarse para recibir información
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  // Se activa el monitor serie para mostrar información posteriormente
  Serial.begin(9600);
  // Definimos todos los pines de los motores como salida
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}



void loop() {
  // Se lee la información ofrecida por el sensor
  izq = digitalRead(SensorLeft);
  der = digitalRead(SensorRight);
  cen = digitalRead(SensorMiddle);
  fw = digitalRead(adelante);
  bw = digitalRead(atras);
  ri = digitalRead(derecha);
  le = digitalRead(izquierda);
  if(bw == 1){
    Return(stpsuave);
  }
  else if (fw == 1 or ri == 1 or le == 1) {
    if ((!der) and (!cen) and (!izq)) {
      Parar();
    } else if ((!izq) and (cen) and (der)) {
      left(stpsuave, velman);
    } else if ((!izq) and (!cen) and (der)) {
      left(stpsuave, velman);
    } else if ((izq) and (!cen) and (der)) {
      forward(stp, velade);
    } else if ((izq) and (!cen) and (!der)) {
      right(stpsuave, velman);
    } else if ((izq) and (cen) and (!der)) {
      right(stpsuave, velman);
    } else if ((!izq) and (cen) and (!der)) {
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



void forward(int st, int vel) {
  Serial.print("ADELANTE");
  //Direccion motor A
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  analogWrite(ENA, vel);  //Velocidad motor A
  //Direccion motor B
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);
  analogWrite(ENB, vel);  //Velocidad motor B
  delay(st);
  Parar();
}



void right(int st, int vel) {
  Serial.print("DERECHA");
  //Direccion motor A
  digitalWrite(IN2, LOW);
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, vel);  //Velocidad motor A
  //Direccion motor B
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);
  analogWrite(ENB, vel);  //Velocidad motor A
  delay(st);
  Parar();
}



void left(int st, int vel) {
  Serial.print("IZQUIERDA");
  //Direccion motor A
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, vel);  //Velocidad motor A
  //Direccion motor B
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, vel);  //Velocidad motor A
  delay(st);
  Parar();
}



void Parar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}



void Return(int st) {
  Serial.print("ATRAS");
  while (bw == 1 or (cen)) {
    if (ri == 1) {
      right(st, 255);
    } else if (le == 1) {
      left(st, 255);
    } else {
      right(st, 255);
    }
  }
  Parar();
}

/*if(izq == 0 &amp;&amp; der == 0)
  {
    robotParar(); // El robot para
 
  }
  // Si el izquierdo retorna 0 (zona blanca) y el derecho 1 (negra) el robot gira derecha
  if (izq == 0 &amp;&amp; der == 1)
  {
    robotDerecha();
     // El robot gira a la derecha
 
  }
  // Si el izquierdo retorna 1 (zona negra) y el derecho 0 (blanca) el robot gira izquierda
  if (izq == 1 &amp;&amp; der == 0)
  {
   robotIzquierda();
 
  }
  // Si ambos sensores retornan 0 (zona negra) el robot sigue recto
  if (izq == 1 &amp;&amp; der == 1)
  {
    robotAvance(); // El robot avanza
    Serial.println("robot avanza");
  }
*/