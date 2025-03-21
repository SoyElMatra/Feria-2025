int L1 = 9;
int L2 = 12;
int L3 = 1;
int L4 = 8;
int S1 = 13;
int S2 = 5;
void setup() {
pinMode(L1, OUTPUT);
pinMode(L2, OUTPUT);
pinMode(L3, OUTPUT);
pinMode(L4, OUTPUT);
pinMode(S1, INPUT);
pinMode(S2, INPUT);
}

void loop() {
int a=digitalRead(S1);
int b=digitalRead(S2);
if (a==0 && b==0){
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  digitalWrite(L3, LOW);
  digitalWrite(L4, LOW);
}else if (a==0 && b==1){
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
  digitalWrite(L3, LOW);
  digitalWrite(L4, LOW);
}else if (a==1 && b==0){
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(L3, HIGH);
  digitalWrite(L4, LOW);
}else if (a==1 && b==1){
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(L3, LOW);
  digitalWrite(L4, HIGH);
}
}
