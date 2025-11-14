// ✅ PINES CORREGIDOS
#define ENA 5       // PWM Motor Izquierdo  
#define IN1 A5      // Dirección Motor Izquierdo
#define IN2 A4      // Dirección Motor Izquierdo

#define ENB 3       // PWM Motor Derecho  
#define IN3 A3      // Dirección Motor Derecho
#define IN4 A2      // Dirección Motor Derecho

int pwm = 0;
bool prueba = false;
bool probarIzquierdo = true;
bool probarDerecho = true;

// ✅ NUEVO: PWM MÍNIMO PARA MOVERSE (ajustar según necesidad)
int pwmMinimo = 80;  // Empezar más alto para más potencia

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("✅ MÁXIMA POTENCIA - PWM 3 y 5");
  Serial.println("COMANDOS:");
  Serial.println("  I - Solo IZQUIERDO");
  Serial.println("  D - Solo DERECHO"); 
  Serial.println("  A - AMBOS motores");
  Serial.println("  S - Parar");
  Serial.println("  + - Aumentar potencia inicial");
  Serial.println("  - - Disminuir potencia inicial");
  Serial.print("  Potencia inicial: ");
  Serial.println(pwmMinimo);
  
  parar();
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    
    if (c == 'A' || c == 'a') {
      iniciarPrueba(true, true);
    }
    else if (c == 'I' || c == 'i') {
      iniciarPrueba(true, false);
    }
    else if (c == 'D' || c == 'd') {
      iniciarPrueba(false, true);
    }
    else if (c == 'S' || c == 's') {
      prueba = false;
      parar();
      Serial.println("⛔ PARADO");
    }
    else if (c == '+') {
      pwmMinimo = min(150, pwmMinimo + 10);
      Serial.print("🔊 Potencia mínima aumentada: ");
      Serial.println(pwmMinimo);
    }
    else if (c == '-') {
      pwmMinimo = max(50, pwmMinimo - 10);
      Serial.print("🔉 Potencia mínima disminuida: ");
      Serial.println(pwmMinimo);
    }
  }
  
  static unsigned long last = 0;
  if (prueba && millis() - last >= 500) {
    last = millis();
    
    // ✅ EMPEZAR DESDE PWM MÍNIMO PARA MÁS POTENCIA
    if (pwm == 0) {
      pwm = pwmMinimo;
    } else {
      pwm += 20;  // Incrementos más grandes
    }
    
    if (pwm > 255) {
      prueba = false;
      delay(3000);
      parar();
      Serial.println("✅ FIN - PWM 255");
      return;
    }
    
    aplicarPWM();
    
    Serial.print("🎛️  PWM: ");
    Serial.print(pwm);
    Serial.print("/255 (");
    Serial.print((pwm * 100) / 255);
    Serial.println("%)");
    
    if (probarIzquierdo) Serial.println("   IZQUIERDO: 💨");
    if (probarDerecho) Serial.println("   DERECHO: 💨");
    
    if (pwm == pwmMinimo) {
      Serial.println("   ¿Tiene suficiente potencia?");
    }
  }
  
  delay(50);
}

void aplicarPWM() {
  // Dirección avance
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  
  if (probarIzquierdo) {
    analogWrite(ENA, pwm);
  } else {
    analogWrite(ENA, 0);
  }
  
  if (probarDerecho) {
    analogWrite(ENB, pwm);
  } else {
    analogWrite(ENB, 0);
  }
}

void iniciarPrueba(bool izquierdo, bool derecho) {
  pwm = 0;  // Empezar desde 0, luego saltar a pwmMinimo
  prueba = true;
  probarIzquierdo = izquierdo;
  probarDerecho = derecho;
  
  Serial.println("========================");
  if (izquierdo && derecho) {
    Serial.println("🚀 INICIANDO: AMBOS MOTORES");
  } else if (izquierdo) {
    Serial.println("🚀 INICIANDO: SOLO IZQUIERDO");
  } else {
    Serial.println("🚀 INICIANDO: SOLO DERECHO");
  }
  Serial.print("🔊 Potencia mínima: ");
  Serial.println(pwmMinimo);
  Serial.println("========================");
}

void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}