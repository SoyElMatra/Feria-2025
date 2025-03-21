#include "SR04.h"
#define TRIG_PIN A0
#define ECHO_PIN A1
#define LED1_PIN 0
#define LED2_PIN 1
#define LED3_PIN 2
#define LED4_PIN 3
#define LED5_PIN 4
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

void setup() {
   Serial.begin(9600);
   delay(100);
}

void loop() {
   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   delay(100);
   digitalWrite (LED1_PIN, LOW);
   digitalWrite (LED2_PIN, LOW);
   digitalWrite (LED3_PIN, LOW);
   digitalWrite (LED4_PIN, LOW);
   digitalWrite (LED5_PIN, LOW);
   
   if (sr04.Distance() > 20)
   digitalWrite (LED1_PIN, HIGH);
   digitalWrite (LED2_PIN, LOW);
   digitalWrite (LED3_PIN, LOW);
   digitalWrite (LED4_PIN, LOW);
   digitalWrite (LED5_PIN, LOW);
   if (sr04.Distance() > 40)
   digitalWrite (LED2_PIN, HIGH);
    digitalWrite (LED1_PIN, LOW);
   digitalWrite (LED3_PIN, LOW);
   digitalWrite (LED4_PIN, LOW);
   digitalWrite (LED5_PIN, LOW);
   
   if (sr04.Distance() > 60)
   digitalWrite (LED3_PIN, HIGH); 
    digitalWrite (LED1_PIN, LOW);
   digitalWrite (LED2_PIN, LOW);
   digitalWrite (LED4_PIN, LOW);
   digitalWrite (LED5_PIN, LOW);
   
   if (sr04.Distance() > 80)
   digitalWrite (LED4_PIN, HIGH);
   digitalWrite (LED1_PIN, LOW);
   digitalWrite (LED2_PIN, LOW);
   digitalWrite (LED3_PIN, LOW);
   digitalWrite (LED5_PIN, LOW);
   if (sr04.Distance() > 100)
   digitalWrite (LED5_PIN, HIGH); 
   digitalWrite (LED1_PIN, LOW);
   digitalWrite (LED2_PIN, LOW);
   digitalWrite (LED3_PIN, LOW);
   digitalWrite (LED4_PIN, LOW);
}
