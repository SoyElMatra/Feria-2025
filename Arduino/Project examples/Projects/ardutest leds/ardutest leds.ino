#include "SR04.h"
#define TRIG_PIN A4
#define ECHO_PIN A3
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

void setup() {
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
     Serial.begin(9600);
   delay(1);
}

void loop() {
   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   delay(1);
   if(a<=10){
     digitalWrite(0, LOW);
     digitalWrite(3, LOW);
     digitalWrite(1, LOW);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(2, LOW);
          digitalWrite(9, LOW);
     digitalWrite(10, LOW);
     digitalWrite(11, LOW);
   }
      if(a>=10){
     digitalWrite(0, HIGH);
     digitalWrite(3, LOW);
     digitalWrite(1, LOW);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(2, LOW);
          digitalWrite(9, LOW);
     digitalWrite(10, LOW);
     digitalWrite(11, LOW);
   }
      if(a>=20){
     digitalWrite(0, HIGH);
     digitalWrite(3, HIGH);
     digitalWrite(1, LOW);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(2, LOW);
          digitalWrite(9, LOW);
     digitalWrite(10, LOW);
     digitalWrite(11, LOW);
   }
      if(a>=30){
     digitalWrite(0, HIGH);
     digitalWrite(3, HIGH);
     digitalWrite(1, HIGH);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(2, LOW);
          digitalWrite(9, LOW);
     digitalWrite(10, LOW);
     digitalWrite(11, LOW);
   }
      if(a>=40){
     digitalWrite(0, HIGH);
     digitalWrite(3, HIGH);
     digitalWrite(1, HIGH);
     digitalWrite(4, HIGH);
     digitalWrite(5, LOW);
     digitalWrite(2, LOW);
          digitalWrite(9, LOW);
     digitalWrite(10, LOW);
     digitalWrite(11, LOW);
   }
      if(a>=50){
     digitalWrite(0, HIGH);
     digitalWrite(3, HIGH);
     digitalWrite(1, HIGH);
     digitalWrite(4, HIGH);
     digitalWrite(5, HIGH);
     digitalWrite(2, LOW);
          digitalWrite(9, LOW);
     digitalWrite(10, LOW);
     digitalWrite(11, LOW);
   }
      if(a>=60){
     digitalWrite(0, HIGH);
     digitalWrite(3, HIGH);
     digitalWrite(1, HIGH);
     digitalWrite(4, HIGH);
     digitalWrite(5, HIGH);
     digitalWrite(2, HIGH);
     digitalWrite(9, LOW);
     digitalWrite(10, LOW);
     digitalWrite(11, LOW);
   }
   if(a>=70){
     digitalWrite(0, LOW);
     digitalWrite(1, LOW);
     digitalWrite(2, LOW);
     digitalWrite(3, LOW);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(9, HIGH);
     digitalWrite(10, LOW);
     digitalWrite(11, LOW);
   }
      if(a>=80){
     digitalWrite(0, LOW);
     digitalWrite(1, LOW);
     digitalWrite(2, LOW);
     digitalWrite(3, LOW);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(9, HIGH);
     digitalWrite(10, HIGH);
     digitalWrite(11, LOW);
   }
      if(a>=70){
     digitalWrite(0, LOW);
     digitalWrite(1, LOW);
     digitalWrite(2, LOW);
     digitalWrite(3, LOW);
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(9, HIGH);
     digitalWrite(10, HIGH);
     digitalWrite(11, HIGH);
   }
   if(a>=100){
     digitalWrite(0, HIGH);
     digitalWrite(1, HIGH);
     digitalWrite(2, HIGH);
     digitalWrite(3, HIGH);
     digitalWrite(4, HIGH);
     digitalWrite(5, HIGH);
     digitalWrite(9, HIGH);
     digitalWrite(10, HIGH);
     digitalWrite(11, HIGH);
   }
}