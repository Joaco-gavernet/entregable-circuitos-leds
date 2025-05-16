#ifndef TECLADO4X4_H
#define TECLADO4X4_H

#ifndef _MAIN_H
	#include "main.h"
#endif

// Keypad function declarations
uint8_t KeypadUpdate(uint8_t *key);
uint8_t KEYPAD_Scan(uint8_t *pkey);
uint8_t KEYPAD_get_init(void);

#endif