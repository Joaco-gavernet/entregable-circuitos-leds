#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "timer1_pwm.h"
#include "soft_pwm.h"
#include "adc.h"
#include "color_control.h"
#include "serialPort.h"

// Definiciones globales
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#endif // MAIN_H
