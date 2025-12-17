// Pines de sensores
#define SensorLeft 12
#define SensorMiddle 6  
#define SensorRight 10
#define SensorBack 4

int izq = 0;
int der = 0;
int cen = 0;
int back = 0;

// Variables para monitoreo periódico
unsigned long ultimoReporte = 0;
const unsigned long INTERVALO_REPORTE = 800; // 2 segundos

void setup() {
  // Configurar sensores
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);
  pinMode(SensorBack, INPUT);
  
  // Iniciar comunicación serial
  Serial.begin(9600);
  Serial.println("=== MONITOR DE SENSORES ===");
  Serial.println("Leyendo sensores cada 2 segundos");
  Serial.println("1 = sobre línea negra, 0 = fuera de línea");
  Serial.println("---------------------------");
}

void loop() {
  // Leer sensores e invertir la lógica
  izq = digitalRead(SensorLeft);  // Invertir: línea=1, sin línea=0
  der = digitalRead(SensorRight); // Invertir: línea=1, sin línea=0
  cen = digitalRead(SensorMiddle); // Invertir: línea=1, sin línea=0
  back = digitalRead(SensorBack); 
  
  // Mostrar estado de sensores solo cada 2 segundos
  if (millis() - ultimoReporte >= INTERVALO_REPORTE) {
    Serial.print("Tiempo: ");
    Serial.print(millis() / 1000);
    Serial.print("s - Sensores → I:");
    Serial.print(izq);
    Serial.print(" C:"); 
    Serial.print(cen);
    Serial.print(" D:");
    Serial.print(der);
    Serial.print(" D:");
    Serial.println(back);
    ultimoReporte = millis();
  }
  
  delay(100); // Pequeña pausa
}