#include "Seos.h"

/***************************************************************
* Propósito de la función: Inicializa el temporizador para el sistema operativo sencillo (sEOS) con el periodo especificado.
* Parámetros de entrada (tipo, rango y formato): uint16_t period (valor del periodo para el temporizador)
* Parámetros de salida (tipo, rango y formato): Ninguno
* Macros y funciones auxiliares:
*   - TCCR0A, TCCR0B: Registros de control del temporizador 0
*   - TCNT0: Registro del contador del temporizador 0
*   - OCR0A: Registro de comparación para el temporizador 0
*   - WGM01: Modo de operación CTC
*   - CS02, CS00: Bits de preescalador
*   - TIMSK0: Registro de interrupciones del temporizador 0
*   - OCIE0A: Habilita la interrupción por comparación
*   - sei: Habilita interrupciones globales
*   - COMPARE_CLOCK_VALUE: Valor de comparación definido para el temporizador
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void sEOS_Init_Timer(uint16_t period) {
    TCCR0A = 0;  // Clear TCCR0A
    TCCR0B = 0;  // Clear TCCR0B
    TCNT0 = 0;   // Initialize counter value
    OCR0A = COMPARE_CLOCK_VALUE;  // Set compare value limit for clock
    TCCR0A |= (1 << WGM01);  // Set CTC mode
    TCCR0B |= (1 << CS02) | (1 << CS00);   // Set prescaler
    TIMSK0 |= (1 << OCIE0A); // Enable Timer0 compare interrupt
    sei();  // Enable global interrupts
}

/***************************************************************
* Propósito de la función: Ejecuta las tareas programadas por el sistema operativo sencillo (sEOS) si la bandera MEF_flag está activa.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Macros y su significado:
*   - MEF_flag: Bandera para indicar si se debe actualizar la MEF
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void sEOS_Dispatch_Tasks (void) {
    if (MEF_flag) {
        MEF_flag = 0;
        JUEGO_Update(); // MEF
    }
}

/***************************************************************
* Propósito de la función: Rutina de servicio de interrupción (ISR) que actualiza la MEF cada MEF_LIMIT interrupciones.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Macros y su significado:
*   - ISR: Macro para definir la rutina de interrupción
*   - TIMER0_COMPA_vect: Vector de interrupción por comparación del temporizador 0
*   - MEF_flag: Bandera para indicar actualización de la MEF
*   - MEF_LIMIT: Límite de interrupciones para activar la bandera
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
ISR(TIMER0_COMPA_vect) {
    if (++cont_MEF == MEF_LIMIT) {  
        MEF_flag = 1;
        cont_MEF = 0;
    }
}