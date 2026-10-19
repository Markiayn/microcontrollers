/*
 * Лабораторна робота №2, ч.2. Кодовий замок.
 * PIC16F84A, MPLAB X IDE, компілятор XC8.
 *
 * Кнопки (на GND, підтяжка 10 кОм, натиснута = 0):
 *   «1» — RB3, «2» — RB2, «3» — RB1, «4» — RB0, OK — RB4, CLR — RB5.
 * Виходи: RA0 — LED «доступ», RA1 — LED «помилка»,
 *         RA2 — реле, що вмикає освітлення (лампу).
 * Код доступу «1223», кодування — байт на кожне натискання:
 *   старша тетрада — номер у черзі, молодша — код кнопки.
 */

#include <xc.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = OFF, CP = OFF

#define _XTAL_FREQ 4000000

#define PIN_LEN   4
#define BTN_OK    PORTBbits.RB4
#define BTN_CLR   PORTBbits.RB5
#define LED_OK    PORTAbits.RA0
#define LED_ERR   PORTAbits.RA1
#define RELAY     PORTAbits.RA2

// Код «1223»: черга 1..4 у старшій тетраді, код кнопки у молодшій
const unsigned char SECRET[PIN_LEN] = {0x11, 0x22, 0x32, 0x44};

unsigned char entered[PIN_LEN];     // введені коди
unsigned char count;                // кількість натискань

// Код кнопки (К1..К4 → біти 0..3) за номером виводу RB0..RB3
const unsigned char KEY_CODE[4] = {0x08, 0x04, 0x02, 0x01};

void init_mk(void)
{
    TRISA = 0b00011000;     // RA0..RA2 — виходи
    TRISB = 0b11111111;     // RB0..RB7 — входи (кнопки)
    PORTA = 0x00;
}

void clear_input(void)
{
    count = 0;
    for (unsigned char i = 0; i < PIN_LEN; i++)
        entered[i] = 0;
}

// Чекає відпускання кнопки (захист від повторного зчитування)
void wait_release(unsigned char mask)
{
    __delay_ms(20);
    while ((PORTB & mask) == 0)
        ;
    __delay_ms(20);
}

unsigned char code_ok(void)
{
    if (count != PIN_LEN)
        return 0;
    for (unsigned char i = 0; i < PIN_LEN; i++)
        if (entered[i] != SECRET[i])
            return 0;
    return 1;
}

void main(void)
{
    init_mk();
    clear_input();

    while (1)
    {
        //---- опитування цифрових кнопок RB0..RB3 ----
        for (unsigned char b = 0; b < 4; b++)
        {
            unsigned char mask = (unsigned char)(1u << b);
            if ((PORTB & mask) == 0)
            {
                wait_release(mask);
                if (count < PIN_LEN)
                {
                    unsigned char order = (unsigned char)((count + 1) << 4);
                    entered[count] = order | KEY_CODE[b];
                }
                count++;    // більше 4 натискань — код невірний
            }
        }

        //---- OK: порівняння введеного коду з кодом у пам'яті ----
        if (BTN_OK == 0)
        {
            wait_release(0x10);
            if (code_ok())
            {
                LED_OK = 1; LED_ERR = 0; RELAY = 1;   // доступ + світло
            }
            else
            {
                LED_OK = 0; LED_ERR = 1; RELAY = 0;   // помилка
            }
            clear_input();
        }

        //---- CLR: скидання виходів і введених параметрів ----
        if (BTN_CLR == 0)
        {
            wait_release(0x20);
            LED_OK = 0; LED_ERR = 0; RELAY = 0;
            clear_input();
        }
    }
}
