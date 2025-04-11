// Motor A Derecba
int ENA = A0;
int IN1 = 2;
int IN2 = 3;

// Motor B Izquierda
int ENB = A1;
int IN3 = 4;
int IN4 = 5;

int vel = 255;
int stp = 150;

void setup()
{
 Serial.begin(9600) ;
 // Declaramos todos los pines como salidas
 pinMode (ENA, OUTPUT);
 pinMode (ENB, OUTPUT);
 pinMode (IN1, OUTPUT);
 pinMode (IN2, OUTPUT);
 pinMode (IN3, OUTPUT);
 pinMode (IN4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
 // Motor gira en un sentido
 if (Serial.available())
 switch (Serial.read())
 {
 case 'F':
 Adelante();
 break;
 case 'B':
 Atras();
 break;
 case 'R':
 Derecha();
 break;
 case 'L':
 Izquierda();
 break;
 case 'S':
 Parar();
 break;
 }
delay (stp);
Parar();
}

void Atras ()
{
 //Direccion motor A
 digitalWrite (IN1, HIGH);
 digitalWrite (IN2, LOW);
 analogWrite (ENA, vel); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW);
 analogWrite (ENB, vel); //Velocidad motor B
 }

void Adelante ()
{
 //Direccion motor A
 digitalWrite (IN2, HIGH);
 digitalWrite (IN1, LOW);
 analogWrite (ENA, vel); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, LOW);
 analogWrite (ENB, vel); //Velocidad motor B
 
}

void Derecha ()
{
 //Direccion motor A
 digitalWrite (IN2, LOW);
 digitalWrite (IN1, HIGH);
 analogWrite (ENA, vel); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, LOW);
 analogWrite (ENB, vel); //Velocidad motor A
 
} 

void Izquierda ()
{
 //Direccion motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, HIGH);
 analogWrite (ENA, vel); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW);
 analogWrite (ENB, vel); //Velocidad motor A
 
} 

void Parar() {
  analogWrite(ENA,0);
  analogWrite(ENB,0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
