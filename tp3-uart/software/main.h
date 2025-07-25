#ifndef MAIN_H
#define MAIN_H

// Librerias estandares
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <avr/pgmspace.h>
#include <stdint.h>

#include "serialPort.h"
#include "rtc3231.h"
#include "led.h"
#include "command.h"
#include <util/delay.h>
#include <string.h>

#define RX_BUFFER_SIZE 64

uint8_t bcd_to_decimal(uint8_t bcd);
uint8_t decimal_to_bcd(uint8_t decimal);

extern volatile char rx_buffer[RX_BUFFER_SIZE];
extern volatile uint8_t rx_index;
extern volatile uint8_t message_ready;

// Control variables
extern volatile uint8_t mode;  // 0 = OFF, 1 = ON

// State machine variables
extern volatile uint8_t mode_change_requested;

// Timer and alarm variables
extern volatile uint8_t timer_flag_1s;
extern volatile uint8_t alarm_active;
extern volatile uint8_t alarm_count;

#endif
