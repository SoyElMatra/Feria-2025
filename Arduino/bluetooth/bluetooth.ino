// PINES MOTORES
#define ENA 3       // PWM Motor Izquierdo
#define IN1 A5      // Dirección 
#define IN2 A4      // Dirección

#define ENB 5       // PWM Motor Derecho  
#define IN3 A3      // Dirección
#define IN4 A2      // Dirección

// Velocidades
int velocidadGiro = 130;  // Velocidad para giro
int tiempoGiro90 = 450;   // Tiempo para 90 grados

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("=== ROBOT - CONTROL BLUETOOTH ===");
  Serial.println("Comandos:");
  Serial.println("L - Girar 90° izquierda");
  Serial.println("R - Girar 90° derecha");
  Serial.println("T - Probar ambos giros");
  Serial.println("S - Detener motores");
  
  pararMotores();
}

void loop() {
  if (Serial.available()) {
    char comando = Serial.read();
    
    if (comando == 'L' || comando == 'l') {
      Serial.println(">>> GIRANDO 90° IZQUIERDA");
      girar90Izquierda();
    } 
    else if (comando == 'R' || comando == 'r') {
      Serial.println(">>> GIRANDO 90° DERECHA");
      girar90Derecha();
    }
    else if (comando == 'T' || comando == 't') {
      Serial.println(">>> PROBANDO AMBOS GIROS");
      probarGiros();
    }
    else if (comando == 'S' || comando == 's') {
      pararMotores();
      Serial.println("🛑 MOTORES DETENIDOS");
    }
  }
}

// =============================================
// FUNCIONES DE GIRO 90 GRADOS SOBRE SU EJE
// =============================================

void girar90Izquierda() {
  // Motor izquierdo: ATRÁS
  // Motor derecho: ADELANTE
  
  digitalWrite(IN1, LOW);   // Izquierda atrás
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);    // Derecha adelante
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  
  delay(tiempoGiro90);
  
  pararMotores();
  Serial.println("✅ Giro 90° izquierda completado");
}

void girar90Derecha() {
  // Motor izquierdo: ADELANTE
  // Motor derecho: ATRÁS
  
  digitalWrite(IN1, HIGH);    // Izquierda adelante
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Derecha atrás
  digitalWrite(IN4, HIGH);
  
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  
  delay(tiempoGiro90);
  
  pararMotores();
  Serial.println("✅ Giro 90° derecha completado");
}

// =============================================
// FUNCIONES AUXILIARES
// =============================================

void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void probarGiros() {
  delay(1000);
  girar90Izquierda();
  delay(2000);
  girar90Derecha();
  delay(2000);
  girar90Derecha();
  delay(2000);
  girar90Izquierda();
  Serial.println(">>> PRUEBA DE GIROS COMPLETADA");
}
