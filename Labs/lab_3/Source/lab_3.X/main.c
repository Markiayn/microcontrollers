/*
 * Лабораторна робота №3, ч.1. Формування часових інтервалів.
 * PIC16F84A, Fosc = 4 МГц (1 цикл = 1 мкс), MPLAB X, XC8.
 *
 * На RB4 — імпульси F = 5 кГц (T = 200 мкс). Номер у журналі 14
 * (парний) → коефіцієнт заповнення D = 10 %:
 *   тривалість імпульсу t = D * T = 20 мкс, пауза 180 мкс,
 *   шпарність S = T / t = 1 / D = 10.
 *
 * METHOD: 1 — __delay_us(), 2 — цикл while(), 3 — вставка асемблера.
 */

#include <xc.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = OFF, CP = OFF

#define _XTAL_FREQ 4000000

#ifndef METHOD
#define METHOD 1
#endif

volatile unsigned char DLY;     // лічильник затримки

void main(void)
{
    TRISB4 = 0;                 // RB4 — вихід
    RB4 = 0;

    while (1)
    {
#if METHOD == 1
        //---- спосіб 1: бібліотечна функція __delay_us() ----
        RB4 = 1;
        __delay_us(18);         // + 2 цикли на RB4 = 20 мкс
        RB4 = 0;
        __delay_us(176);        // + 4 цикли (RB4, goto) = 180 мкс
#elif METHOD == 2
        //---- спосіб 2: затримки на операторі while() ----
        RB4 = 1;
        DLY = 2;                // 1 ітерація ≈ 7 циклів (Stopwatch)
        while (--DLY)
            ;
        NOP(); NOP(); NOP(); NOP();
        RB4 = 0;
        DLY = 25;
        while (--DLY)
            ;
#else
        //---- спосіб 3: вставка асемблерного коду ----
        RB4 = 1;
        asm("movlw  5");        // цикл: 3 * 5 - 1 = 14 циклів
        asm("movwf  _DLY");
        asm("decfsz _DLY, f");
        asm("goto   $-1");
        asm("nop");
        asm("nop");
        RB4 = 0;
        asm("movlw  58");       // цикл: 3 * 58 - 1 = 173 цикли
        asm("movwf  _DLY");
        asm("decfsz _DLY, f");
        asm("goto   $-1");
        asm("nop");
#endif
    }
}
