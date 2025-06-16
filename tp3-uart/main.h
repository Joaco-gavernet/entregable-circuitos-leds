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

#define TX_BUFFER_SIZE 64
#define RX_BUFFER_SIZE 64

static volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;



#endif
