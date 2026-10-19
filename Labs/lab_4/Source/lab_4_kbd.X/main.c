/*
 * Лабораторна робота №4, ч.2. Матрична клавіатура 3×4.
 * PIC16F84A, Fosc = 4 МГц, MPLAB X, XC8.
 *
 * Стовпці (виходи, активна «1», через 1 кОм):
 *   стовпець 1 — RB2, стовпець 2 — RB1, стовпець 3 — RB0.
 * Рядки (входи, стяжка 10 кОм на GND):
 *   A (1 2 3) — RB4, B (4 5 6) — RB5, C (7 8 9) — RB6, D (* 0 #) — RB7.
 * Код натиснутої кнопки — на RA0..RA3 (ССІ з дешифратором,
 * статична індикація). «*» → E, «#» → F.
 */

#include <xc.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = OFF, CP = OFF

#define _XTAL_FREQ 4000000

// Код символа для кожної кнопки [рядок][стовпець]
const unsigned char KEY_CODE[4][3] = {
    {0x1, 0x2, 0x3},
    {0x4, 0x5, 0x6},
    {0x7, 0x8, 0x9},
    {0xE, 0x0, 0xF}
};

// Маска підключення стовпця: 1 — RB2, 2 — RB1, 3 — RB0
const unsigned char COL_MASK[3] = {0x04, 0x02, 0x01};

void init_mk(void)
{
    TRISA = 0b00010000;         // RA0..RA3 — вихід коду
    TRISB = 0b11110000;         // RB0..RB3 — виходи, RB4..RB7 — входи
    PORTA = 0x00;
    PORTB = 0x00;
}

// Сканування матриці: повертає код кнопки або 0xFF
unsigned char scan_keypad(void)
{
    for (unsigned char col = 0; col < 3; col++)
    {
        PORTB = COL_MASK[col];          // підключити стовпець
        __delay_us(50);                 // встановлення рівнів
        unsigned char rows = PORTB >> 4;
        for (unsigned char row = 0; row < 4; row++)
        {
            unsigned char bit = (unsigned char)(1u << row);
            if (rows & bit)             // кнопка натиснута
            {
                __delay_ms(20);         // брязкіт контактів
                while ((PORTB >> 4) & bit)
                    ;                   // чекати відпускання
                return KEY_CODE[row][col];
            }
        }
    }
    PORTB = 0x00;
    return 0xFF;
}

void main(void)
{
    init_mk();
    while (1)
    {
        unsigned char code = scan_keypad();
        if (code != 0xFF)
            PORTA = code;               // вивести код у порт A
    }
}
