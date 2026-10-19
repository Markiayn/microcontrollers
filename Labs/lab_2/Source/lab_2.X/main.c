/*
 * Лабораторна робота №2, ч.1. Робота з портами вводу/виводу.
 * PIC16F84A, MPLAB X IDE, компілятор XC8.
 *
 * Кн1 — RA0, Кн2 — RA2 (натиснута кнопка дає лог. 0),
 * світлодіоди — RB0..RB7 (світяться при лог. 1).
 */

#include <xc.h>
#include "INIT_MK.h"

// XT-генератор, WDT вимкнено, PWRT вимкнено, захист вимкнено
#pragma config FOSC = XT, WDTE = OFF, PWRTE = OFF, CP = OFF

#define K1_PRESSED  (PORTAbits.RA0 == 0)
#define K2_PRESSED  (PORTAbits.RA2 == 0)

void main(void)
{
    //---- ініціалізація МК: налаштування портів ----
    INIT_MK();

    //---- робочий цикл: алгоритм за блок-схемою ----
    while (1)
    {
        if (K1_PRESSED)
        {
            if (K2_PRESSED)
                PORTBbits.RB1 = 1;      // К1 і К2: RB1 = 1
            else
                PORTB = 0x3C;           // лише К1: 0011 1100
        }
        else
        {
            if (K2_PRESSED)
                PORTB = 129;            // лише К2: 1000 0001
            else
                PORTB = 0xFF;           // кнопки відпущені
        }
    }
}
