#include<16F886.h>
#fuses INTRC_IO,MCLR,INTRC_IO,NOWDT,NOLVP
#use delay(clock=4M)

#use rs232(baud=9600, parity=N, xmit=PIN_C6, rcv=PIN_C7, bits=8)
#use standard_io(B) 
#use standard_io(A)


#define LCD_ENABLE_PIN  PIN_B3
#define LCD_RS_PIN      PIN_B1
#define LCD_RW_PIN      PIN_B2
#define LCD_DATA4       PIN_B4
#define LCD_DATA5       PIN_B5
#define LCD_DATA6       PIN_B6
#define LCD_DATA7       PIN_B7
#include<LCD.c>
#include<stdio.h>

int32 count = 0;
char valor;

void stop() {
    disable_interrupts(INT_TIMER1);
    output_low(PIN_A3);
    output_low(PIN_A4);
    output_low(PIN_A5);
    output_low(PIN_A6);
}

#INT_EXT

void isr_ext() {
    stop();
    lcd_putc("\fGame Over\n");
    printf(lcd_putc, "Score: %lu", count * 10);
    while (1);
}

#INT_TIMER0

void isr_timer0() {
    set_timer0(178);
    output_toggle(PIN_A7);
}

#INT_TIMER1

void isr_timer1() {
    set_timer1(3035);
    count++;
}

void Adelante() {
    enable_interrupts(INT_TIMER1);
    printf("Adelante\n\n");
    lcd_putc("\fAdelante");
    output_high(PIN_A4);
    output_high(PIN_A6);
    delay_ms(100);
}

void Atras() {
    enable_interrupts(INT_TIMER1);
    printf("Atras\n\n");
    lcd_putc("\fAtras");
    output_high(PIN_A3);
    output_high(PIN_A5);
    delay_ms(100);
}

void Derecha() {
    enable_interrupts(INT_TIMER1);
    printf("Derecha\n\n");
    lcd_putc("\fDerecha");
    output_high(PIN_A3);
    output_high(PIN_A6);
    delay_ms(100);
}

void izquierda() {
    printf("\n\nIzquierda\n\n");
    enable_interrupts(INT_TIMER1);
    lcd_putc("\fIzquierda");
    output_high(PIN_A4);
    output_high(PIN_A5);
    delay_ms(100);
}

void main() {
    stop();
    lcd_init();
    lcd_putc("\f");
    port_b_pullups(1);
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_TIMER0);
    disable_interrupts(INT_TIMER1);
    enable_interrupts(INT_EXT_H2L);
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    set_timer0(178);
    set_timer1(3035);
    setup_uart(9600);
    while (1) {
        while (kbhit());
        lcd_putc("\f");
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