String xx;
int x;

void setup() {
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  testing(0);
  testing(1);
  testing(2);
  testing(3);
  testing(4);
  testing(5);
  testing(6);
  testing(7);
  testing(8);
  testing(9);
  testing(10);
  testing(11);
  testing(13);
  testing(A0);
  testing(A1);
  testing(A2);
  testing(A3);
  testing(A4);
  testing(A5);
  
  
}

void testing(int puerto){
  pinMode(puerto, OUTPUT);
  Serial.println ("Probando puerto: ");
    Serial.print (puerto);
    Serial.print (" Encendiendo...");
    digitalWrite(puerto, HIGH);
    delay(1000);
    Serial.print (" Apagando...");
    digitalWrite(puerto, LOW);
    Serial.println();

}


  


