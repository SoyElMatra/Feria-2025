// PINES CORREGIDOS CON ENA/ENB
#define ENA 3       // PWM Motor Izquierdo - PIN 3
#define IN1 A5      // Dirección 
#define IN2 A4      // Dirección

#define ENB 5       // PWM Motor Derecho - PIN 5  
#define IN3 A3      // Dirección
#define IN4 A2      // Dirección

// SENSORES
#define SensorLeft 10
#define SensorMiddle 11  
#define SensorRight 12

// Variables de sensores
int L, C, R;

// Variables de control
int velocidad = 150;
int tiempoGiro = 320;  // Ajustado para giro directo

// Control por serial
bool robotActivo = false;


unsigned long ultimaDeteccion = 0;
const int tiempoMinEntreDecisiones = 1000;

// Semilla para aleatoriedad
unsigned long semillaAleatoria = 0;

// Registro de decisiones (se mantiene)

void setup() {
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("=== PACMAN ROBOT - DETECCIÓN INSTANTÁNEA ===");
  Serial.println("Comandos:");
  Serial.println("'E' - Empezar modo normal");
  Serial.println("'A' - Apagar");
  Serial.println("'T' - Probar CRUCE T (10 decisiones)");
  Serial.println("'C' - Probar CRUCE COMPLETO (10 decisiones)"); 
  Serial.println("'R' - Ver registros");
  Serial.println("'L' - Limpiar registros");
  Serial.println("'V' - Ajustar velocidad: " + String(velocidad));
  
  semillaAleatoria = analogRead(A0);
  randomSeed(semillaAleatoria);
  
  pararMotores();
}

void loop() {
  if (Serial.available()) {
    char comando = Serial.read();
    
    if (comando == 'E' || comando == 'e') {
      robotActivo = true;
      Serial.println(">>> ROBOT ACTIVADO");
    } else if (comando == 'A' || comando == 'a') {
      robotActivo = false;
      Serial.println(">>> ROBOT APAGADO");
      pararMotores();
    } else if (comando == 'T' || comando == 't') {
      probarCruceT();
    } else if (comando == 'C' || comando == 'c') {
      probarCruceCompleto();
    } else if (comando == 'R' || comando == 'r') {
      mostrarRegistrosCompletos();
    } else if (comando == 'L' || comando == 'l') {
      limpiarRegistros();
    } else if (comando == 'V' || comando == 'v') {
      ajustarVelocidad();
    } else if (comando == '+' || comando == '-') {
      if (comando == '+') {
        velocidad = min(255, velocidad + 10);
      } else {
        velocidad = max(80, velocidad - 10);
      }
      Serial.println("Nueva velocidad: " + String(velocidad));
    }
  }
  
  if (robotActivo) {
    // ✅ LECTURA SIMPLE - SIN MÚLTIPLES LECTURAS
    L = digitalRead(SensorLeft);
    C = digitalRead(SensorMiddle);
    R = digitalRead(SensorRight);
    
    Serial.print("L:"); Serial.print(L);
    Serial.print(" C:"); Serial.print(C);
    Serial.print(" R:"); Serial.print(R);
    Serial.print(" -> ");
    
    // ✅ EVITAR MÚLTIPLES DETECCIONES RÁPIDAS
    unsigned long tiempoActual = millis();
    if (tiempoActual - ultimaDeteccion < tiempoMinEntreDecisiones) {
      Serial.println("AVANZAR (Esperando entre decisiones)");
      avanzar();
      delay(50);
      return;
    }
    
    // ✅ LÓGICA INSTANTÁNEA - SIN CONFIRMACIÓN MÚLTIPLE
    if (L == 0 && C == 1 && R == 0) {
      Serial.println("AVANZAR");
      avanzar();
    }
    // ✅ DETECCIÓN DIRECTA DE CRUCES
    else if (L == 1 && C == 1 && R == 0) {
      Serial.println("CRUCE IZQUIERDA DETECTADO");
      decisionAleatoriaCruce("IZQUIERDA");
      ultimaDeteccion = millis();
    }
    else if (L == 0 && C == 1 && R == 1) {
      Serial.println("CRUCE DERECHA DETECTADO");
      decisionAleatoriaCruce("DERECHA");
      ultimaDeteccion = millis();
    }
    else if (L == 1 && C == 1 && R == 1) {
      Serial.println("CRUCE EN T DETECTADO");
      decisionAleatoriaCruce("T");
      ultimaDeteccion = millis();
    }
    else if (L == 1 && C == 0 && R == 1) {
      Serial.println("CRUCE COMPLETO DETECTADO");
      decisionAleatoriaCruce("COMPLETO");
      ultimaDeteccion = millis();
    }
    else if (L == 0 && C == 0 && R == 0) {
      Serial.println("BUSCAR LÍNEA");
      // buscarLinea(); // Opcional: descomentar si necesitas
    }
    else {
      Serial.println("AVANZAR (Patrón no crítico)");
      avanzar();
    }
  }
  
  delay(30);
}

// ✅ SECUENCIAS DE GIRO SIN AVANCE PREVIO
void secuenciaCruceIzquierda() {
  Serial.println(">>> GIRO IZQUIERDA 90° - DIRECTO");
  pararMotores();
  delay(150);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
  delay(tiempoGiro);
  
  pararMotores();
  delay(100);
}

void secuenciaCruceDerecha() {
  Serial.println(">>> GIRO DERECHA 90° - DIRECTO");
  pararMotores();
  delay(150);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
  delay(tiempoGiro);
  
  pararMotores();
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

void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}



// Las funciones de decisión aleatoria y registros se mantienen igual...
void decisionAleatoriaCruce(String tipoCruce) {
  int decision = random(100);
  Serial.print("Aleatorio: "); Serial.print(decision);
  Serial.print(" -> ");
  
  if (tipoCruce == "COMPLETO") {
    if (decision < 50) {
      Serial.println("GIRO IZQUIERDA (50%)");
      registrarDecisionCompleto("IZQUIERDA");
      secuenciaCruceIzquierda();
    } else {
      Serial.println("GIRO DERECHA (50%)");
      registrarDecisionCompleto("DERECHA");
      secuenciaCruceDerecha();
    }
  }
  else if (tipoCruce == "IZQUIERDA") {
      avanzar();
      delay(50);
      Serial.println("GIRO IZQUIERDA");
      secuenciaCruceIzquierda();
  }
  else if (tipoCruce == "DERECHA") {
      avanzar();
      delay(50);
      Serial.println("GIRO DERECHA");
      secuenciaCruceDerecha();
  }
  else if (tipoCruce == "CRUCE EN T DETECTADO"){
    if (decision < 20) {
      Serial.println("AVANZAR (40%)");
      registrarDecisionT("ADELANTE");
      avanzar();
      delay(150);
    }
    else if (decision < 60) {
      Serial.println("GIRO IZQUIERDA (30%)");
      registrarDecisionT("IZQUIERDA");
      secuenciaCruceIzquierda();
    }
    else {
      Serial.println("GIRO DERECHA (30%)");
      registrarDecisionT("DERECHA");
      secuenciaCruceDerecha();
    }
  }
}

