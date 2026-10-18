/*
 * Практична робота №1. Конфігурування мікроконтролера.
 * PIC16F84A, MPLAB X IDE, компілятор XC8.
 *
 * Варіант 1: захист коду вимкнено, затримку ввімкнення
 * живлення (PWRT) увімкнено, сторожовий таймер (WDT)
 * увімкнено, кварцовий XT-генератор.
 * Слово конфігурації (адреса 2007h) = 3FF5h.
 */

//---- файл опису підтримки процесора ----
#include <xc.h>

//---- конфігураційне слово ----
// HI-TECH: __CONFIG(FOSC_XT & WDTE_ON & PWRTE_ON & CP_OFF);
#pragma config FOSC = XT, WDTE = ON, PWRTE = ON, CP = OFF

//---- оголошення змінних ----
unsigned char TMP1;
unsigned char TMP2;
unsigned char RES;

//---- оголошення функцій ----
unsigned char SUM(unsigned char a, unsigned char b);

//---- опис функцій ----
// Сума двох 8-бітних чисел (результат за модулем 256)
unsigned char SUM(unsigned char a, unsigned char b)
{
    return a + b;
}

//---- основна програма ----
void main(void)
{
    //---- цикл ініціалізацій ----
    TMP1 = 5;
    TMP2 = 3;
    RES = 0;

    //---- робочий цикл ----
    while (1)
    {
        CLRWDT();               // скидання WDT, інакше перезапуск
        RES = SUM(TMP1, TMP2);  // RES = 8
    }
}
