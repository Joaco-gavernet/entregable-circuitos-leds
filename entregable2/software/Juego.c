#include "Juego.h"

/* Variables para funcionamiento de MEF */
extern volatile uint8_t MEF_flag=0;
extern volatile uint8_t cont_MEF=0; 
uint8_t volatile State_call_count = 0;

typedef enum {INIT, SHOW_PASSWORD, INPUT_PASSWORD, VERIFY_PASSWORD, WIN, LOSE} eSystem_state;
static eSystem_state System_state;

char * palabras [] = {"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve", "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa",
"SERIE", "SalUd", "Salud", "Silla", "Tecla", "Valor", "Verde", "YnHRz", "hARdD", "silla"};

static uint8_t random_index(void) { srand(State_call_count); return rand() % NUM_PALABRAS; }  // Helper: Indice aleatorio de la lista de palabras

void JUEGO_Init(void) {
    LCDclr();
    LCDGotoXY(0, 0);
    LCDstring("'*' para iniciar", 16);
    System_state = INIT;
}

void JUEGO_Update(void) {
    State_call_count++;  // Numero de interrupciones totales  
    uint8_t key;         // Lecturas de teclado
    uint8_t start;       // Inicio del juego + tiempo transcurrido (para mostrar en pantalla)
    uint8_t delta;
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
            else if(count >= 10) {
                start = State_call_count;
                LCDclr(); 
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

                if(key == '#') {
                    caracter_ingresado = accumulated_number;
                    System_state = VERIFY_PASSWORD;
                    accumulated_number = 0;  // Reset para proximo caracter
                    x = 0;
                    LCDGotoXY(0, 1);  // Borramos ASCII ingresado
                    LCDstring("             ", 13);
                    LCDGotoXY(0, 1);
                }
                else if(key >= '0' && key <= '9') accumulated_number = (accumulated_number * 10) + (key - '0');  // Acumulamos ASCII ingresado
            }   
            break;
        case VERIFY_PASSWORD:
            if(caracter_ingresado == password[correct]) { 
                LCDGotoXY(correct, 0);
                LCDsendChar(caracter_ingresado);
                correct++;
            }
            else incorrect++;
            caracter_ingresado = 0;  

            if(correct == LONGITUD_PALABRA) { 
                System_state = WIN;
                delta = (State_call_count - start) * 100; 
            }
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
                sprintf(time_str, "%d", delta);
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
