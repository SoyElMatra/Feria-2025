// Esto define que vamos a usar el sensor de proximidad.
#include <SR04.h>
#define TRIG_PIN A1
#define ECHO_PIN A0 // Este es el pin de datos donde está conectado el sensor ultrasónico SR04.
#define maxdist 30  // Esta es la distancia minima libre que debe tener el robot para avanzar

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);  // Inicializa la librería para poder usar el sensor ultrasónico.

long a;
int rad;
int distfront = 0;
int distleft = 0;
int distright = 0;

#include <Servo.h>    // Libreria para usar el servo que mueve el sensor de un lado a otro.
#define PINSERVO 11  // Pin en el que está conectado el servo.

// Angulos de trabajo angforward es el angulo cuando va hacia adelante.
#define angforward 85
#define angleft 40
#define angright 130
Servo myservo;  // create servo object to control a servo





// Motor A Derecba
int ENA = 5;
int IN1 = 2;
int IN2 = 4;

// Motor B Izquierda
int ENB = 10;
int IN3 = 7;
int IN4 = 8;
long tim = 0;

int vel = 255;
int stp = 100;

String show = "y";
int kp;

void setup()
// Abre los puertos y setea la salida para los motores
{
  Serial.begin(9600) ;
  // Declaramos todos los pines como salidas
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  myservo.attach(PINSERVO);
  //  Mirar();



}

void NoNo() { // Mueve el sensor de manera de simular un gesto de negación.
  int x;
  int dl = 3;
  for (x = angforward ; x <= angright; x += 1) {
    myservo.write(x);
    delay(dl);

  }
  for (int i = 1; i <= 2; i += 1) {


    for (x = angright; x >= angleft; x -= 1) {
      myservo.write(x);
      delay(dl);

    }
    for (x = angleft ; x <= angright; x += 1) {
      myservo.write(x);
      delay(dl);

    }
  }
  for (x = angright; x >= angforward; x -= 1) {
    myservo.write(x);
    delay(dl);

  }
return;
}

void Mirar() { // Esto permite "mirar" con el sensor y determinar las distancias adelante a la derecha y a la izquierda
  SetAngle(angleft);
  distleft = sr04.Distance();
  SetAngle(angright);
  distright = sr04.Distance();
  SetAngle(angforward);
  distfront = sr04.Distance();
  Serial.print ("Obstaculos: Left= ");
  Serial.print(distleft);
  Serial.print (" - Center= ");
  Serial.print(distfront);
  Serial.print (" - Right= ");
  Serial.println(distright);
return;
}

void SetAngle(int ang) {  // Posiciona el servo en el ángulo deseado
  myservo.write(ang);
  delay(300);
  return;
}
void loop() {

  if (Serial.available()) { // chequea si hay entrada del teclado o del bluetooth
    kp = Serial.read();
    //    Serial.print("Ingresó: ");
    //    Serial.print(kp);
    //    Serial.print("\n");
    if ((kp > 96) and (kp < 123)) {
      kp = kp - 32;
    }
    switch (kp)
    {
      case 'w': // Mueve adelante
        Adelante();
        break;
      case 'a': // Mueve hacia atr[as
        Atras();
        break;
      case 's': // Gira a la derecha
        Derecha();
        break;
      case 'd': // Gira a la izquierda
        Izquierda();
        break;
      case 'F': // Detiene la marcha (no se usa en este contexto porque no hay marcha continua)
        Parar();
        break;
      case '+': // Incrementa la velocidad en rangos de 10
        vel = vel + 10;
        if (vel > 255) {
          vel = 255;
        }
        break;
      case '-': // Reduce velocidad de a diez
        vel = vel - 10;
        if (vel < 50) {
          vel = 50;
        }
        break;
      case 'D': // Incrementa el delay entre movimientos Maximo 3 segundos
        stp = stp + 50;
        //        Serial.print(stp);
        if (stp > 3000) {
          stp = 3000;
        }
        break;
      case 'E': // Reduce el delay entre movimientos minimo una decima de segundo
        stp = stp - 50;
        if (stp < 25) {
          stp = 25;
        }
        break;
      case 'x': // Cambia la modalidad de mostrar datos en pantalla
        if (show == "y") {
          show = "n";
        }  else {
          show = "y";
        }
        break;
    }


    if (show == "y") { // Decide si debe mostrar en pantalla los valores
      tim = tim + 1;
//      if (tim >= 150000) {
        Serial.print ("Vel= ");
        Serial.print(vel);
        Serial.print ("   - Delay= ");
        Serial.println(stp);

  //      tim = 0;
    //  }
    }
  }
}




void Atras () // Mueve para atrás
{
  //Direccion motor A
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  analogWrite (ENB, vel); //Velocidad motor B
  delay (stp);
  Parar();
  return;
}

void Adelante () // Mueve adelante
{
  // aca hay que poner el sensor derecho para adelante
  myservo.write(angforward);
  a = sr04.Distance();
  if (a > maxdist) {
    //Direccion motor A
    digitalWrite (IN2, HIGH);
    digitalWrite (IN1, LOW);
    analogWrite (ENA, vel); //Velocidad motor A
    //Direccion motor B
    digitalWrite (IN4, HIGH);
    digitalWrite (IN3, LOW);
    analogWrite (ENB, vel); //Velocidad motor B
    delay (stp);
    Parar();
    return;
  } else {
NoNo();
    Parar();

    Serial.print ("Objeto a: ");
    Serial.print (a);
    Serial.println ("cm. ");
    return;
  }
return;
}

void Derecha ()
{
  //Direccion motor A
  digitalWrite (IN2, LOW);
  digitalWrite (IN1, HIGH);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW);
  analogWrite (ENB, vel); //Velocidad motor A
  delay (stp);
  Parar();
return;
}

void Izquierda ()
{
  //Direccion motor A
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, HIGH);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  analogWrite (ENB, vel); //Velocidad motor A
  delay (stp);
  Parar();
return;
}

void Parar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  return;
}
