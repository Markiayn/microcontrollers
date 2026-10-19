/*
 * Лабораторна робота №3, ч.2. Програмний ШІМ.
 * PIC16F84A, Fosc = 4 МГц, MPLAB X, XC8.
 *
 * RB0 — ШІМ-сигнал (через RC-фільтр 1 кОм / 100 мкФ — аналоговий
 * рівень, який показує вольтметр).
 * Кн1 «Λ» (RA1) — збільшити заповнення, Кн2 «V» (RA0) — зменшити.
 * Період = t1 + t2 = 200 одиниць, старт: t1 = 5, t2 = 195.
 */

#include <xc.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = OFF, CP = OFF

#define _XTAL_FREQ 4000000

#define KN1_PRESSED  (PORTAbits.RA1 == 0)   // «Λ»
#define KN2_PRESSED  (PORTAbits.RA0 == 0)   // «V»
#define PERIOD       200
#define T_MIN        5                      // межі тривалості імпульсу
#define T_MAX        (PERIOD - 5)
#define RAMP_DIV     8   // зміна раз на 8 періодів — плавніше

unsigned char t1, t2, ramp;

// Затримка n одиниць (одиниця ≈ 17 мкс разом з циклом;
// період 200 одиниць = 3449 мкс, ≈ 290 Гц — виміряно Stopwatch)
void delay_units(unsigned char n)
{
    while (n--)
        __delay_us(8);
}

void main(void)
{
    TRISA = 0b00011111;         // RA0..RA4 — входи (кнопки)
    TRISB0 = 0;                 // RB0 — вихід ШІМ
    RB0 = 0;

    t1 = 5;                     // ініціалізація за блок-схемою
    t2 = 195;
    ramp = 0;

    while (1)
    {
        //---- опитування кнопок ----
        if (++ramp >= RAMP_DIV)
        {
            ramp = 0;
            if (KN1_PRESSED && t1 < T_MAX)
            {
                t1++; t2--;
            }
            else if (KN2_PRESSED && t1 > T_MIN)
            {
                t1--; t2++;
            }
        }

        //---- один період ШІМ: імпульс t1, пауза t2 ----
        RB0 = 1;
        delay_units(t1);
        RB0 = 0;
        delay_units(t2);
    }
}
