#include <Servo.h>
#include "SR04.h"
// Motor A Derecba
int ENA = 5;
int IN1 = 2;
int IN2 = 4;


// Motor B Izquierda
int ENB = 10;
int IN3 = 7;
int IN4 = 8;

int stp = 100;

Servo servoMotor;

#define TRIG_PIN A1
#define ECHO_PIN A0
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
int maxdist = 30;
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
 pinMode (TRIG_PIN, OUTPUT);
 pinMode (ECHO_PIN, INPUT);
 servoMotor.attach(9);
}

void loop() {
   a=sr04.Distance();
 
if(a<maxdist){
  Serial.println("No se puede, hay un objeto adelante.");
  Parar();
}else{
  Serial.println(a);
}
 if (Serial.available())
 switch (Serial.read())
 {
 case 'w':
 Adelante();
 break;
 case 's':
 Atras();
 break;
 case 'd':
 Derecha();
 break;
 case 'a':
 Izquierda();
 break;
 case 'T':
 Parar();
 break;
 case 'e':
 DerLante();
 break;
 case 'q':
 IzqLante();
 break;
 case 'c':
 DerTras();
 break;
 case 'z':
 IzqTras();
 break;
 case 'g':
 AdeConst();
 break;
  case '+':
 stp = stp + 10;
 if (stp > 1000){
   stp = 1000;
 }
 break;
 case '-':
 stp = stp - 10;
 if (stp < 1){
   stp = 1;
 }
 break;
 }

delay (stp);
Parar();
}

void Atras ()
{servoMotor.write(0);
 digitalWrite (IN1, HIGH);
 digitalWrite (IN2, LOW);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW);
 digitalWrite(ENB, HIGH); //Velocidad motor B
 }

void Adelante ()
{servoMotor.write(0);
  if (a > maxdist){
 //Direccion motor A
 digitalWrite (IN2, HIGH);
 digitalWrite (IN1, LOW);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, LOW);
 digitalWrite(ENB, HIGH); //Velocidad motor B
  }else{
Parar();
  }
}

void Derecha ()
{servoMotor.write(0);
 //Direccion motor A
 digitalWrite (IN2, LOW);
 digitalWrite (IN1, HIGH);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, LOW);
 digitalWrite(ENB, HIGH); //Velocidad motor A
} 

void Izquierda ()
{servoMotor.write(0);
 //Direccion motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, HIGH);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW);
 digitalWrite(ENB, HIGH); //Velocidad motor A
 
} 

void Parar() {
 digitalWrite(ENA, LOW);
 digitalWrite(ENB, LOW);
}

void DerLante () 
{servoMotor.write(0);
if (a > maxdist){
 //Direccion motor A
 digitalWrite (IN2, HIGH);
 digitalWrite (IN1, LOW);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, HIGH);
 digitalWrite(ENB, HIGH); //Velocidad motor B
  }else{
Parar();
  }
}

void IzqLante () 
{servoMotor.write(0);
if (a > maxdist){
 //Direccion motor A
 digitalWrite (IN2, HIGH);
 digitalWrite (IN1, HIGH);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, LOW);
 digitalWrite(ENB, HIGH); //Velocidad motor B
  }else{
Parar();
  }
}

void DerTras () 
{servoMotor.write(0);
 digitalWrite (IN2, LOW);
 digitalWrite (IN1, HIGH);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, HIGH);
 digitalWrite(ENB, HIGH); //Velocidad motor B
}

void IzqTras () 
{servoMotor.write(0);
 //Direccion motor A
 digitalWrite (IN2, HIGH);
 digitalWrite (IN1, HIGH);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, LOW);
 digitalWrite (IN3, HIGH);
 digitalWrite(ENB, HIGH); //Velocidad motor B
}

void AdeConst () 
{servoMotor.write(0);
     a=sr04.Distance();
Serial.println(a);
  if (a > maxdist){
 //Direccion motor A
 digitalWrite (IN2, HIGH);
 digitalWrite (IN1, LOW);
 digitalWrite(ENA, HIGH); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, LOW);
 digitalWrite(ENB, HIGH); //Velocidad motor B
 if (a < maxdist){
   return;
 }
  if (Serial.available())
 switch (Serial.read())
 {
 case 'T':
 return;
 break;
 }
 AdeConst();
}else{
Parar();
}
}
