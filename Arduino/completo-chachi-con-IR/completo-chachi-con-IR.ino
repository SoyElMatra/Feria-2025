// PINES CORREGIDOS
#define IN1 A5
#define IN2 A4
#define IN3 A3
#define IN4 A2

// SENSORES
#define SensorLeft 10
#define SensorMiddle 11  
#define SensorRight 12

// Variables de sensores
int L, C, R;

// Variables de control
int velocidad = 180;
int tiempoPosicionamiento = 30;  // Avance corto después de parar
int tiempoGiro = 330;            // Tiempo de giro

// Control por serial
bool robotActivo = false;

void setup() {
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("=== ROBOT - PARADA EN CRUCE ===");
  Serial.println("Comandos: 'E' - Empezar, 'A' - Apagar, 'T' - Test");
  
  pararMotores();
}

void loop() {
  if (Serial.available()) {
    String comando = Serial.readString();
    comando.trim();
    
    if (comando == "E") {
      robotActivo = true;
      Serial.println(">>> ROBOT ACTIVADO");
    } else if (comando == "A") {
      robotActivo = false;
      Serial.println(">>> ROBOT APAGADO");
      pararMotores();
    } else if (comando == "T") {
      testSecuenciaCompleta();
    }
  }
  
  if (robotActivo) {
    L = digitalRead(SensorLeft);
    C = digitalRead(SensorMiddle);
    R = digitalRead(SensorRight);
    
    Serial.print("L:"); Serial.print(L);
    Serial.print(" C:"); Serial.print(C);
    Serial.print(" R:"); Serial.print(R);
    Serial.print(" -> ");
    
    if (L == 0 && C == 0) {
      Serial.println("BUSCAR LÍNEA");
      pararMotores();
    }
    else if (L == 0 && C == 1) {
      Serial.println("AVANZAR");
      avanzar();
    }
    else if (L == 1 && C == 1) {
      // ✅ NUEVA ESTRATEGIA: PARADA COMPLETA + POSICIONAMIENTO + GIRO
      Serial.println("CRUCE DETECTADO - INICIANDO SECUENCIA...");
      secuenciaCruceIzquierda();
    }
    else if (L == 1 && C == 0) {
      Serial.println("GIRO IZQUIERDA EMERGENCIA");
      secuenciaCruceIzquierda();  // Usamos la misma secuencia
    }
  }
  
  delay(50);
}

// ✅ NUEVA SECUENCIA MEJORADA PARA CRUCES
void secuenciaCruceIzquierda() {
  Serial.println(">>> INICIANDO SECUENCIA CRUCE IZQUIERDA");
  
  // FASE 1: PARADA COMPLETA INMEDIATA
  Serial.println("FASE 1: PARADA COMPLETA...");
  pararMotores();
  delay(100);  // ⬅️ PARADA DE 100ms COMO PEDISTE
  
  // FASE 2: AVANCE CORTO DE POSICIONAMIENTO
  Serial.println("FASE 2: AVANCE POSICIONAMIENTO...");
  digitalWrite(IN1, LOW);
  analogWrite(IN2, 150);  // Velocidad reducida para precisión
  digitalWrite(IN3, LOW);
  analogWrite(IN4, 150);
  delay(tiempoPosicionamiento);  // ⬅️ AVANCE CORTO
  
  // FASE 3: PARADA NUEVAMENTE
  Serial.println("FASE 3: PARADA PRE-GIRO...");
  pararMotores();
  delay(100);
  
  // FASE 4: GIRO 90° EXACTO
  Serial.println("FASE 4: GIRO 90°...");
  digitalWrite(IN1, LOW);
  analogWrite(IN2, 160);
  digitalWrite(IN3, HIGH);
  analogWrite(IN4, 0);
  delay(tiempoGiro);  // ⬅️ GIRO DE 300ms
  
  // FASE 5: PARADA FINAL
  Serial.println("FASE 5: PARADA FINAL...");
  pararMotores();
  delay(100);
  
  Serial.println(">>> SECUENCIA CRUCE COMPLETADA");
}

void testSecuenciaCompleta() {
  Serial.println("=== TEST SECUENCIA CRUCE ===");
  secuenciaCruceIzquierda();
  delay(1000);
  Serial.println("=== TEST COMPLETADO ===");
}

void avanzar() {
  digitalWrite(IN1, LOW);
  analogWrite(IN2, velocidad);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, velocidad);
}

void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}