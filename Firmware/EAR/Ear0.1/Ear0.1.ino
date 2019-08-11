#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "sounddata.h"
#define SAMPLE_RATE 17000 // скорость воспроизведения

int speakerPin = 11;
volatile uint16_t sample;
byte lastSample;

// Это называется на 8000 Гц для загрузки следующего образца.
ISR(TIMER1_COMPA_vect) {
    if (sample >= sounddata_length) {
        if (sample == sounddata_length + lastSample) {
            stopPlayback();
        }
        else {
            // Рампа вниз до нуля, чтобы уменьшить щелчок в конце воспроизведения.
            OCR2A = sounddata_length + lastSample - sample;
        }
    }
    else {
          OCR2A = pgm_read_byte(&sounddata_data[sample]);
          }
    ++sample;
}

void startPlayback()
{
    pinMode(speakerPin, OUTPUT);

    // Настройте таймер 2, чтобы выполнить широтно-импульсную модуляцию на динамике
    // pin.

    // Использовать внутренние часы
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));

    // Установите быстрый режим PWM
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);

    // Неинвертируйте ШИМ на контакте OC2A
    // На Arduino это вывод 11.
    TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
    TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));

    //  Нет предварительного делителя 
    TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    // Задайте начальную ширину импульса для первого образца.
    OCR2A = pgm_read_byte(&sounddata_data[0]);


    // Настройте Таймер 1, чтобы отправить образец каждого прерывания.

    cli();

    // Установите режим CTC (Очистить таймер на совпадении)
    // Необходимо установить OCR1A * после * ( *after*), иначе он будет сброшен на 0!
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

    // Без предварительного делителя 
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    // Установите регистр сравнения (OCR1A).
    // OCR1A - это 16-разрядный регистр, поэтому мы должны сделать это с помощью
    // прерывания отключены, чтобы быть в безопасности.
    OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000

    // Включить прерывание, когда TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);

    lastSample = pgm_read_byte(&sounddata_data[sounddata_length-1]);
    sample = 0;
    sei();
}

void stopPlayback()
{
    // Отключить прерывание воспроизведения на выборку.
    TIMSK1 &= ~_BV(OCIE1A);

    // Полностью отключите таймер за выборку.
    TCCR1B &= ~_BV(CS10);

    // Отключите таймер PWM.
    TCCR2B &= ~_BV(CS10);

    digitalWrite(speakerPin, LOW);
}

void setup()
{
    startPlayback();
}

void loop()
{
  
}
