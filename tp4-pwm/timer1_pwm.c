#include "timer1_pwm.h"

void timer1_pwm_init(void) {
    // Modo Fast PWM 8 bits, prescaler 1024
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    DDRB |= (1 << PB1) | (1 << PB2); // Salidas
    OCR1A = 0;
    OCR1B = 0;
}

void timer1_pwm_set(uint8_t channel, uint8_t value) {
    if (channel == 1) {
        OCR1A = value;
    } else if (channel == 2) {
        OCR1B = value;
    }
} 