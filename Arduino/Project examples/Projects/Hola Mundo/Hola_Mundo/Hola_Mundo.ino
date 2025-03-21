int led7 = 7;
int led8 = 6;
int led9 = 5;
int led10 = 4;
int led11 = 3;
int led12 = 2;
int led13 = 1;
void setup() {
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);
  pinMode(led11, OUTPUT);
  pinMode(led12, OUTPUT);
  pinMode(led13, OUTPUT);

}

void loop() {
  digitalWrite(led7, LOW);
digitalWrite(led8, HIGH);
digitalWrite(led9, HIGH);
digitalWrite(led10, LOW);
digitalWrite(led11, HIGH);
digitalWrite(led12, HIGH);
digitalWrite(led13, HIGH);

delay(500);

digitalWrite(led7, HIGH);
digitalWrite(led8, HIGH);
digitalWrite(led9, HIGH);
digitalWrite(led10, HIGH);
digitalWrite(led11, HIGH);
digitalWrite(led12, HIGH);
digitalWrite(led13, LOW);

delay(750);

digitalWrite(led7, LOW);
digitalWrite(led8, LOW);
digitalWrite(led9, LOW);
digitalWrite(led10, HIGH);
digitalWrite(led11, HIGH);
digitalWrite(led12, HIGH);
digitalWrite(led13, LOW);

delay(500);

digitalWrite(led7, HIGH);
digitalWrite(led8, HIGH);
digitalWrite(led9, HIGH);
digitalWrite(led10, LOW);
digitalWrite(led11, HIGH);
digitalWrite(led12, HIGH);
digitalWrite(led13, HIGH);

delay(500);

digitalWrite(led7, LOW);
digitalWrite(led8, LOW);
digitalWrite(led9, LOW);
digitalWrite(led10, LOW);
digitalWrite(led11, LOW);
digitalWrite(led12, LOW);
digitalWrite(led13, LOW);

delay(1500);
}
