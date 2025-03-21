#include "SR04.h"
#define TRIG_PIN A4
#define ECHO_PIN A3 
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
int led[]={0,1,5,3,4,2,9,10,11}; // defino un array para los puertos donde están conectados los leds en secuencia.
int dis[11]; // defino un array para distintas distancias (11 en total)
int dd;
int jdel1=5; // delays para el jueguito cuando la distancia es alta
int aR=50;
int aG=100;
int aB=150;
int sw;
int dmax=100; // limite maximo del delay
int dmin=0; // limite minimo del delay
void setup() {
// arma un bucle y asigna al array los valores 10, 20, 30 hasta el 100.
  for (int i = 0; i <= 10; i++){ 
    dis[i] = (i+1)*10 ;
  }
// Usando el array de los leds ponemos el modo en los pines y apagamos cualquiera que pudiera estar encendido
  for (int i = 0; i <= 9; i++){
      pinMode(led[i], OUTPUT);
      pinMode(1, OUTPUT);
      pinMode(0, OUTPUT);
      digitalWrite(led[i], LOW);
    
  }

Serial.begin(9600);
delay(1);
}

void loop() {

  if (Serial.available()){
    switch (Serial.read()){
      
      case '+':
        switch (sw){
          
          case 0:
            jdel1=jdel1+10;
          break;
        }
        
      break;
      
      case '-':
          switch (sw){
          case 0:
            jdel1=jdel1-10;
          break;
          

        }
    }
    if (jdel1 > dmax){
      jdel1=dmax;
    }else if (jdel1<dmin) {
      jdel1=dmin;
    }
    Serial.print ("delay1: ");
    Serial.println (jdel1);

    
  }
  
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
        for (aR; aR >= 0; i++){
        analogWrite(11, aR);
        if(aR>=255){
          aR=0;
        }
        }
        for (aG; aG >= 0; i++){
        analogWrite(9, aG);
        if(aG>=255){
          aG=0;
        }
        }
        for (aB; aB >= 0; i++){
        analogWrite(10, aB);
        if(aB>=255){
          aB=0;
        }
        }
        
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

