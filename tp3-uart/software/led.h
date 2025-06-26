#ifndef LED_H
#define LED_H

#ifndef MAIN_H
	#include "main.h"
#endif

void led_setup(void);
void led_on(void);
void led_off(void);
void led_toggle(void);

#endif