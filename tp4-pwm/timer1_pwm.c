#include "timer1_pwm.h"

void timer1_pwm_init(void) {
    // Modo Fast PWM 8 bits, prescaler 1024, modo invertido
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0); // Configurar modo invertido
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // Configurar prescaler 1024
    DDRB |= (1 << PB1) | (1 << PB2); // Salidas
    OCR1A = 255; // 0% duty cycle en modo invertido
    OCR1B = 255; // 0% duty cycle en modo invertido
}

void timer1_pwm_set(uint8_t channel, uint8_t value) {
    // En modo invertido: value=0 -> ~100% duty cycle, value=255 -> 0% duty cycle
    uint8_t inverted_value = 255 - value;
    if (channel == 1) OCR1A = inverted_value;
    else if (channel == 2) OCR1B = inverted_value;
} 