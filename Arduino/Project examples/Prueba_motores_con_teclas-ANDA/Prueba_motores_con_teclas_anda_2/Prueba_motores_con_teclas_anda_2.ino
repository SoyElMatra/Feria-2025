// Motor A Derecba
int ENA = 9;
int IN1 = 5;
int IN2 = 6;

// Motor B Izquierda
int ENB = 10;
int IN3 = 7;
int IN4 = 8;
long tim = 0;

int vel = 255;
int stp = 150;

String show = "y";

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
      case '+':
        vel = vel + 10;
        if (vel > 255) {
          vel = 255;
        }
        break;
      case '-':
        vel = vel - 10;
        if (vel < 50) {
          vel = 50;
        }
        break;
      case 'a':
        stp = stp + 10;
        if (stp > 500) {
          stp = 500;
        }
        break;
      case 'e':
        stp = stp - 10;
        if (stp < 100) {
          stp = 100;
        }
        break;
      case 'W':
        if (show == "y") {
          show = "n";
        }  else {
          show = "y";
        }
        break;
    }


  if (show == "y") {
    tim = tim + 1;
    if (tim >= 150000) {
      Serial.print ("Vel= ");
      Serial.print(vel);
      Serial.print ("   - Delay= ");
      Serial.println(stp);

      tim = 0;
    }
  }
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
  delay (stp);
  Parar();
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
  delay (stp);
  Parar();

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
  delay (stp);
  Parar();

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
  delay (stp);
  Parar();

}

void Parar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
