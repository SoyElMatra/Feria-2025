// Esto define que vamos a usar el sensor de proximidad.
#include "SR04.h"
#define TRIG_PIN A1
#define ECHO_PIN A0
#define maxdist 30
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

long a;
int rad;
int distfront = 0;
int distleft = 0;
int distright = 0;

#include <Servo.h>
#define PINSERVO 11
#define angforward 85
#define angleft 40
#define angright 130
Servo myservo;  // create servo object to control a servo





// Motor A Derecba
int ENA = 4;
int IN1 = 5;
int IN2 = 6;

// Motor B Izquierda
int ENB = 13;
int IN3 = 7;
int IN4 = 8;
long tim = 0;

int vel = 255;
int stp = 150;

String show = "y";

void setup()
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

void NoNo() {
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

}

void Mirar() {
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

}

void SetAngle(int ang) {
  myservo.write(ang);
  delay(300);
}



void loop() {
  // put your main code here, to run repeatedly:
  // Motor gira en un sentido

  if (Serial.available())
    switch (Serial.read())
    {
      case 'F':
        Adelante();
        break;
      case 'B':
        Atras();
        break;
      case 'R':
        Derecha();
        break;
      case 'L':
        Izquierda();
        break;
      case 'S':
        Parar();
        break;
      case '+':
        vel = vel + 10;
        if (vel > 255) {
          vel = 255;
        }
        break;
      case '-':
        vel = vel - 10;
        if (vel < 50) {
          vel = 50;
        }
        break;
      case 'a':
        stp = stp + 10;
        if (stp > 1000) {
          stp = 1000;
        }
        break;
      case 'e':
        stp = stp - 10;
        if (stp < 100) {
          stp = 100;
        }
        break;
      case 'W':
        if (show == "y") {
          show = "n";
        }  else {
          show = "y";
        }
        break;
    }


  if (show == "y") {
    tim = tim + 1;
    if (tim >= 150000) {
      Serial.print ("Vel= ");
      Serial.print(vel);
      Serial.print ("   - Delay= ");
      Serial.println(stp);

      tim = 0;
    }
  }
}




void Atras ()
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

void Adelante ()
{
  // aca hay que poner el sensor derecho para adelante
  myservo.write(angforward);
  a = sr04.Distance();
  if (a < maxdist) {
    // aca llo que tiene que hacer si hay un objeto
    NoNo();
    Parar();

    Serial.print ("Objeto a: ");
    Serial.print (a);
    Serial.println ("cm. ");
  } else {


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
