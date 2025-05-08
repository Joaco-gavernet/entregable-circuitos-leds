#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>

// Common port definitions
#define KEYPAD_PORT PORTC
#define KEYPAD_DDR  DDRC
#define KEYPAD_PIN  PINC

// Common timing definitions
#define DEBOUNCE_TIME 20  // milliseconds

#endif
