#include <16F628A.h>
#fuses INTRC_IO, MCLR, NOWDT, NOPROTECT, NOLVP
#use delay(clock=4000000)

#use rs232(baud=9600, parity=N, xmit=PIN_B2, rcv=PIN_B1, bits=8)
#use standard_io(B) 
#use standard_io(A)
#include <stdio.h>

#define TRIG_PIN PIN_B3
#define ECHO_PIN PIN_B4

void main() {
    enable_interrupts(INT_TIMER1);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    set_timer1(0);
   int32 duration, distance;
   while(TRUE) {
      output_low(TRIG_PIN);
      delay_us(2);
      output_high(TRIG_PIN);
      delay_us(10);
      output_low(TRIG_PIN);

      while(!input(ECHO_PIN));
      set_timer1(0);
      while(input(ECHO_PIN));
      duration = get_timer1();

      distance = (duration / 58.82);
      printf("Distancia: %lu cm\n", distance);

      delay_ms(500);
   }
}