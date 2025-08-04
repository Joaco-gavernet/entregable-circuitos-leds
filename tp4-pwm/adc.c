#include "adc.h"

void adc_init(void) {
    ADMUX = (1 << REFS0); // AVcc
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}

uint16_t adc_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Seleccionar canal
    ADCSRA |= (1 << ADSC); // Iniciar conversión
    while (ADCSRA & (1 << ADSC)); // Polling para esperar a que la conversión termine
    return ADC;
} 