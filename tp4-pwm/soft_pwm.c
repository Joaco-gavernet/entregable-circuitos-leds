#include "soft_pwm.h"

static volatile uint8_t soft_pwm_duty = 0;
static volatile uint8_t soft_pwm_counter = 0;

void soft_pwm_init(void) {
    DDRB |= (1 << PB5);
    // TIMER0 en modo CTC, prescaler 8, OCR0A=129 para 15360Hz
    TCCR0A = (1 << WGM01); // Modo CTC
    TCCR0B = (1 << CS01);  // Prescaler 8
    OCR0A = 129;
    TIMSK0 = (1 << OCIE0A); // Habilitar interrupción por comparación
}

void soft_pwm_set(uint8_t value) {
    soft_pwm_duty = value;
}

ISR(TIMER0_COMPA_vect) {
    if (soft_pwm_counter < soft_pwm_duty) {
        PORTB |= (1 << PB5);
    } else {
        PORTB &= ~(1 << PB5);
    }
    soft_pwm_counter++;
    if (soft_pwm_counter == 0xFF) {
        soft_pwm_counter = 0;
    }
} 