const int TIMEOUT = 75;
const int SENSOR_COUNT = 3;

struct Sensor {
  const char* name;
  unsigned int pin;
  bool lastState;
  bool signalPresent;
  unsigned long lastChangeTime;
};

Sensor sensors[SENSOR_COUNT] = {
  {"Izquierda", 7, HIGH, false, 0},
  {"Centro",    8, HIGH, false, 0},
  {"Derecha",   9, HIGH, false, 0}
};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensors[i].pin, INPUT);
    sensors[i].lastState = digitalRead(sensors[i].pin);
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  for (int i = 0; i < SENSOR_COUNT; i++) {
    Sensor &s = sensors[i];
    bool currentState = digitalRead(s.pin);
    
    if (currentState != s.lastState) {
      s.lastChangeTime = currentTime;
      s.lastState = currentState;
    }
    
    bool newState = (currentTime - s.lastChangeTime) <= TIMEOUT;
    
    if (newState != s.signalPresent) {
      s.signalPresent = newState;
      
      Serial.print(s.name);
      Serial.print(": ");
      Serial.println(s.signalPresent ? "1" : "0");
    }
  }
}