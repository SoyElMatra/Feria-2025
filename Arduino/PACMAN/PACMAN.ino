bool checkeacion;

//Fin y reinicio del juego
#define over 6
#define restart 7

// Motor A Derecha
#define ENA A6
#define IN1 A3
#define IN2 A4

// Motor B Izquierda
#define ENB A7
#define IN3 A5
#define IN4 A2

int vel = 255;
int stp = 255;
int a;
char currentCommand = 'S'; // Variable para guardar el comando actual

void setup() {
  Serial.begin(9600);
  // Declaramos todos los pines como salidas
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(over, INPUT);
  pinMode(restart, INPUT);
  Parar(); // Iniciar con motores detenidos
}

void loop() {
  // Leer comandos del serial
  if (Serial.available()) {
    char command = Serial.read();
    
    // Limpiar el buffer serial de caracteres extra
    while(Serial.available()) {
      Serial.read(); // Descarta los caracteres restantes
    }
    
    Serial.print("Comando recibido: ");
    Serial.println(command);
  
    // Ejecutar el comando actual continuamente
    switch (command) {
      case 'F':
        Serial.print("entre F");
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
      case '0':
        Parar();
        break;
    }
  }
  delay(50); // Pequeña pausa para estabilidad
}

void Atras() {
  //Direccion motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, vel);
  //Direccion motor B
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, vel);
}

void Adelante() {
  //Direccion motor A
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, vel);
  //Direccion motor B
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, vel);
}

void Derecha() {
  // Motor A gira atrás
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, vel);
  // Motor B gira adelante
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, vel);
}

void Izquierda() {
  // Motor A gira adelante
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, vel);
  // Motor B gira atrás
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, vel);
}

void Parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}