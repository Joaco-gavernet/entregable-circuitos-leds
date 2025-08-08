#ifndef ADC_H
#define ADC_H
#include "main.h"

void adc_init(void);
uint16_t adc_read(uint8_t channel);

#endif // ADC_H 