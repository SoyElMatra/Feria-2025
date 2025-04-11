#define sensor1 12 // D0 del sensor a pin D12
#define sensor2 11 // D0 del sensor a pin D11
#define sensor3 10 // D0 del sensor a pin D10
void setup() {
  Serial.begin(9600);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
}

void loop() {
  int estado1 = digitalRead(sensor1);
  int estado2 = digitalRead(sensor2);
  int estado3 = digitalRead(sensor3);
  Serial.print("Sensor 1: ");
  if(estado1 == HIGH){
    Serial.println("OBJETO DETECTADO");
  }else{
    Serial.println("Sin objeto");
  }
  delay(100);
  Serial.print("Sensor 2: ");
  if(estado2 == HIGH){
    Serial.println("OBJETO DETECTADO");
  }else{
    Serial.println("Sin objeto");
  }
  delay(100);
  Serial.print("Sensor 3: ");
  if(estado3 == HIGH){
    Serial.println("OBJETO DETECTADO");
  }else{
    Serial.println("Sin objeto");
  }
  delay(100);
}