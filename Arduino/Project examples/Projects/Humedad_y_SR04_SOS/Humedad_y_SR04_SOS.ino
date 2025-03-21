#include "SR04.h"
#define TRIG_PIN 2
#define ECHO_PIN 3
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
const int sensor = A3;
int AB;

void setup() {
  AB=0;
 Serial.begin(9600);
 delay(1);

}

void loop() {
       int sensorValue = analogRead(sensor);
  int percentage = map(sensorValue, 0, 1023, 100, 0);
  Serial.print("humedad ");
  Serial.print(percentage);
  Serial.print("%, ");

   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   delay(100);
     if (Serial.available()){
    switch (Serial.read()){
case 'S':
if (AB==0){
  AB=1;
} else if (AB==1){
  AB=2;
}else if(AB==2){
  AB=0;
}}}
    

  if(AB>=1){
    digitalWrite(12, LOW);
  }if (AB==0){

  
if(percentage<=10){
  digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(100);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(100);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(300);
digitalWrite(12, HIGH);
delay(300);
digitalWrite(12, LOW);
delay(300);
digitalWrite(12, HIGH);
delay(300);
digitalWrite(12, LOW);
delay(300);
digitalWrite(12, HIGH);
delay(300);
digitalWrite(12, LOW);
delay(300);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(100);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(100);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(2000);
return;
}
else{
  digitalWrite(12, LOW);
}}else{
  if(a>=100){
  digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(100);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(100);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(300);
digitalWrite(12, HIGH);
delay(300);
digitalWrite(12, LOW);
delay(300);
digitalWrite(12, HIGH);
delay(300);
digitalWrite(12, LOW);
delay(300);
digitalWrite(12, HIGH);
delay(300);
digitalWrite(12, LOW);
delay(300);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(100);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(100);
digitalWrite(12, HIGH);
delay(100);
digitalWrite(12, LOW);
delay(2000);
return;
}
else{
  digitalWrite(12, LOW);
}}
delay(100);
}
