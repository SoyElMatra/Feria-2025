#include<16F886.h>
#fuses INTRC_IO,MCLR,NOWDT,NOLVP
#use delay(clock=4M)

#use rs232(baud=9600, parity=N, xmit=PIN_C6, rcv=PIN_C7, bits=8)
#use standard_io(C)
#use standard_io(B) 
#use standard_io(A)

#include<stdio.h>
#include<stdlib.h>

#define TRIGDEL PIN_C0
#define ECHODEL PIN_C1
#define TRIGDER PIN_C2
#define ECHODER PIN_C3
#define TRIGIZQ PIN_C4
#define ECHOIZQ PIN_C5

int16 duration;
int16 distanceDelante;
int16 distanceDerecha;
int16 distanceIzquierda;

void sr04distDelante() {
    enable_interrupts(INT_TIMER1);
    output_low(TRIGDEL);
    delay_us(2);
    output_high(TRIGDEL);
    delay_us(10);
    output_low(TRIGDEL);

    while (!input(ECHODEL));
    set_timer1(0);
    while (input(ECHODEL));
    duration = get_timer1();

    distanceDelante = (duration / 58);
    disable_interrupts(INT_TIMER1);
}

void sr04distDerecha() {
    enable_interrupts(INT_TIMER1);
    output_low(TRIGDER);
    delay_us(2);
    output_high(TRIGDER);
    delay_us(10);
    output_low(TRIGDER);

    while (!input(ECHODER));
    set_timer1(0);
    while (input(ECHODER));
    duration = get_timer1();

    distanceDerecha = (duration / 58);
    disable_interrupts(INT_TIMER1);
}

void sr04distIzquierda() {
    enable_interrupts(INT_TIMER1);
    output_low(TRIGIZQ);
    delay_us(2);
    output_high(TRIGIZQ);
    delay_us(10);
    output_low(TRIGIZQ);

    while (!input(ECHOIZQ));
    set_timer1(0);
    while (input(ECHOIZQ));
    duration = get_timer1();
    distanceIzquierda = (duration / 58);
    disable_interrupts(INT_TIMER1);
}

void main() {
    enable_interrupts(INT_TIMER1);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    set_timer1(0);
    while (1) {
        sr04distDelante();
        sr04distDerecha();
        sr04distIzquierda();
        printf("Delante: %.2f\n", distanceDelante);
        printf("Derecha: %.2f\n", distanceDerecha);
        printf("Izquierda: %.2f\n\n\n\n", distanceIzquierda);
        delay_ms(1000);
    }
}
