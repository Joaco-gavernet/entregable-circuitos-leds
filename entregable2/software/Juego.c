#include "Juego.h"


/* Variables para funcionamiento de MEF */
extern volatile uint8_t MEF_flag=0;
extern volatile uint8_t cont_MEF=0; 
uint16_t volatile State_call_count = 0;

typedef enum {INIT, SHOW_PASSWORD, INPUT_PASSWORD, VERIFY_PASSWORD, WIN, LOSE} eSystem_state;
static eSystem_state System_state;

char * palabras [] = {"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve", "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa",
"SERIE", "SalUd", "Salud", "Silla", "Tecla", "Valor", "Verde", "YnHRz", "hARdD", "silla"};

/***************************************************************
* Propósito de la función: Genera un índice aleatorio para seleccionar
*   una palabra de la lista.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): uint8_t, valor entre 0 y NUM_PALABRAS-1
* Condiciones de Error de la función (poner ejemplos si hace falta): Ninguna
* Macros y su significado: srand, rand, NUM_PALABRAS
* Otros comentarios: Autor, fecha y log de modificaciones, etc
***************************************************************/
static uint8_t random_index(void) { srand(State_call_count); return rand() % NUM_PALABRAS; }  // Helper: Indice aleatorio de la lista de palabras

/***************************************************************
* Propósito de la función: Inicializa el juego, limpia la pantalla
*   y muestra el mensaje de inicio.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Condiciones de Error de la función (poner ejemplos si hace falta): Ninguna
* Macros y su significado: LCDclr, LCDGotoXY, LCDstring
* Otros comentarios: Autor, fecha y log de modificaciones, etc
***************************************************************/
void JUEGO_Init(void) {
    LCDclr();
    LCDGotoXY(0, 0);
    LCDstring("'*' para iniciar", 16);
    System_state = INIT;
}

/***************************************************************
* Propósito de la función: Actualiza el estado del juego según la
*   máquina de estados finitos (MEF), gestiona la lógica principal.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Condiciones de Error de la función (poner ejemplos si hace falta): Ninguna
* Macros y su significado: LCDclr, LCDGotoXY, LCDstring, LCDsendChar, KEYPAD_Scan, etc.
* Otros comentarios: Autor, fecha y log de modificaciones, etc
***************************************************************/
void JUEGO_Update(void) {
    State_call_count++;  // Numero de interrupciones totales  
    uint8_t key;         // Lecturas de teclado
    static uint16_t start;       // Inicio del juego + tiempo transcurrido (para mostrar en pantalla)
    uint16_t delta;
    static uint32_t count = 0;           // Contador para generar delay 
    static uint8_t init_flag = 0;        // Flag para mostrar mensaje de inicio (una sola vez)
    static uint8_t * password;           // Palabra random a adivinar
    static uint8_t x = 0;                // Indice de caracteres en segunda fila
    static uint16_t accumulated_number = 0;  // Acumulador de numero ingresado
    static uint8_t caracter_ingresado = 0;   // Caracter ingresado (valido)
    static uint8_t correct = 0;          // Contador correctas e incorrectas
    static uint8_t incorrect = 0;

    switch (System_state) {
        case INIT:
            if(init_flag == 0) {
                JUEGO_Init();
                count = 0;
                init_flag = 1;
                correct = 0;
                incorrect = 0;
            }
            if(KEYPAD_Scan(&key)) {
                if (key == '*') System_state = SHOW_PASSWORD;  // Si se recibe * de teclado, iniciar juego
            }
            break;
        case SHOW_PASSWORD:
            if(count == 0) {  
                password = palabras[random_index()];  
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring(password, LONGITUD_PALABRA);
            }
            else if(count >= 20) {
                start = State_call_count;
                LCDclr(); 
                LCDGotoXY(13, 0);
                LCDstring("***", 3);
                LCDGotoXY(0, 1);
                System_state = INPUT_PASSWORD;
            }
            count++;
            break;
        case INPUT_PASSWORD:
            count = 0;  // Reiniciamos contador para delay futuros
            if(KEYPAD_Scan(&key)) {
                LCDGotoXY(x, 1);  // Escribimos simbolo ingresado en segunda fila
                LCDsendChar(key);
                x++;

                if(key == '#' || (accumulated_number > 122)) {
                    caracter_ingresado = accumulated_number;
                    System_state = VERIFY_PASSWORD;
                    accumulated_number = 0;  // Reset para proximo caracter
                    x = 0;
                    LCDclearline(1);
                }
                else if(key >= '0' && key <= '9') {
                    if (accumulated_number == 0 && key == '0') {
                        LCDclearline(1);
                        LCDGotoXY(0,1);
                        x = 0; 
                    }
                    accumulated_number = (accumulated_number * 10) + (key - '0');  // Acumulamos ASCII ingresado
                }
            }   
            break;
        case VERIFY_PASSWORD:
            if(caracter_ingresado == password[correct]) { 
                LCDGotoXY(correct, 0);
                LCDsendChar(caracter_ingresado);
                correct++;
            }
            else {
                LCDGotoXY(13 + incorrect++, 0);
                LCDsendChar(' '); 
                LCDGotoXY(0,1); 
            }
            caracter_ingresado = 0;  

            if(correct == LONGITUD_PALABRA) {System_state = WIN;}
            else if(incorrect == ERROR_MAX) System_state = LOSE;
            else System_state = INPUT_PASSWORD;
            break;
        case WIN:
            if(count == 0) {
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring("Victoria!", 9);
                LCDGotoXY(0, 1);
                LCDstring("Tiempo: ", 8);  // Mostrar tiempo transcurrido
                char time_str[6];
                sprintf(time_str, "%u", (uint16_t)((State_call_count - start)*PERIODO_MEF));
                LCDstring(time_str, strlen(time_str));
                LCDstring("ms", 2);
            }
            else if(count == 50) {  // Delay de 5000ms (50Ts)
                LCDclr();
                count = 0;
                System_state = INIT;
                init_flag = 0;
            }
            count++;
            break;
        case LOSE:
            if(count == 0) {
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring("Derrota!", 8);
            }
            else if(count == 50) {  // Delay de 5000ms (50Ts)
                LCDclr();
                count = 0;
                System_state = INIT;
                init_flag = 0;
            }
            count++;
            break;
    }
}
