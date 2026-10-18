/*
 * Лабораторна робота №1. Середовище розробки MPLAB.
 * Написання і відлагодження програм.
 * PIC16F84A, MPLAB X IDE, компілятор XC8.
 */

#include <xc.h>

// XT-генератор, WDT вимкнено, PWRT вимкнено, захист коду вимкнено
#pragma config FOSC = XT, WDTE = OFF, PWRTE = OFF, CP = OFF

//---- оголошення глобальних змінних ----
unsigned char TMP;      // 1 байт
unsigned int  CNT16;    // 2 байти
__uint24      CNT24;    // 3 байти
unsigned long CNT32;    // 4 байти

//---- оголошення функцій ----
//---- опис функцій ----

//---- ОСНОВНА ПРОГРАМА ----
void main(void)
{
    //---- цикл ініціалізацій ----
    TMP = 0;
    CNT16 = 0;
    CNT24 = 0;
    CNT32 = 0;

    //---- робочий цикл ----
    while (1)
    {
        TMP = TMP + 2;
        TMP--;
        CNT16++;
        CNT24++;
        CNT32++;
    }
}
