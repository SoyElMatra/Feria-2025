#include "SR04.h"
#include "colores.h"
#define TRIG_PIN A4
#define ECHO_PIN A3
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
int ccp=30000; // Esta es la velocidad del cambio (a mayor valor cambia más despacito, con 30000 es un cambio suave y con 1 cambia instantaneamente)
const int pr=11; // Pin del RED en el led RGB
const int pg=9; // Pin del GREEN en el led RGB
const int pb=10; // Pin del BLUE en el led RGB
int LDR=A0;
const int retraso=500; // Este es el valor del delay para permanecer en cada color antes de comenzar a cambiar al otro.

const int nc=10; // Cantidad de colores en el array o arreglo, acordate que empiezan del cero, es decir que 10 en esta constante indica que en realidad hay 11 colores.

// aca defino cada uno de los colores, acordate que si agregas o quitas colores tenes que modificar la constante nc definida antes.
color clr[]={{10,10,10},{255,255,255},{0,255,0},{0,0,255},{255,0,0},{255,255,0},{255,0,255},{0,255,255},{255,128,0},{0,121,126},{128,255,0}};
int b;
int c = 0;
int d;



void setup(){
  Serial.begin(9600);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(pr, OUTPUT);
  pinMode(pg, OUTPUT);
  pinMode(pb, OUTPUT);
  pinMode(LDR, INPUT);
}

void loop(){
  a=sr04.Distance();
  Serial.print(a);
  Serial.println("cm");
  if(a>=100){
    led();
  }
  if(a<100 && a>10){
    ledsRAG();
  }
  delay(10);
}
  void led(){
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  int ini,fin;

  for (ini=0;ini<=nc;ini++){


/* 
****************************************************************************
*** LEETE BIEN ESTA EXPLICACION PORQUE TE VA A SER MUY UTIL EN EL FUTURO ***
****************************************************************************

En la siguiente línea usamos el operador %, este simbolo realiza el calculo del resto de una division por ejemplo:
1 % 3 = 1 Porque 1/3 es =0 pero queda como resto el 1. Asi es que 
2 % 3 = 2
3 % 3 = 0 porque en este caso el resultado de la division es 1 pero el resto seria cero y por ejemplo
5 % 3 = 2 
6 % 3 = 0

Esta funcion es muy util en programacion para muchas cosas. En este caso lo que hace es hacer que cuando se completó el arreglo de colores vuelva al primer color definido
o sea, cuando ini vale 1, fin vale 2, y se va incrementando hasta que ini vale nc que es el final del arreglo, entonces fin vale 0.
Supongamos que nc vale 10, entonces cuando:
ini=9 || fin = ((9+1) % (10+1)) || entonces esto es 10 % 11 y como 10 dividido 11 es cero sobran 10, entonces fin=10. Pero que pasa cuando ini vale 10? veamos...

ini=10 || fin = ((10+1) % (10+1)) || entonces esto es 11 % 11 y como 11 dividido 11 es 1 el resto es 0, entonces fin=0, y si ini=10 y fin=0 lo que estamos haciendo
es que cambie nuevamente al primer color del arreglo y asi de seguido.

*/
    fin=((ini+1) % (nc+1)); 
  


    Serial.print ("\nNumero de color (Inicio|fin): (");
    Serial.print(ini);
    Serial.print ("|");
    Serial.print(fin);
    Serial.print (") - - - - Color ini (R|G|B): (");
    Serial.print (clr[ini].red);
    Serial.print ("|");
    Serial.print (clr[ini].gre);
    Serial.print ("|");
    Serial.print (clr[ini].blu);
    Serial.print (") - Color fin: (");
    Serial.print (clr[fin].red);
    Serial.print ("|");
    Serial.print (clr[fin].gre);
    Serial.print ("|");
    Serial.print (clr[fin].blu);
    Serial.print (").");*/

    CambiaColor(clr[ini],clr[fin],ccp,pr,pg,pb);
    delay(retraso);
  a=sr04.Distance();
  Serial.print(a);
  Serial.println("cm");
  if(a>10 && a<100){
    return;
  }}}
 
  void ledsRAG(){
if(a<10 ){
  led();
}
if(a>=100){
  led();
}
if(a>=10){
  digitalWrite(0, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(1, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(2, LOW);
  digitalWrite(pr, LOW);
  digitalWrite(pg, LOW);
  digitalWrite(pb, LOW);
}
if(a>=20){
  digitalWrite(0, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(1, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(2, LOW);
    digitalWrite(pr, LOW);
  digitalWrite(pg, LOW);
  digitalWrite(pb, LOW);
}
if(a>=30){
  digitalWrite(0, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(2, LOW);
    digitalWrite(pr, LOW);
  digitalWrite(pg, LOW);
  digitalWrite(pb, LOW);
}
if(a>=40){
  digitalWrite(0, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(2, LOW);
    digitalWrite(pr, LOW);
  digitalWrite(pg, LOW);
  digitalWrite(pb, LOW);
}
if(a>=50){
  digitalWrite(0, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(2, LOW);
    digitalWrite(pr, LOW);
  digitalWrite(pg, LOW);
  digitalWrite(pb, LOW);
}
if(a>=60){
  digitalWrite(0, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(2, HIGH);
    digitalWrite(pr, LOW);
  digitalWrite(pg, LOW);
  digitalWrite(pb, LOW);
}
if(a>=70){
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
    digitalWrite(pr, HIGH);
  digitalWrite(pg, LOW);
  digitalWrite(pb, LOW);
}
if(a>=80){
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
    digitalWrite(pr, LOW);
  digitalWrite(pg, HIGH);
  digitalWrite(pb, LOW);
}
if(a>=90){
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
    digitalWrite(pr, LOW);
  digitalWrite(pg, LOW);
  digitalWrite(pb, HIGH);
}}
