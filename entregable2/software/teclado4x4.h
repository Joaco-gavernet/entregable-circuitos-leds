#ifndef TECLADO4X4_H
#define TECLADO4X4_H

#ifndef _MAIN_H
	#include "main.h"
#endif

// Virtual port mapping for PORTB
#define VIRTUAL_PORTB_MAP(Data) do { \
    /* Map PB4 -> PB7 */ \
    if(Data & (1<<4)) PORTB |= (1<<7); else PORTB &= ~(1<<7); \
    /* Map PB3 -> PB6 */ \
    if(Data & (1<<3)) PORTB |= (1<<6); else PORTB &= ~(1<<6); \
    /* Map PB0 -> PB5 */ \
    if(Data & (1<<0)) PORTB |= (1<<5); else PORTB &= ~(1<<5); \
    /* Map PD7 -> PB4 */ \
    if(Data & (1<<7)) PORTB |= (1<<4); else PORTB &= ~(1<<4); \
    /* Map PD3 -> PB3 */ \
    if(Data & (1<<3)) PORTB |= (1<<3); else PORTB &= ~(1<<3); \
    /* Map PD5 -> PB2 */ \
    if(Data & (1<<5)) PORTB |= (1<<2); else PORTB &= ~(1<<2); \
    /* Map PD4 -> PB1 */ \
    if(Data & (1<<4)) PORTB |= (1<<1); else PORTB &= ~(1<<1); \
    /* Map PD2 -> PB0 */ \
    if(Data & (1<<2)) PORTB |= (1<<0); else PORTB &= ~(1<<0); \
} while(0)

// Virtual port mapping for DDRB
#define VIRTUAL_DDRB_MAP(Data) do { \
    /* Map PB4 -> PB7 */ \
    if(Data & (1<<4)) DDRB |= (1<<7); else DDRB &= ~(1<<7); \
    /* Map PB3 -> PB6 */ \
    if(Data & (1<<3)) DDRB |= (1<<6); else DDRB &= ~(1<<6); \
    /* Map PB0 -> PB5 */ \
    if(Data & (1<<0)) DDRB |= (1<<5); else DDRB &= ~(1<<5); \
    /* Map PD7 -> PB4 */ \
    if(Data & (1<<7)) DDRB |= (1<<4); else DDRB &= ~(1<<4); \
    /* Map PD3 -> PB3 */ \
    if(Data & (1<<3)) DDRB |= (1<<3); else DDRB &= ~(1<<3); \
    /* Map PD5 -> PB2 */ \
    if(Data & (1<<5)) DDRB |= (1<<2); else DDRB &= ~(1<<2); \
    /* Map PD4 -> PB1 */ \
    if(Data & (1<<4)) DDRB |= (1<<1); else DDRB &= ~(1<<1); \
    /* Map PD2 -> PB0 */ \
    if(Data & (1<<2)) DDRB |= (1<<0); else DDRB &= ~(1<<0); \
} while(0)

// Virtual port mapping for PINB
#define VIRTUAL_PINB_MAP(Data) do { \
    /* Map PB4 -> PB7 */ \
    if(Data & (1<<4)) PINB |= (1<<7); else PINB &= ~(1<<7); \
    /* Map PB3 -> PB6 */ \
    if(Data & (1<<3)) PINB |= (1<<6); else PINB &= ~(1<<6); \
    /* Map PB0 -> PB5 */ \
    if(Data & (1<<0)) PINB |= (1<<5); else PINB &= ~(1<<5); \
    /* Map PD7 -> PB4 */ \
    if(Data & (1<<7)) PINB |= (1<<4); else PINB &= ~(1<<4); \
    /* Map PD3 -> PB3 */ \
    if(Data & (1<<3)) PINB |= (1<<3); else PINB &= ~(1<<3); \
    /* Map PD5 -> PB2 */ \
    if(Data & (1<<5)) PINB |= (1<<2); else PINB &= ~(1<<2); \
    /* Map PD4 -> PB1 */ \
    if(Data & (1<<4)) PINB |= (1<<1); else PINB &= ~(1<<1); \
    /* Map PD2 -> PB0 */ \
    if(Data & (1<<2)) PINB |= (1<<0); else PINB &= ~(1<<0); \
} while(0)

// Keypad function declarations
uint8_t KeypadUpdate(void);
uint8_t KEYPAD_Scan(uint8_t *pkey);

#endif
