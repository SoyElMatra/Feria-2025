#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  BT.begin(9600); // Modo AT por defecto

  Serial.println("Listo. Escribí comandos AT abajo:");
  Serial.println("Por ejemplo: AT+ADDR?");
}

void loop() {
  // Enviar datos desde Serial a BT
  if (Serial.available()) {
    char c = Serial.read();
    BT.write(c);
  }

  // Enviar datos desde BT a Serial
  if (BT.available()) {
    char c = BT.read();
    Serial.write(c);
  }
}
