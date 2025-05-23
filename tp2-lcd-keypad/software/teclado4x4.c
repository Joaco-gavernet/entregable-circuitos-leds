#include "teclado4x4.h"

const uint8_t keymap[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

/***************************************************************
* Propósito de la función: Escanea el teclado 4x4. Si se detecta una tecla presionada, se devuelve el carácter correspondiente por *key y retorna 1. Si no, *key = 0xFF y retorna 0.
* Parámetros de entrada (tipo, rango y formato): uint8_t *key (puntero donde se almacena la tecla detectada)
* Parámetros de salida (tipo, rango y formato): uint8_t (1 si hay tecla, 0 si no)
* Macros y su significado: 
*   - DDRD, DDRB: Configuración de pines como entrada/salida
*   - PORTD, PORTB: Escritura de valores en pines
*   - PIND: Lectura del estado de los pines
*   - PORTD2, PORTD3, PORTD4, PORTD5, PORTD7, PORTB0, PORTB3, PORTB4: Pines usados para filas y columnas
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
uint8_t KeypadUpdate(uint8_t *key) {
    // Configurar columnas como entrada con pull-up (PD2–PD5)
    DDRD &= ~(1 << PORTD2 | 1 << PORTD3 | 1 << PORTD4 | 1 << PORTD5 | 1 << PORTD7);  // PD7 también es entrada
    DDRB &= ~(1 << PORTB0 | 1 << PORTB3 | 1 << PORTB4); // Filas como entrada momentáneamente

    PORTD |= (1 << PORTD2 | 1 << PORTD3 | 1 << PORTD4 | 1 << PORTD5); // Pull-up activado en columnas

    // Preparar pines para forzar salidas en filas
    PORTB &= ~(1 << PORTB0 | 1 << PORTB3 | 1 << PORTB4);
    PORTD &= ~(1 << PORTD7);

    for (uint8_t row = 0; row < 4; row++) {
        // Desactivar filas
        DDRD &= ~(1 << PORTD7);
        DDRB &= ~(1 << PORTB0 | 1 << PORTB3 | 1 << PORTB4);

        // Activar solo la fila correspondiente como salida
        switch (row) {
            case 0: DDRB |= (1 << PORTB4); break; // Fila 0
            case 1: DDRB |= (1 << PORTB3); break; // Fila 1
            case 2: DDRB |= (1 << PORTB0); break; // Fila 2
            case 3: DDRD |= (1 << PORTD7); break; // Fila 3
        }
        *key = 0xFF;

        // Lectura de columnas
        if (!(PIND & (1 << PORTD5))) { *key = keymap[row][1]; return 1; } // Columna 1
        if (!(PIND & (1 << PORTD3))) { *key = keymap[row][0]; return 1; } // Columna 0
        if (!(PIND & (1 << PORTD4))) { *key = keymap[row][2]; return 1; } // Columna 2
        if (!(PIND & (1 << PORTD2))) { *key = keymap[row][3]; return 1; } // Columna 3
    }

    return 0;
}

/***************************************************************
* Propósito de la función: Aplica doble verificación para evitar detecciones múltiples por rebote. Si se detecta una nueva tecla, retorna 1 y guarda el valor en *pkey. Si no hay tecla presionada, retorna 0.
* Parámetros de entrada (tipo, rango y formato): uint8_t *pkey (puntero donde se almacena la tecla detectada)
* Parámetros de salida (tipo, rango y formato): uint8_t (1 si hay tecla, 0 si no)
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
* Otros comentarios: Fecha y log de modificaciones, etc
***************************************************************/
uint8_t KEYPAD_Scan(uint8_t *pkey) {
    static uint8_t old_key = 0xFF;
    static uint8_t last_valid_key = 0xFF;
    uint8_t current_key;

    KeypadUpdate(&current_key);

    if (current_key == 0xFF) {
        old_key = 0xFF;
        last_valid_key = 0xFF;
        return 0;
    }

    if (current_key == old_key) {
        if (current_key != last_valid_key) {
            *pkey = current_key;
            last_valid_key = current_key;
            return 1;
        }
    }

    old_key = current_key;
    return 0;
}

/***************************************************************
* Propósito de la función: Detecta si se presionó la tecla '*', útil para iniciar el juego.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): uint8_t (1 si se presionó '*', 0 si no)
* Macros y su significado: 
*   - KEYPAD_Scan: Función auxiliar para escanear el teclado
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
* Otros comentarios: Fecha y log de modificaciones, etc
***************************************************************/
uint8_t KEYPAD_get_init(void) {
    uint8_t key; 
    if (KEYPAD_Scan(&key)) {
        if (key == '*') {
            return 1;
        }
    }
    return 0;
}