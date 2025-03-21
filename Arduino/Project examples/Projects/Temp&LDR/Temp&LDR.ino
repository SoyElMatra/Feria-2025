int LDR=14;
int Temp=15;
int a;
int b;
int c;
int d;
void setup() {
  pinMode(LDR, INPUT);
  Serial.begin(9600);
}

void loop() {
  a=digitalRead(19);
  b=analogRead(15);
  c=((4*b)*100)/1023;
  d=c*1.8;
   int sensorValue = analogRead(LDR);
  int percentage = map(sensorValue, 0, 1023, 100, 0);
  if(a==1){
  Serial.print("iluminacion: ");
  Serial.print(percentage);
  Serial.println("%");
}if(a==0){
  Serial.print(c);
  Serial.println("°");
}delay(100);
}
