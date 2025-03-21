int dd;
void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  delay(1);
}

void loop() {
digitalWrite(13, HIGH);
delay(dd);
digitalWrite(13, LOW);
dd=dd-1;
Serial.println(dd);
if(dd<=1){
  dd=500;
}
delay(dd);
}
