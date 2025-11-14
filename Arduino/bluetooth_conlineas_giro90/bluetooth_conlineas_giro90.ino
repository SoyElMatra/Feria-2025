// PINES MOTORES
#define ENA 3       // PWM Motor Izquierdo
#define IN1 A5      // Dirección 
#define IN2 A4      // Dirección

#define ENB 5       // PWM Motor Derecho  
#define IN3 A3      // Dirección
#define IN4 A2      // Dirección

// SENSORES DE LÍNEA
#define SensorLeft 10
#define SensorMiddle 11  
#define SensorRight 12

// Variables de sensores
int L, C, R;

// Velocidades
int velocidadGiro = 150;  // Velocidad para giro 90°
int tiempoGiro90 = 450;   // Tiempo para 90 grados
int velocidadLinea = 130; // Velocidad para seguir línea

// Estados
bool robotActivo = false;
bool modoLinea = false;

// Control de decisiones en cruces
unsigned long ultimaDecision = 0;
const int tiempoEntreDecisiones = 1000;




// Variables para mostrar información
String accionActual = "INICIO";
String comandoEjecutando = "---";

void setup() {
  // Configurar pines sensores
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  
  // Configurar pines motores
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("=== ROBOT - SEGUIDOR DE LÍNEA + BLUETOOTH ===");
  Serial.println("Comandos: E=Encender A=Apagar L=Linea M=Manual R=Derecha I=Izquierda");
  Serial.println("SENSORES      | ACCION              | COMANDO");
  Serial.println("--------------|---------------------|-----------------");
  
  // Semilla aleatoria
  randomSeed(analogRead(A0));
  
  pararMotores();
}

void loop() {
  // Leer comandos Bluetooth
  if (Serial.available()) {
    char comando = Serial.read();
    procesarComando(comando);
  }
  
  // Si está activo el modo línea, ejecutar seguimiento
  if (robotActivo && modoLinea) {
    seguirLinea();
  }
  
  delay(30);
}

void procesarComando(char comando) {
  comandoEjecutando = String(comando) + "               ";
  comandoEjecutando = comandoEjecutando.substring(0, 10);
  
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
      activarModoLinea();
      break;
    case 'M':
    case 'm':
      desactivarModoLinea();
      break;
    case 'R':
    case 'r':
      if (robotActivo) {
        accionActual = "GIRO 90° DERECHA";
        mostrarInfo();
        girar90Derecha();
      }
      break;
    case 'I':
    case 'i':
      if (robotActivo) {
        accionActual = "GIRO 90° IZQUIERDA";
        mostrarInfo();
        girar90Izquierda();
      }
      break;
    case 'T':
    case 't':
      if (robotActivo) {
        accionActual = "PRUEBA GIROS";
        mostrarInfo();
        probarGiros();
      }
      break;
    case 'S':
    case 's':
      pararMotores();
      accionActual = "MOTORES DETENIDOS";
      mostrarInfo();
      break;
    case 'D':
    case 'd':
      mostrarDecisiones();
      break;
  }
  comandoEjecutando = "---";
}

// =============================================
// FUNCIONES DE CONTROL
// =============================================

void encenderRobot() {
  robotActivo = true;
  accionActual = "ROBOT ENCENDIDO";
  mostrarInfo();
}

void apagarRobot() {
  robotActivo = false;
  modoLinea = false;
  pararMotores();
  accionActual = "ROBOT APAGADO";
  mostrarInfo();
}

void activarModoLinea() {
  if (robotActivo) {
    modoLinea = true;
    accionActual = "MODO LINEA ON";
    mostrarInfo();
  } else {
    accionActual = "ERROR: APAGADO";
    mostrarInfo();
  }
}

void desactivarModoLinea() {
  modoLinea = false;
  pararMotores();
  accionActual = "MODO LINEA OFF";
  mostrarInfo();
}

// =============================================
// FUNCIÓN PARA MOSTRAR INFORMACIÓN EN UNA LÍNEA
// =============================================

void mostrarInfo() {
  String sensores = "[" + String(L) + " " + String(C) + " " + String(R) + "]        ";
  sensores = sensores.substring(0, 12);
  
  String accion = accionActual + "                  ";
  accion = accion.substring(0, 18);
  
  String comando = comandoEjecutando + "         ";
  comando = comando.substring(0, 12);
  
  Serial.println(sensores + " | " + accion + " | " + comando);
}

// =============================================
// SEGUIDOR DE LÍNEA - CONDICIONES Y MOVIMIENTOS
// =============================================

void seguirLinea() {
  // Leer sensores
  L = digitalRead(SensorLeft);
  C = digitalRead(SensorMiddle);
  R = digitalRead(SensorRight);
  
  // CONTROL PARA EVITAR MÚLTIPLES DETECCIONES
  unsigned long tiempoActual = millis();
  if (tiempoActual - ultimaDecision < tiempoEntreDecisiones) {
    avanzar();
    return;
  }
  
  // CONDICIONES DE LÍNEA Y MOVIMIENTOS
  
  // 1. LÍNEA RECTA - [0 1 0]
  if (L == 0 && C == 1 && R == 0) {
    accionActual = "AVANZAR";
    mostrarInfo();
    avanzar();
  }
  
  // 2. CURVA SUAVE A DERECHA - [0 0 1]
  else if (L == 0 && C == 0 && R == 1) {
    accionActual = "GIRO DER SUAVE";
    mostrarInfo();
    girarDerechaSuave();
  }
  
  // 3. CURVA SUAVE A IZQUIERDA - [1 0 0]
  else if (L == 1 && C == 0 && R == 0) {
    accionActual = "GIRO IZQ SUAVE";
    mostrarInfo();
    girarIzquierdaSuave();
  }
  
  // 4. CRUCE EN T - [1 1 1]
  else if (L == 1 && C == 1 && R == 1) {
    accionActual = "CRUCE T -> DECIDE";
    mostrarInfo();
    decisionAleatoriaCruce("T");
    ultimaDecision = millis();
  }
  
  // 5. CRUCE DERECHA - [0 1 1]
  else if (L == 0 && C == 1 && R == 1) {
    accionActual = "CRUCE DERECHA";
    mostrarInfo();
    girarDerechaSuave();
  }
  
  // 6. CRUCE IZQUIERDA - [1 1 0]
  else if (L == 1 && C == 1 && R == 0) {
    accionActual = "CRUCE IZQUIERDA";
    mostrarInfo();
    girarIzquierdaSuave();
  }
  
  // 7. CRUCE COMPLETO - [1 0 1]
  else if (L == 1 && C == 0 && R == 1) {
    accionActual = "CRUCE COMPLETO";
    mostrarInfo();
    decisionAleatoriaCruce("COMPLETO");
    ultimaDecision = millis();
  }
  
  // 8. PÉRDIDA DE LÍNEA - [0 0 0]
  else if (L == 0 && C == 0 && R == 0) {
    accionActual = "BUSCAR LINEA";
    mostrarInfo();
    buscarLinea();
  }
  
  // 9. CASO POR DEFECTO
  else {
    accionActual = "AVANZAR (DEF)";
    mostrarInfo();
    avanzar();
  }
}

// =============================================
// DECISIONES ALEATORIAS EN CRUCES
// =============================================

void decisionAleatoriaCruce(String tipoCruce) {
  int decision = random(100);
  
  if (tipoCruce == "COMPLETO") {
    // 50% izquierda, 50% derecha
    if (decision < 50) {
      accionActual = "DEC: IZQ 50%";
      mostrarInfo();
      decisionesCompleto_izquierda++;
      secuenciaCruceIzquierda();
    } else {
      accionActual = "DEC: DER 50%";
      mostrarInfo();
      decisionesCompleto_derecha++;
      secuenciaCruceDerecha();
    }
  }
  else if (tipoCruce == "T") {
    // 40% adelante, 30% izquierda, 30% derecha
    if (decision < 40) {
      accionActual = "DEC: ADEL 40%";
      mostrarInfo();
      decisionesT_adelante++;
      avanzar();
      delay(300);
    }
    else if (decision < 70) {
      accionActual = "DEC: IZQ 30%";
      mostrarInfo();
      decisionesT_izquierda++;
      secuenciaCruceIzquierda();
    }
    else {
      accionActual = "DEC: DER 30%";
      mostrarInfo();
      decisionesT_derecha++;
      secuenciaCruceDerecha();
    }
  }
}

void secuenciaCruceIzquierda() {
  accionActual = "GIRO IZQ 90°";
  mostrarInfo();
  pararMotores();
  delay(150);
  
  digitalWrite(IN1, HIGH);   // Izquierda atrás
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);    // Derecha adelante
  digitalWrite(IN4, HIGH);
  
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  delay(tiempoGiro90);
  
  pararMotores();
  delay(100);
}

void secuenciaCruceDerecha() {
  accionActual = "GIRO DER 90°";
  mostrarInfo();
  pararMotores();
  delay(150);
  
  digitalWrite(IN1, LOW);    // Izquierda adelante
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);   // Derecha atrás
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  delay(tiempoGiro90);
  
  pararMotores();
  delay(100);
}

// =============================================
// MOVIMIENTOS PARA SEGUIDOR DE LÍNEA
// =============================================

void avanzar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidadLinea);
  analogWrite(ENB, velocidadLinea);
}

void girarDerechaSuave() {
  // Motor izquierdo: velocidad normal
  // Motor derecho: velocidad reducida
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidadLinea);
  analogWrite(ENB, velocidadLinea * 0.4);
}

void girarIzquierdaSuave() {
  // Motor izquierdo: velocidad reducida
  // Motor derecho: velocidad normal
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidadLinea * 0.4);
  analogWrite(ENB, velocidadLinea);
}

void buscarLinea() {
  // Giro en el lugar para buscar línea
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 120);
  analogWrite(ENB, 120);
  delay(200);
}

// =============================================
// GIROS 90 GRADOS (para Bluetooth)
// =============================================

void girar90Izquierda() {
  secuenciaCruceIzquierda();
}

void girar90Derecha() {
  secuenciaCruceDerecha();
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
  accionActual = "PRUEBA COMPLETADA";
  mostrarInfo();
}

void mostrarDecisiones() {
  Serial.println("=== 📊 ESTADÍSTICAS DE DECISIONES ===");
  
  int totalT = decisionesT_adelante + decisionesT_izquierda + decisionesT_derecha;
  Serial.println("CRUCES EN T:");
  Serial.print("Total: "); Serial.println(totalT);
  if (totalT > 0) {
    Serial.print("Adelante: "); Serial.print(decisionesT_adelante); 
    Serial.print(" ("); Serial.print((decisionesT_adelante * 100) / totalT); Serial.println("%)");
    Serial.print("Izquierda: "); Serial.print(decisionesT_izquierda);
    Serial.print(" ("); Serial.print((decisionesT_izquierda * 100) / totalT); Serial.println("%)");
    Serial.print("Derecha: "); Serial.print(decisionesT_derecha);
    Serial.print(" ("); Serial.print((decisionesT_derecha * 100) / totalT); Serial.println("%)");
  }
  
  int totalCompleto = decisionesCompleto_izquierda + decisionesCompleto_derecha;
  Serial.println("CRUCES COMPLETOS:");
  Serial.print("Total: "); Serial.println(totalCompleto);
  if (totalCompleto > 0) {
    Serial.print("Izquierda: "); Serial.print(decisionesCompleto_izquierda);
    Serial.print(" ("); Serial.print((decisionesCompleto_izquierda * 100) / totalCompleto); Serial.println("%)");
    Serial.print("Derecha: "); Serial.print(decisionesCompleto_derecha);
    Serial.print(" ("); Serial.print((decisionesCompleto_derecha * 100) / totalCompleto); Serial.println("%)");
  }
  Serial.println("=================================");
}