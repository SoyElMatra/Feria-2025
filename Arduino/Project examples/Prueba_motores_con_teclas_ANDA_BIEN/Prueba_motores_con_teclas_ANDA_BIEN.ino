// Motor A Derecba
int IN1 = A3;
int IN2 = A4;

// Motor B Izquierda
int IN3 = A5;
int IN4 = A2;

int vel = 255;
int stp = 150;

void setup()
{
 Serial.begin(9600) ;
 // Declaramos todos los pines como salidas
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
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, HIGH);
 //Direccion motor B
 digitalWrite (IN3, LOW);
 digitalWrite (IN4, HIGH);
 }

void Adelante ()
{
 //Direccion motor A
 digitalWrite (IN2, HIGH);
 digitalWrite (IN1, LOW);
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, LOW);
 
}

void Derecha ()
{
 //Direccion motor A
 digitalWrite (IN2, LOW);
 digitalWrite (IN1, HIGH);
 //Direccion motor B
 digitalWrite (IN4, HIGH);
 digitalWrite (IN3, LOW);
 
} 

void Izquierda ()
{
 //Direccion motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, HIGH);
 //Direccion motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW); 
} 

void Parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
