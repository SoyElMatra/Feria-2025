// Motor A Derecba
int ENA = 2;
int IN1 = 3;
int IN2 = 4;

// Motor B Izquierda
int ENB = 5;
int IN3 = 6;
int IN4 = 7;

int izq = 0;
int der = 0;
int cen = 0;

int velade = 190;
int velman = 100;
int stp = 50;
int stpsuave = 10;

// Guardar el número de pin de Arduino al que está conectado el pin OUT del sensor
#define SensorLeft    8   // input pin of left sensor
#define SensorMiddle  9   // input pin of middle sensor
#define SensorRight  10   // input pin of right sensor

void setup() {
  // Se indica que ese pin va a utilizarse para recibir información
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  // Se activa el monitor serie para mostrar información posteriormente
  Serial.begin(9600);
  // Definimos todos los pines de los motores como salida
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
}



void loop() {
  // Se lee la información ofrecida por el sensor
  izq = digitalRead(SensorLeft);
  der = digitalRead(SensorRight);
  cen = digitalRead(SensorMiddle);

  if ((!der) and (!cen) and (!izq)) {
    Parar();
  } else if ((!izq) and (cen) and (der)) {
    Izquierda(stpsuave, velman);
  } else if ((!izq) and (!cen) and (der)) {
    Izquierda(stpsuave, velman);
  } else if ((izq) and (!cen) and (der)) {
    Adelante(stp, velade);
  } else if ((izq) and (!cen) and (!der)) {
    Derecha(stpsuave, velman);
  } else if ((izq) and (cen) and (!der)) {
    Derecha(stpsuave, velman);
  } else if ((izq) and (cen) and (der)) {
    Parar();
  }

}



void Adelante(int st, int vel) {
  //Direccion motor A
  digitalWrite (IN2, HIGH);
  digitalWrite (IN1, LOW);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW);
  analogWrite (ENB, vel); //Velocidad motor B
  delay (st);
  Parar();

}



void Atras (int st, int vel) // Mueve para atrás
{
  //Direccion motor A
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  analogWrite (ENB, vel); //Velocidad motor B
  delay (st);
  Parar();
}



void Derecha (int st, int vel)
{
  //Direccion motor A
  digitalWrite (IN2, LOW);
  digitalWrite (IN1, HIGH);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW);
  analogWrite (ENB, vel); //Velocidad motor A
  delay (st);
  Parar();

}



void Izquierda (int st, int vel)
{
  //Direccion motor A
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, HIGH);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  analogWrite (ENB, vel); //Velocidad motor A
  delay (st);
  Parar();

}



void Parar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
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