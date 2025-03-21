#include "SR04.h"
#define TRIG_PIN A0 //TRIG pin del sensor HC SR04 seteado en A0
#define ECHO_PIN A1 //ECHO pin del sensor HC SR04 seteado en A1 
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
const int pir_output = 4;  //Sensor HC SR501 seteado en el puerto 0

void setup() {
  pinMode(pir_output, INPUT);
  Serial.begin(9600);
}

//Verificar si el sensor detecta movimiento o no
void loop() {
  a=sr04.Distance();
  if (digitalRead(pir_output) == HIGH) {
    Serial.print("HIGH, ");
    Serial.print(a);
    Serial.println("cm");
  }
  else {
   Serial.print("LOW, ");
    Serial.print(a);
    Serial.println("cm");
  }
  delay(1000);
}
