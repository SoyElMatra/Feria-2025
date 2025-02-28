#include<16F628A.h>
#fuses INTRC_IO,MCLR,NOWDT,NOLVP
#use delay(clock=4M)

#use rs232(baud=9600, parity=N, xmit=PIN_B2, rcv=PIN_B1, bits=8)
#use standard_io(B) 
#use standard_io(A)

#include<stdio.h>

char valor;

#INT_TIMER0

void isr_timer0() {
    set_timer0(126);
    output_toggle(PIN_A7);
}

void stop() {
    output_low(PIN_A0);
    output_low(PIN_A1);
    output_low(PIN_A2);
    output_low(PIN_A3);
}

void Adelante() {
    printf("Adelante\n\n");
    output_high(PIN_A1);
    output_high(PIN_A3);
    delay_ms(100);
}

void Atras() {
    printf("Atras\n\n");
    output_high(PIN_A0);
    output_high(PIN_A2);
    delay_ms(100);
}

void Derecha() {
    printf("Derecha\n\n");
    output_high(PIN_A0);
    output_high(PIN_A3);
    delay_ms(100);
}

void izquierda() {
    printf("\n\nIzquierda\n\n");
    output_high(PIN_A1);
    output_high(PIN_A2);
    delay_ms(100);
}

void main() {
    stop();
    port_b_pullups(TRUE);
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_TIMER0);
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
    set_timer0(126);

    while (1) {
        while (kbhit());
        stop();
        scanf("%c", &valor);
        switch (valor) {
            case 'W':
                Adelante();
                break;
            case 'S':
                Atras();
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
}

