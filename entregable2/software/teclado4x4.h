#ifndef TECLADO4X4_H
#define TECLADO4X4_H

#ifndef _MAIN_H
	#include "main.h"
#endif

// Keypad function declarations
uint8_t KeypadUpdate(void);
uint8_t KEYPAD_Scan(uint8_t *pkey);

#endif
