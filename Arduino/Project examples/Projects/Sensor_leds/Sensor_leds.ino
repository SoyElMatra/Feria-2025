#include "SR04.h"
#define TRIG_PIN A1
#define ECHO_PIN A0 
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
int led[]={10,11,2,4,3,5,6,7,8,9}; // defino un array para los puertos donde están conectados los leds en secuencia.
int dis[11]; // defino un array para distintas distancias (11 en total)
int dd;
int jdel1=30; // delays para el jueguito cuando la distancia es alta
int jdel2=30;
void setup() {
// arma un bucle y asigna al array los valores 10, 20, 30 hasta el 100.
  for (int i = 0; i <= 10; i++){ 
    dis[i] = (i+1)*10 ;
  }
// Usando el array de los leds ponemos el modo en los pines y apagamos cualquiera que pudiera estar encendido
  for (int i = 0; i <= 9; i++){
      pinMode(led[i], OUTPUT);
      digitalWrite(led[i], LOW);
    
  }

Serial.begin(9600);
delay(1);
}

void loop() {
  dd=sr04.Distance(); // esto lee la distancia del sensor y la pone en la variable dd.
  admled(dd); // esto va a una funcion para encender los leds de acuerdo a la distancia.

}




void admled(int dist){ // esta es la funcion para administrar el encendido de los leds.
  Serial.println(dist); 

//armamos un bucle para comparar la distancia proveniente del sensor con las distancias definidas en el array dis[]
  for (int i = 10; i >= 0; i--){
    // compara la distancia con la correspondiente del array (si = 10 y la dist es mayor a la máxima va a la funcion jugar para mostrar que está en standby.
    if (dist > dis[i]){ 
      if (i == 10){ 
        jugar(); // esto va a otra funcion mas pero para hacer un bucle en caso de que detecte una distancia muy larga o ni llegue a detectarla.
        return;
      }else{ // de otra manera si i está entre 0 y 9 enciende o apaga el led correspondiente.
        digitalWrite(led[i], HIGH);
      }
    }else{
      digitalWrite(led[i], LOW);
    }
  }
  return;
}

void jugar(){
  // enciende los leds uno a uno
  for (int ii=0; ii<=9;ii++){
    digitalWrite(led[ii], HIGH);
    delay(jdel1);  
  }
  // apaga los leds uno a uno
  for (int ii=0; ii<=9;ii++){
    digitalWrite(led[ii], LOW);
    delay(jdel2);  
    
  }
return;
}
