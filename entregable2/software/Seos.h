/**/

#ifndef SEOS_H
#define SEOS_H


#ifndef _MAIN_H
	#include "main.h"
#endif

void sEOS_Init_Timer(uint16_t period); // Inicializa el timer
void sEOS_Dispatch_Tasks(void); // Actualiza la MEF



#endif
