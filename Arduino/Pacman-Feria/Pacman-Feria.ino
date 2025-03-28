#include "SR04.h"

//Sensor Distancia
#define TRIG_PIN 9
#define ECHO_PIN 8
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;
bool checkeacion;

//Fin y reinicio del juego
#define over 6
#define restart 7

// Motor A Derecba
#define ENA A0
#define IN1 2
#define IN2 3

// Motor B Izquierda
#define ENB A1
#define IN3 4
#define IN4 5

int vel = 255;
int stp = 150;

void setup() {
  Serial.begin(9600);
  // Declaramos todos los pines como salidas
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(over, INPUT);
  pinMode(restart, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Motor gira en un sentido
  a = sr04.Distance();
  Serial.print(a);
  Serial.println("cm");
  if (Serial.available())
    switch (Serial.read()) {
      case 'F':
        if (a > 5) {
          Adelante();
        }
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
    }
  delay(stp);
  Parar();
  checkeacion = digitalRead(over);
  if (checkeacion == 0) {
    checkeacion = digitalRead(restart);
    while (checkeacion == 1) {
      checkeacion = digitalRead(restart);
      Parar();
    }
    Serial.println("Reinicio");
  }
}

void Atras() {
  //Direccion motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, vel);  //Velocidad motor A
  //Direccion motor B
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, vel);  //Velocidad motor B
}

void Adelante() {
  //Direccion motor A
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  analogWrite(ENA, vel);  //Velocidad motor A
  //Direccion motor B
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);
  analogWrite(ENB, vel);  //Velocidad motor B
}

void Derecha() {
  //Direccion motor A
  digitalWrite(IN2, LOW);
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, vel);  //Velocidad motor A
  //Direccion motor B
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);
  analogWrite(ENB, vel);  //Velocidad motor A
}

void Izquierda() {
  //Direccion motor A
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, vel);  //Velocidad motor A
  //Direccion motor B
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, vel);  //Velocidad motor A
}

void Parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
