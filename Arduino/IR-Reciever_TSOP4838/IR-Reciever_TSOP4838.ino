const int TIMEOUT = 75; // Tiempo de espera en milisegundos
const int SENSOR_COUNT = 3; // Número de sensores

struct Sensor {
  const char* name; // Nombre del sensor
  unsigned int pin; // Pin del sensor
  bool lastState; // Último estado del sensor
  bool signalPresent; // Estado actual del sensor
  unsigned long lastChangeTime; // Última vez que cambió el estado
};

Sensor sensors[SENSOR_COUNT] = {
  {"Izquierda", 9, HIGH, false, 0},
  {"Centro",    7, HIGH, false, 0},
  {"Derecha",   8, HIGH, false, 0}
};

void setup() {
  Serial.begin(9600); // Iniciar la comunicación serial
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensors[i].pin, INPUT); // Configurar el pin del sensor como entrada
    sensors[i].lastState = digitalRead(sensors[i].pin); // Leer el estado inicial
  }
}

void loop() {
  unsigned long currentTime = millis(); // Obtener el tiempo actual
  
  for (int i = 0; i < SENSOR_COUNT; i++) {
    Sensor &s = sensors[i];
    bool currentState = digitalRead(s.pin); // Leer el estado actual del sensor
    
    // Si el estado actual es diferente al último estado
    if (currentState != s.lastState) {
      s.lastChangeTime = currentTime; // Actualizar el tiempo de cambio
      s.lastState = currentState; // Actualizar el último estado
    }
    
    // Verificar si el tiempo desde el último cambio es menor o igual al timeout
    bool newState = (currentTime - s.lastChangeTime) <= TIMEOUT;
    
    // Si el nuevo estado es diferente al estado actual del sensor
    if (newState != s.signalPresent) {
      s.signalPresent = newState; // Actualizar el estado del sensor
      
      // Imprimir el estado del sensor
      Serial.print(s.name);
      Serial.print(": ");
      Serial.println(s.signalPresent ? "1" : "0");
    }
  }
}
