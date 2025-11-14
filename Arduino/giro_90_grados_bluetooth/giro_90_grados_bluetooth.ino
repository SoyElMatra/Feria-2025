// PINES MOTORES
#define ENA 3       // PWM Motor Izquierdo
#define IN1 A5      // Dirección 
#define IN2 A4      // Dirección

#define ENB 5       // PWM Motor Derecho  
#define IN3 A3      // Dirección
#define IN4 A2      // Dirección

// Velocidades
int velocidadGiro = 150;  // Velocidad para giro
int tiempoGiro90 = 450;   // Tiempo para 90 grados (ajustar según robot)

// Estados
bool robotActivo = false;

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
  Serial.println("E - Encender robot");
  Serial.println("A - Apagar robot");
  Serial.println("L - Girar 90° izquierda");
  Serial.println("R - Girar 90° derecha");
  Serial.println("T - Probar ambos giros");
  Serial.println("C - Calibrar giro");
  Serial.println("S - Detener motores");
  Serial.println("1 - Velocidad baja (120)");
  Serial.println("2 - Velocidad media (150)");
  Serial.println("3 - Velocidad alta (180)");
  
  pararMotores();
}

void loop() {
  if (Serial.available()) {
    char comando = Serial.read();
    procesarComando(comando);
  }
}

void procesarComando(char comando) {
  switch(comando) {
    case 'E':
    case 'e':
      encenderRobot();
      break;
    case 'A':
    case 'a':
      apagarRobot();
      break;
    case 'L':
    case 'l':
      if (robotActivo) {
        Serial.println(">>> GIRANDO 90° IZQUIERDA");
        girar90Izquierda();
      } else {
        Serial.println("❌ Robot apagado. Envía 'E' para encender");
      }
      break;
    case 'R':
    case 'r':
      if (robotActivo) {
        Serial.println(">>> GIRANDO 90° DERECHA");
        girar90Derecha();
      } else {
        Serial.println("❌ Robot apagado. Envía 'E' para encender");
      }
      break;
    case 'T':
    case 't':
      if (robotActivo) {
        Serial.println(">>> PROBANDO AMBOS GIROS");
        probarGiros();
      } else {
        Serial.println("❌ Robot apagado. Envía 'E' para encender");
      }
      break;
    case 'C':
    case 'c':
      if (robotActivo) {
        calibrarGiro();
      } else {
        Serial.println("❌ Robot apagado. Envía 'E' para encender");
      }
      break;
    case 'S':
    case 's':
      pararMotores();
      Serial.println("🛑 MOTORES DETENIDOS");
      break;
    case '1':
      velocidadGiro = 120;
      Serial.println("🎚️ Velocidad BAJA: 120");
      break;
    case '2':
      velocidadGiro = 150;
      Serial.println("🎚️ Velocidad MEDIA: 150");
      break;
    case '3':
      velocidadGiro = 180;
      Serial.println("🎚️ Velocidad ALTA: 180");
      break;
    default:
      Serial.println("❌ Comando no reconocido");
      break;
  }
}

// =============================================
// FUNCIONES DE CONTROL
// =============================================

void encenderRobot() {
  robotActivo = true;
  Serial.println("✅ ROBOT ENCENDIDO - Listo para girar");
}

void apagarRobot() {
  robotActivo = false;
  pararMotores();
  Serial.println("🔴 ROBOT APAGADO");
}

// =============================================
// FUNCIONES DE GIRO 90 GRADOS SOBRE SU EJE
// =============================================

void girar90Izquierda() {
  // Motor izquierdo: ATRÁS
  // Motor derecho: ADELANTE
  // Esto hace que gire sobre su eje central
  
  digitalWrite(IN1, HIGH);   // Izquierda atrás
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);    // Derecha adelante
  digitalWrite(IN4, HIGH);
  
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  
  delay(tiempoGiro90);  // Mantener el giro por el tiempo calculado
  
  pararMotores();
  Serial.println("✅ Giro 90° izquierda completado");
}

void girar90Derecha() {
  // Motor izquierdo: ADELANTE
  // Motor derecho: ATRÁS
  // Esto hace que gire sobre su eje central
  
  digitalWrite(IN1, LOW);    // Izquierda adelante
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);   // Derecha atrás
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  
  delay(tiempoGiro90);  // Mantener el giro por el tiempo calculado
  
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

// =============================================
// FUNCIÓN PARA CALIBRAR TIEMPO DE GIRO
// =============================================

void calibrarGiro() {
  Serial.println("=== 🔧 CALIBRACIÓN DE GIRO ===");
  Serial.println("Girando 90° derecha...");
  
  unsigned long inicio = millis();
  girar90Derecha();
  unsigned long fin = millis();
  
  int tiempoReal = fin - inicio;
  Serial.print("Tiempo real del giro: ");
  Serial.print(tiempoReal);
  Serial.println(" ms");
  
  Serial.print("¿Fue un giro de 90° exacto? (S/N): ");
  
  // Esperar respuesta por Bluetooth
  unsigned long tiempoEspera = millis();
  while (millis() - tiempoEspera < 10000) { // Timeout de 10 segundos
    if (Serial.available()) {
      char respuesta = Serial.read();
      if (respuesta == 'S' || respuesta == 's') {
        tiempoGiro90 = tiempoReal;
        Serial.println("✅ Tiempo de giro calibrado: " + String(tiempoGiro90) + " ms");
        return;
      } else if (respuesta == 'N' || respuesta == 'n') {
        Serial.println("❌ Ajusta manualmente 'tiempoGiro90' en el código");
        return;
      }
    }
    delay(100);
  }
  
  Serial.println("⏰ Timeout - No se recibió respuesta");
}