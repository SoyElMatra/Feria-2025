int Buzzer=12;
int Switch=19;
int a;
long DO=523.25, //definimos las frecuencias de las notas
DoS=554.37,
RE=587.33,
RES=622.25,
MI=659.26,
FA=698.46,
FAS=739.99,
SOL=783.99,
SOLS=830.61,
LA=880,
LAS=932.33,
SI=987.77,
RE2=1174.66,
FAS2=1479.98;
void setup() {
  Serial.begin(9600);
pinMode(Buzzer, OUTPUT);
pinMode(Switch, INPUT);
}

void loop(){
a=digitalRead(Switch);
Serial.println(a);
if(a==0){
  tone(Buzzer, 0, 1000);
}else{
  if(a==0){
  tone(Buzzer, 0, 1000);
}
tone(Buzzer, RE, 200);
delay(700);
tone(Buzzer, RE, 200);
delay(700);
tone(Buzzer, RE, 200);
delay(700);
tone(Buzzer, RE, 200);
delay(325);
tone(Buzzer, LA, 200);
delay(325);
tone(Buzzer, SOL, 200);
delay(700);
tone(Buzzer, SOL, 200);
delay(700);
tone(Buzzer, SOL, 200);
delay(200);
tone(Buzzer, SOL, 200);
delay(200);
tone(Buzzer, LA, 200);
delay(200);
tone(Buzzer, LAS, 200);
delay(200);
tone(Buzzer, LA, 200);
delay(350);
tone(Buzzer, SOL, 200);
delay(375);
}
}


