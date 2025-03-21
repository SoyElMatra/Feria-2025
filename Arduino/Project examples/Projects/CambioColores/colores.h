/*
Este archivo se graba como libreria para poder ser utilizado posteriormente por otros programas
*/



/* 
Esta es la definicion del tipo de variables COLOR, de manera que cada variable definida como color
tiene tres numeros enteros correspondientes a red, green y blue..
*/
struct color{ 
  int red;
  int gre;
  int blu;
};




/*
Esta rutina es solo para sacar los valores del bucle por pantalla, no debe usarse en condiciones normales
es solamente para poder debugear el programa.
*/

void muestraevolucioncolor(color c1,color c2, int r, int g, int b){
    Serial.print ("\nColor ini (R|G|B): (");
    Serial.print (c1.red);
    Serial.print ("|");
    Serial.print (c1.gre);
    Serial.print ("|");
    Serial.print (c1.blu);
    Serial.print (") - Color fin: (");
    Serial.print (c2.red);
    Serial.print ("|");
    Serial.print (c2.gre);
    Serial.print ("|");
    Serial.print (c2.blu);
    Serial.print (") - Color actual: (");
    Serial.print (r);
    Serial.print ("|");
    Serial.print (g);
    Serial.print ("|");
    Serial.print (b);
    Serial.print (")");
    return;
}




/*===================================================================================================
  ===== CambiaColor es el procedimiento para cambiar suavemente el color del led rgb y requiere =====
  ===== los siguientes parámetros:                                                              =====
  ===== c1=color inicial, c2=color final, pasos=cantidad de bucles para alcanzar el nuevo color =====
  ===== pr, pg, pb, son los puertos de la placa arduino donde se conecta el led RGB.            =====
  ===================================================================================================
*/
void CambiaColor(color c1, color c2, int pasos, int pr, int pg, int pb) { 
  float rf,gf,bf,rdif,gdif,bdif; // defino como flotante las variables porque si la cantidad de pasos es mayor a 255, se necesitan decimales para calcular
  int r,g,b; // luego defino como enteros los parámetros de color que voy a mandar al led.
  
  rdif=c2.red - c1.red; // Diferencia entre el rojo del primer color y el segundo
  gdif=c2.gre - c1.gre; // Diferencia entre el verde del primer color y el segundo
  bdif=c2.blu - c1.blu; // Diferencia entre el azul del primer color y el segundo
  
  for (int i=1; i<=pasos;i++){ // cuento la cantidad de pasos para realizar el cambio.
    rf=c1.red + (i * (rdif / pasos)); // Divido la diferencia del rojo en la cantidad de pasos y los multiplico por i que es el paso que está ejecutándose en este momento.
    gf=c1.gre + (i * (gdif / pasos)); // lo mismo para el verde
    bf=c1.blu + (i * (bdif / pasos)); // y el azul.
    r=rf; // Convierto los resultados flotantes a enteros
    g=gf;
    b=bf;
    // escribo los valores en el led.
    analogWrite(pr,r );
    analogWrite(pg,g );
    analogWrite(pb,b );
    

/*
// Esta parte solo activarla si se desea mostrar la evolucion del color en pantalla, caso contrario conviene dejarla apagada para evitar el flickeo

    if ((i % 300) == 0){
      muestraevolucioncolor(c1,c2,r,g,b);
    }
*/

  }
}
