#include "Juego.h"

/* Variables para funcionamiento de MEF */
extern volatile uint8_t MEF_flag=0;
extern volatile uint8_t cont_MEF=0; 
uint8_t volatile State_call_count = 0;

typedef enum {INIT, SHOW_PASSWORD, INPUT_PASSWORD, CORRECT_PASSWORD, INCORRECT_PASSWORD, WIN, LOSE} eSystem_state;
static eSystem_state System_state;

char * palabras [] = {"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve", "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa",
"SERIE", "SalUd", "Salud", "Silla", "Tecla", "Valor", "Verde", "YnHRz", "hARdD", "silla"};


/* Funcion helper para obtener un indice aleatorio de la lista de palabras */
static uint8_t random_index(void) { 
    srand(TCNT0);
    return rand() % NUM_PALABRAS;
}

/* Inicializa la MEF controladora del juego */
void JUEGO_Init(void)
{
    System_state = INIT;
    LCDclr();
    LCDGotoXY(0, 0);
    LCDstring("'*' para iniciar", 16);

}

/* ISR para actualizar la MEF cada 10 ISR */
ISR(TIMER0_COMPA_vect) 
{
    if (++cont_MEF == 20) {  // Changed to >= for more reliable counting
        MEF_flag = 1;
        cont_MEF = 0;
    }
}

/* Actualizacion de MEF (switch case) */
void JUEGO_Update(void) 
{
    // Numero de interrupciones totales
    State_call_count++;

    // Inicio del juego + tiempo transcurrido (para mostrar en pantalla)
    uint8_t start;
    uint8_t delta;

    // Contador para generar delay
    static uint32_t count = 0;

    // Palabra a adivinar
    static uint8_t * password;  // Changed from array to pointer

    // Indice de comparacion (palabra vs palabra ingresada)
    static uint8_t index = 0;
    static uint8_t i = 0;


    static uint16_t accumulated_number = 0;  // To store accumulated digits
    static uint8_t caracter_ingresado = 0;
    uint8_t key;  // Moved outside switch case

    // Contador correctas e incorrectas
    static uint8_t correct = 0;
    static uint8_t incorrect = 0;

    switch (System_state) {
        
        case INIT:
            // Si se recibe * de teclado, iniciar juego
            if(KEYPAD_Scan(&key)) {
                if (key == '*') {
                    System_state = SHOW_PASSWORD;
                }
            }
            break;
        case SHOW_PASSWORD:
            if(count == 0){  // Mostrar palabra a adivinar
                password = palabras[random_index()];  
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring(password, LONGITUD_PALABRA);
                // _delay_ms(4000);
            }
            else if(count >= 10){ //Ts = 100ms; Delay = 2000ms = 20Ts
                start = State_call_count;
                LCDclr(); 
                System_state = INPUT_PASSWORD;
            }
            count++;
            break;

        case INPUT_PASSWORD:
            count = 0;
            // Si se recibe una letra, comparar con la palabra a adivinar
            if(KEYPAD_Scan(&key)){
                // If '#' is pressed, convert accumulated number to ASCII
                LCDGotoXY(i, 1);
                LCDsendChar(key);
                i++;
                if(key == '#') {
                    if((accumulated_number >= 65 && accumulated_number <= 90) || (accumulated_number >= 97 && accumulated_number <= 122)) {  // ASCII A-Z range
                        caracter_ingresado = accumulated_number;
                        LCDGotoXY(correct, 0);
                        LCDsendChar(caracter_ingresado);
                        accumulated_number = 0;  // Reset for next character
                        i = 0;
                    }
                }
                // If a number is pressed, accumulate it
                else if(key >= '0' && key <= '9') {
                    accumulated_number = (accumulated_number * 10) + (key - '0');
                }
            }

            if(caracter_ingresado != 0){
                LCDGotoXY(0, 1);
                LCDstring(&caracter_ingresado, 1);
                if(caracter_ingresado == password[index]){ 
                    // Escribimos letra correcta en primera fila
                    LCDGotoXY(0, correct);
                    LCDstring(&caracter_ingresado, 1);
                    correct++;
                    index++;
                }
                else{
                    incorrect++;
                }
                // Borramos caracter ingresado (luego de compararlo)
                LCDGotoXY(0, 1);
                LCDstring("             ", 13);
                LCDGotoXY(0, 1);
                caracter_ingresado = 0;  // Reset for next character

                if(correct == LONGITUD_PALABRA){ //Palabra adivinada
                    System_state = WIN;
                    delta = (State_call_count - start) * 100; // Interrupciones *  Ts (100ms)
                }
                else if(incorrect == 3){ // Maximos intentos fallidos
                    System_state = LOSE;
                }
            }
            break;
        case WIN:
            if(count == 0){
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring("Victoria!", 9);
                LCDGotoXY(0, 1);
                // Mostrar tiempo transcurrido
                LCDstring("Tiempo: ", 8);
                char time_str[6];
                sprintf(time_str, "%d", delta);
                LCDstring(time_str, strlen(time_str));
                LCDstring("ms", 2);
            }
            else if(count == 50){
                // Delay de 5000ms (50Ts)
                LCDclr();
                count = 0;
                System_state = INIT;
            }
            count++;
            break;
        case LOSE:
            LCDclr();
            if(count == 0){
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring("Derrota!", 8);
            }
            else if(count == 50){
                // Delay de 5000ms (50Ts)
                LCDclr();
                count = 0;
                System_state = INIT;
            }
            count++;
            break;
    }

}


// KEYPADO_Get_init(): Devuelve 1 si se presiona '*'
// KEYPAD_LeerAscii(*ptr): Nos devuelve 1 si se ingreso un caracter, y en ptr el caracter ingresado
