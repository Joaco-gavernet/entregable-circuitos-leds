#include "led.h"

/***************************************************************
* Propósito de la función: Configura el pin del LED como salida y lo inicializa apagado.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void led_setup(void) {
  DDRB |= (1 << PB5);  // Set PB5 (pin 13) as output for built-in LED
  PORTB &= ~(1 << PB5);  // Start with LED OFF
}

/***************************************************************
* Propósito de la función: Enciende el LED conectado al pin PB5.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void led_on(void) {
  PORTB |= (1 << PB5);  // Set LED ON
}

/***************************************************************
* Propósito de la función: Apaga el LED conectado al pin PB5.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void led_off(void) {
  PORTB &= ~(1 << PB5);  // Set LED OFF
}

/***************************************************************
* Propósito de la función: Cambia el estado del LED basado en la variable global mode.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void led_toggle(void) {
  if (mode) led_off();
  else led_on();
}