/*
 * Лабораторна робота №4, ч.1. Динамічна індикація.
 * PIC16F84A, Fosc = 4 МГц, MPLAB X, XC8.
 *
 * Дворозрядний ССІ зі спільним катодом:
 *   сегменти a..g, dp — RB0..RB7 (через резистори);
 *   розряд 1 — транзистор VT1 (база через резистор від RA2),
 *   розряд 2 — транзистор VT2 (база через резистор від RA3).
 * На індикаторі — день народження «05».
 * Кожен розряд світиться 5 мс → оновлення кожні 10 мс.
 */

#include <xc.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = OFF, CP = OFF

#define _XTAL_FREQ 4000000

#define DIGIT1  PORTAbits.RA2
#define DIGIT2  PORTAbits.RA3

// Коди символів 0..9 для ССІ (біти: dp g f e d c b a)
const unsigned char MAS[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

#define DAY_TENS  0             // день народження: 05
#define DAY_UNITS 5

void main(void)
{
    TRISB = 0x00;               // сегменти — виходи
    TRISA = 0b00010011;         // RA2, RA3 — виходи на транзистори
    PORTB = 0x00;
    DIGIT1 = 0;
    DIGIT2 = 0;

    while (1)
    {
        //---- розряд 1: десятки ----
        PORTB = MAS[DAY_TENS];  // код символа
        DIGIT1 = 1;             // відкрити VT1
        __delay_ms(5);
        DIGIT1 = 0;             // закрити VT1

        //---- розряд 2: одиниці ----
        PORTB = MAS[DAY_UNITS];
        DIGIT2 = 1;             // відкрити VT2
        __delay_ms(5);
        DIGIT2 = 0;             // закрити VT2
    }
}
