int tempC;
float detrasdelhumoestaP;
#define pinLM35 A1
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {
  tempC = analogRead(pinLM35); 
  detrasdelhumoestaP = (5.0 * tempC * 100.0)/1024.0; 
  Serial.println(detrasdelhumoestaP);
  delay(1000);
}