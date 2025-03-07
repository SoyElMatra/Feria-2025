#include <16F886.h>
#fuses INTRC_IO, MCLR, NOWDT, NOLVP
#use delay(clock = 4M)

#use rs232(baud = 9600, parity = N, xmit = PIN_C6, rcv = PIN_C7, bits = 8)
#use standard_io(C)
#use standard_io(B)
#use standard_io(A)

#include <stdio.h>
#include <stdlib.h>

char posA;
char posB;
char valor;
int check = 0;
int16 duration;
int16 turns = turn;
int1 testprintf = 1;
int16 distanceDelante;
int16 distanceDerecha;
int16 distanceIzquierda;

void stop() {
    output_low(PIN_A0);
    output_low(PIN_A1);
    output_low(PIN_A2);
    output_low(PIN_A3);
}

void Adelante() {
    if (testprintf == 1) {
        printf("\n\nAdelante\n\n");
    }
    testprintf = 0;
    output_high(PIN_A1);
    output_high(PIN_A3);
    delay_ms(100);
}

void Derecha() {
    if (testprintf == 1) {
        printf("\n\nDerecha\n\n");
    }
    testprintf = 0;
    output_high(PIN_A0);
    output_high(PIN_A3);
    delay_ms(turns);
}

void izquierda() {
    if (testprintf == 1) {
        printf("\n\nIzquierda\n\n");
    }
    testprintf = 0;
    output_high(PIN_A1);
    output_high(PIN_A2);
    delay_ms(turns);
}

void action() {
    switch (valor) {
        case 'W':
            Adelante();
            break;
        case 'D':
            Derecha();
            break;
        case 'A':
            Izquierda();
            break;
        default:
            stop();
            break;
    }
}

void sr04distDelante() {
    enable_interrupts(INT_TIMER1);
    output_low(TRIGDEL);
    delay_us(2);
    output_high(TRIGDEL);
    delay_us(10);
    output_low(TRIGDEL);

    set_timer1(0);
    while (!input(ECHODEL));
    set_timer1(0);
    while (input(ECHODEL));
    duration = get_timer1();

    distanceDelante = (duration / 58);
    disable_interrupts(INT_TIMER1);
}

void posicion() {
    posA = 0;
    posB = 0;
    if (input(PIN_B4)) {
        posA = 'W';
    } else if (input(PIN_B5)) {
        posA = 'S';
    }
    if (input(PIN_B6)) {
        posB = 'D';
    } else if (input(PIN_B7)) {
        posB = 'A';
    }
}

void main() {
    stop();
    enable_interrupts(INT_TIMER0);
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    set_timer1(0);
    set_timer0(0);

    while (TRUE) {
        sr04distDelante();
        posicion();
        valor = posA;

        if (posA == 'W') {
            if (distanceDelante > 5) {
                valor = posA;
            } else {
                valor = 0;
            }
        }
        if (posA == 'S') {
            turns = back;
            valor = 'D';
        }
        if (posB == 'D') {
            if (distanceDerecha > 5 && (distanceDelante <= 5 || posA != 'W')) {
                turns = turn;
                valor = posB;
            } else if (distanceDerecha < 5) {
                posB = 'A';
            }
        }
        if (posB == 'A') {
            if (distanceIzquierda > 5 && (distanceDelante <= 5 || posA != 'W')) {
                turns = turn;
                valor = posB;
            }
        }
        if (distanceDelante <= 5 && distanceDerecha <= 5 && distanceIzquierda <= 5) {
            valor = 'D';
            posA = 'S';
        }
        if (valor == 0) {
            testprintf = 1;
        }
        action();
        if (get_timer0() >= 196) {
            set_timer0(0);
            check++;
        }
        if (check > 2) {
            check = 0;
            printf("\nAdelante: %lu cm\n", distanceDelante);
            printf("\nDerecha: %lu cm\n", distanceDerecha);
            printf("\nIzquierda: %lu cm\n\n\n", distanceIzquierda);
        }
    }
}