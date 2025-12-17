// PINES CORREGIDOS CON ENA/ENB
#define ENA 3       // PWM Motor Izquierdo - PIN 3 //10
#define IN1 A5      // Dirección 
#define IN2 A4      // Dirección

#define ENB 5       // PWM Motor Derecho - PIN 5  //8
#define IN3 A3      // Dirección
#define IN4 A2      // Dirección

// SENSORES
#define SensorLeft 6
#define SensorMiddle 10  
#define SensorRight 12
#define SensorBack 4    //9

// Variables de sensores
int L, C, R, B;

// Variables de control
int velocidad = 120;
int tiempoMaximoGiro = 3000;  // Tiempo máximo de giro para evitar bucles infinitos
int MINI_AVANCE = 130;
int paro_motores = 1000;
int despegue_de_linea = 300;

// Control por Bluetooth
bool robotActivo = false;

unsigned long ultimaDeteccion = 0;
const int tiempoMinEntreDecisiones = 1000;

void configurarPWMIdentica() {
  // Timer2 (ENA - pin 3) - ~61.04 Hz
  TCCR2B = 0;
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20); // Prescaler 1024
  
  // Timer0 (ENB - pin 5) - ~61.04 Hz (misma frecuencia)
  TCCR0B = 0;
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
  TCCR0B = _BV(CS02) | _BV(CS00); // Prescaler 1024
}


void setup() {
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  pinMode(SensorBack, INPUT);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  configurarPWMIdentica();
  
  // Iniciar comunicación Bluetooth
  Serial.begin(9600);
  Serial.println("=== PACMAN ROBOT - CONTROL BLUETOOTH ===");
  Serial.println("Comandos Bluetooth:");
  Serial.println("E - Encender robot");
  Serial.println("A - Apagar robot");
  Serial.println("S - Estado sensores");
  Serial.println("+ - Aumentar velocidad");
  Serial.println("- - Disminuir velocidad");
  Serial.println("V - Ver velocidad actual");
  Serial.println("W - Avanzar");
  Serial.println("P - Parar motores");
  Serial.println("I - Izquierda");
  Serial.println("D - Derecha");
  
  pararMotores();
}

void loop() {
  // Leer comandos Bluetooth
  if (Serial.available()) {
    char comando = Serial.read();
    
    // Filtrar caracteres no deseados (enter, retorno de carro, espacios)
    if (comando != '\n' && comando != '\r' && comando != ' ') {
      procesarComando(comando);
    }
  }
  
  if (robotActivo) {
    // Lectura de sensores
    L = digitalRead(SensorLeft);
    C = digitalRead(SensorMiddle);
    R = digitalRead(SensorRight);
    B = digitalRead(SensorBack);
    
    // Enviar estado de sensores por Bluetooth periódicamente
    static unsigned long ultimoReporte = 0;
    if (millis() - ultimoReporte > 500) { // Reportar cada 500ms
      Serial.print("SENSORES:L:");
      Serial.print(L);
      Serial.print(":C:");
      Serial.print(C);
      Serial.print(":R:");
      Serial.print(R);
      Serial.print(":B:");
      Serial.println(B);
      ultimoReporte = millis();
    }
    
    // Evitar múltiples detecciones rápidas
    unsigned long tiempoActual = millis();
    if (tiempoActual - ultimaDeteccion < tiempoMinEntreDecisiones) {
      avanzar();
      delay(50);
      return;
    }
    
    // Lógica de detección automática
    if (L == 0 && C == 1 && R == 0) {
      avanzar();
      Serial.println("avanzar");
    }
    else if (L == 1 && C == 1 && R == 0) {
      Serial.println("EVENTO:CRUCE_IZQUIERDA");
      decisionAleatoriaCruce("IZQUIERDA");
      ultimaDeteccion = millis();
    }
    else if (L == 0 && C == 1 && R == 1) {
      Serial.println("EVENTO:CRUCE_DERECHA");
      decisionAleatoriaCruce("DERECHA");
      ultimaDeteccion = millis();
    }
    else if (L == 1 && C == 1 && R == 1) {
      Serial.println("EVENTO:CRUCE_T");
      decisionAleatoriaCruce("T");
      ultimaDeteccion = millis();
    }
    else if (L == 1 && C == 0 && R == 1) {
      Serial.println("EVENTO:CRUCE_COMPLETO");
      decisionAleatoriaCruce("COMPLETO");
      ultimaDeteccion = millis();
    }
    //else if (L == 0 && C == 0 && R == 0) {
     // Serial.println("EVENTO:PERDIDA_LINEA");
      //buscarLinea();
    //}
    
  }
  
  delay(30);
}

void procesarComando(char comando) {
  // Convertir a mayúscula para simplificar
  comando = toupper(comando);
  
  switch(comando) {
    case 'E':
              robotActivo = true;
              Serial.println("ESTADO:ROBOT_ACTIVADO");
              break;
      
    case 'A':
              robotActivo = false;
              Serial.println("ESTADO:ROBOT_APAGADO");
              pararMotores();
              break;
      
    case 'S':
              L = digitalRead(SensorLeft);
              C = digitalRead(SensorMiddle);
              R = digitalRead(SensorRight);
              B = digitalRead(SensorBack);
              Serial.print("SENSORES:L:");
              Serial.print(L);
              Serial.print(":C:");
              Serial.print(C);
              Serial.print(":R:");
              Serial.print(R);
              Serial.print(":B:");
              Serial.println(B);
              break;
      
    case '+':
              velocidad = min(255, velocidad + 10);
              Serial.print("VELOCIDAD:");
              Serial.println(velocidad);
              break;
      
    case '-':
              velocidad = max(80, velocidad - 10);
              Serial.print("VELOCIDAD:");
              Serial.println(velocidad);
              break;
      
    case 'V':
              Serial.print("VELOCIDAD_ACTUAL:");
              Serial.println(velocidad);
              break;
       
    case 'W':
              avanzar();
              Serial.println("ACCION:AVANZAR");
              break;
           
    case 'I':
              girarIzquierda();
              Serial.println("ACCION:GIRO_IZQUIERDA");
              break;
            
    case 'D':
              girarDerecha();
              Serial.println("ACCION:GIRO_DERECHA");
              break;
            
    case 'P':
              pararMotores();
              Serial.println("ACCION:PARAR");
              break;
      
    default:
              // Solo mostrar error para caracteres que no sean control
              if (comando >= 32 && comando <= 126) { // Caracteres imprimibles
                Serial.print("ERROR:COMANDO_DESCONOCIDO: ");
                Serial.println(comando);
              }
              break;
  }
}
             

// ✅ GIRO IZQUIERDA HASTA QUE CENTRO ESTÉ EN 1 Y LATERALES EN 0
void secuenciaCruceIzquierda() {
  Serial.println("ACCION:INICIANDO_GIRO_IZQUIERDA");
  pararMotores();
  delay(paro_motores);
  avanzar();
  Serial.println("HACIENDO_MINI_AVANCE");
  delay(MINI_AVANCE);
  pararMotores();
  
  unsigned long inicioGiro = millis();
  

  //do{
  //    digitalWrite(IN1, LOW);
  //    digitalWrite(IN2, HIGH);
  //    digitalWrite(IN3, HIGH);
  //    digitalWrite(IN4, LOW);
  //    analogWrite(ENA, 130);
  //    analogWrite(ENB, 130);

  //}while(C == 1 && L == 0 && R == 0)

  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  

  //delay(despegue_de_linea);
  Serial.println("despegue");


  //L = digitalRead(SensorLeft);
  //C = digitalRead(SensorMiddle);
  //R = digitalRead(SensorRight);
  

  //antirrebote, 
  // Girar hasta que centro=1, back=1 y laterales=0 o tiempo máximo alcanzado
 while (!(C == 1 && L == 0 && R == 0 && B == 1) && (millis() - inicioGiro < tiempoMaximoGiro)) {
    
    delay(100);
    L = digitalRead(SensorLeft);
    C = digitalRead(SensorMiddle);
    R = digitalRead(SensorRight);
    B = digitalRead(SensorBack);
    delay(10);
  }
  
  pararMotores();
  Serial.println("ACCION:GIRO_IZQUIERDA_COMPLETADO");
  delay(100);
}

// ✅ GIRO DERECHA HASTA QUE CENTRO ESTÉ EN 1 Y LATERALES EN 0
void secuenciaCruceDerecha() {
  Serial.println("ACCION:INICIANDO_GIRO_DERECHA");
  pararMotores();
  delay(paro_motores);
  avanzar();
  Serial.println("HACIENDO_MINI_AVANCE");
  delay(MINI_AVANCE);
  pararMotores();
  unsigned long inicioGiro = millis();
  

  //do{
  //    digitalWrite(IN1, LOW);
  //    digitalWrite(IN2, HIGH);
  //    digitalWrite(IN3, HIGH);
  //    digitalWrite(IN4, LOW);
  //    analogWrite(ENA, 130);
  //    analogWrite(ENB, 130);

  //}while(C == 1 && L == 0 && R == 0)

  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);


  //delay(despegue_de_linea);
  Serial.println("despegue");


  //L = digitalRead(SensorLeft);
  //C = digitalRead(SensorMiddle);
  //R = digitalRead(SensorRight);
  
  // Girar hasta que centro=1, back=1 y laterales=0 o tiempo máximo alcanzado
  while (!(C == 1 && L == 0 && R == 0 && B == 1) && (millis() - inicioGiro < tiempoMaximoGiro)) {
    
    delay(100);
    L = digitalRead(SensorLeft);
    C = digitalRead(SensorMiddle);
    R = digitalRead(SensorRight);
    B = digitalRead(SensorBack);
    delay(10);
  }
  
  pararMotores();
  Serial.println("ACCION:GIRO_DERECHA_COMPLETADO");
  delay(100);
}

// ✅ FUNCIONES BÁSICAS DE MOVIMIENTO
void avanzar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void girarIzquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void girarDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}



void decisionAleatoriaCruce(String tipoCruce) {
  int decision = random(100);
  Serial.print("DECISION_ALEATORIA:");
  Serial.println(decision);
  
  if (tipoCruce == "COMPLETO") {
    if (decision < 50) {
      Serial.println("ACCION:GIRO_IZQUIERDA_50%");
      secuenciaCruceIzquierda();
    } else {
      Serial.println("ACCION:GIRO_DERECHA_50%");
      secuenciaCruceDerecha();
    }
  }
  else if (tipoCruce == "IZQUIERDA") {
    Serial.println("ACCION:GIRO_IZQUIERDA_OBLIGATORIO");
    secuenciaCruceIzquierda();
  }
  else if (tipoCruce == "DERECHA") {
    Serial.println("ACCION:GIRO_DERECHA_OBLIGATORIO");
    secuenciaCruceDerecha();
  }
  else if (tipoCruce == "T") {
    if (decision < 40) {
      Serial.println("ACCION:AVANZAR_40%");
      pararMotores();
      delay(500);
      avanzar();
      delay(300);
    }
    else if (decision < 70) {
      Serial.println("ACCION:GIRO_IZQUIERDA_30%");
      secuenciaCruceIzquierda();
    }
    else {
      Serial.println("ACCION:GIRO_DERECHA_30%");
      secuenciaCruceDerecha();
    }
  }
}

