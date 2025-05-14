#ifndef JUEGO_H
#define JUEGO_H

#ifndef _MAIN_H
    #include "main.h"
#endif

// External declarations for variables used across files
extern volatile uint8_t MEF_flag;
extern volatile uint8_t cont_MEF;
uint8_t volatile State_call_count;

/* Funcion helper para obtener un indice aleatorio de la lista de palabras */
static uint8_t random_index(void);

/* Inicializa la MEF controladora del juego */
void JUEGO_Init(void);

/* Actualizacion de MEF (switch case) */
void JUEGO_Update(void);

uint8_t JUEGO_Get_init(void);

#endif
