const int SENSOR_COUNT = 3;
const int SAMPLES_PER_SECOND = 10;
const int DETECTION_THRESHOLD = 5;  // Mínimo de detecciones consecutivas
unsigned long lastSampleTime = 0;

struct Sensor {
  const char* name;
  unsigned int pin;
  int detectionCount;
  bool lastReportedState;
  unsigned long lastDetectionTime;
};

Sensor sensors[SENSOR_COUNT] = {
  {"Izquierda", 7, 0, false, 0},
  {"Centro",    8, 0, false, 0},
  {"Derecha",   9, 0, false, 0}
};

// Variables para etiquetado
bool recording = false;
char currentLabel = '0';  // '1'=válido, '0'=inválido
unsigned long lastSerialOutput = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Esperar conexión serial
  
  Serial.println("Sistema de Captura Mejorado para ML");
  Serial.println("----------------------------------");
  Serial.println("Instrucciones:");
  Serial.println("1. Presione 's' para iniciar/detener grabación");
  Serial.println("2. Durante la grabación:");
  Serial.println("   - '1': Marcar muestra como VÁLIDA");
  Serial.println("   - '0': Marcar muestra como INVÁLIDA");
  Serial.println("3. Encabezado CSV: sensor1,sensor2,sensor3,label");
  
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensors[i].pin, INPUT);
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  // Manejo de comandos seriales
  if (Serial.available()) {
    char command = Serial.read();
    
    if (command == 's') {
      recording = !recording;
      Serial.print("\nGrabación: ");
      Serial.println(recording ? "ACTIVA" : "DETENIDA");
      if (recording) {
        Serial.println("sensor1,sensor2,sensor3,label");
        // Reiniciar estados al comenzar grabación
        for (int i = 0; i < SENSOR_COUNT; i++) {
          sensors[i].detectionCount = 0;
          sensors[i].lastReportedState = false;
        }
      }
    }
    else if (recording && (command == '0' || command == '1')) {
      currentLabel = command;
      Serial.print("Etiqueta cambiada a: ");
      Serial.println(currentLabel);
    }
  }

  // Lectura continua de sensores con filtro de persistencia
  for (int i = 0; i < SENSOR_COUNT; i++) {
    bool currentState = (digitalRead(sensors[i].pin) == LOW);
    
    if (currentState) {
      sensors[i].detectionCount = min(DETECTION_THRESHOLD, sensors[i].detectionCount + 1);
      sensors[i].lastDetectionTime = currentTime;
    } else {
      // Decrementar solo si ha pasado tiempo desde la última detección
      if (currentTime - sensors[i].lastDetectionTime > 50) {
        sensors[i].detectionCount = max(0, sensors[i].detectionCount - 1);
      }
    }
    
    // Determinar estado actual basado en conteo
    bool activeState = (sensors[i].detectionCount >= DETECTION_THRESHOLD);
    
    // Solo reportar cambios de estado
    if (activeState != sensors[i].lastReportedState) {
      sensors[i].lastReportedState = activeState;
    }
  }

  // Captura de muestras periódicas para ML
  if (recording && currentTime - lastSampleTime > 1000/SAMPLES_PER_SECOND) {
    lastSampleTime = currentTime;
    
    // Reportar estados actuales
    for (int i = 0; i < SENSOR_COUNT; i++) {
      Serial.print(sensors[i].lastReportedState ? "1" : "0");
      if (i < SENSOR_COUNT - 1) Serial.print(",");
    }
    
    Serial.print(",");
    Serial.println(currentLabel);
  }

  // Debug visual cada 500ms
  if (currentTime - lastSerialOutput > 500) {
    lastSerialOutput = currentTime;
    Serial.print("Estados: [");
    for (int i = 0; i < SENSOR_COUNT; i++) {
      Serial.print(sensors[i].lastReportedState ? "1" : "0");
      Serial.print("(");
      Serial.print(sensors[i].detectionCount);
      Serial.print(")");
      if (i < SENSOR_COUNT - 1) Serial.print("|");
    }
    Serial.println("]");
  }
  
  delay(5);  // Pequeña pausa para estabilidad
}