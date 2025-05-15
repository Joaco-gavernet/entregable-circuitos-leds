#include "Juego.h"

/* Variables para funcionamiento de MEF */
extern volatile uint8_t MEF_flag=0;
extern volatile uint8_t cont_MEF=0; 
uint8_t volatile State_call_count = 0;

typedef enum {INIT, SHOW_PASSWORD, INPUT_PASSWORD, VERIFY_PASSWORD, WIN, LOSE} eSystem_state;
static eSystem_state System_state;

char * palabras [] = {"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve", "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa",
"SERIE", "SalUd", "Salud", "Silla", "Tecla", "Valor", "Verde", "YnHRz", "hARdD", "silla"};


/* Helper: Indice aleatorio de la lista de palabras */
static uint8_t random_index(void) { 
    srand(TCNT0);
    return rand() % NUM_PALABRAS;
}

/* Inicializa la MEF controladora del juego */
void JUEGO_Init(void)
{
    System_state = INIT;

}

/* ISR para actualizar la MEF cada 10 ISR */
ISR(TIMER0_COMPA_vect) 
{
    if (++cont_MEF == 20) {  
        MEF_flag = 1;
        cont_MEF = 0;
    }
}

/* Actualizacion de MEF (switch case) */
void JUEGO_Update(void) 
{
    
    // Numero de interrupciones totales
    State_call_count++;

    // Lecturas de teclado
    uint8_t key;  

    // Inicio del juego + tiempo transcurrido (para mostrar en pantalla)
    uint8_t start;
    uint8_t delta;


    // Contador para generar delay 
    static uint32_t count = 0;

    // Flag para mostrar mensaje de inicio (una sola vez)
    static uint8_t init_flag = 0;


    // Palabra random a adivinar
    static uint8_t * password;  

    // Indice de caracteres en segunda fila
    static uint8_t x = 0;

    // Acumulador de numero ingresado
    static uint16_t accumulated_number = 0;  

    // Caracter ingresado (valido)
    static uint8_t caracter_ingresado = 0;

    // Contador correctas e incorrectas
    static uint8_t correct = 0;
    static uint8_t incorrect = 0;

    switch (System_state) {
        
        case INIT:
            if(init_flag == 0){
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring("'*' para iniciar", 16);
                init_flag = 1;
            }
            if(KEYPAD_Scan(&key)) {
                // Si se recibe * de teclado, iniciar juego
                if (key == '*') {
                    System_state = SHOW_PASSWORD;
                }
            }

            break;
        case SHOW_PASSWORD:
            // Mostramos palabra a adivinar durante 10 ciclos de reloj interno
            if(count == 0){  
                password = palabras[random_index()];  
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring(password, LONGITUD_PALABRA);
            }
            else if(count >= 10){
                start = State_call_count;
                LCDclr(); 
                System_state = INPUT_PASSWORD;
            }
            count++;
            break;

        case INPUT_PASSWORD:
            // Reiniciamos contador para delay futuros
            count = 0;

            // Leemos teclado hasta que se presione '#' 
            if(KEYPAD_Scan(&key)){
                // Escribimos caracter ingresado en segunda fila
                LCDGotoXY(x, 1);
                LCDsendChar(key);
                x++;

                // En fin de secuencia verificamos si es un ASCII valido
                if(key == '#') {
                    if((accumulated_number >= 65 && accumulated_number <= 90) || (accumulated_number >= 97 && accumulated_number <= 122)) {  // Rangos [A-Z, a-z]
                        // ASCII valido: Se guarda en caracter_ingresado y se pasa a verificar
                        caracter_ingresado = accumulated_number;
                        System_state = VERIFY_PASSWORD;
                    }
                    // Reset para proximo caracter
                    accumulated_number = 0; 
                    x = 0;
                }
                // Acumulamos ASCII ingresado
                else if(key >= '0' && key <= '9') {
                    accumulated_number = (accumulated_number * 10) + (key - '0');
                }
            }   
            break;

        case VERIFY_PASSWORD:
            // Verificamos si coincide con el proximo caracter de la palabra a adivinar
            if(caracter_ingresado == password[correct]){ 
                LCDGotoXY(correct, 0);
                LCDsendChar(caracter_ingresado);
                correct++;
            }
            else{
                incorrect++;
            }

            // Borramos ASCII ingresado (se puede hacer funcion clearLine())
            LCDGotoXY(0, 1);
            LCDstring("             ", 13);
            LCDGotoXY(0, 1);

            // Reset para proximo caracter
            caracter_ingresado = 0;  

            // Verificamos correctas o errores (para cambio de estado) + tiempo transcurrido
            if(correct == LONGITUD_PALABRA){ 
                System_state = WIN;
                delta = (State_call_count - start) * 100; 
            }
            else if(incorrect == ERROR_MAX){ 
                System_state = LOSE;
            }
            else{
                System_state = INPUT_PASSWORD;
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
                init_flag = 0;
            }
            count++;
            break;
        case LOSE:
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
                init_flag = 0;
            }
            count++;
            break;
    }

}
