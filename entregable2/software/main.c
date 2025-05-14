#include "main.h"

void main(void)
{
    // Inicializar LCD
    LCD_Init();

    // Inicializar teclado
    // KEYPAD_Init();

    // Inicializar juego
    JUEGO_Init();

    // Configuracion de periodo de ISR (Tmef = Ttimer * (ISR(count)))
    sEOS_Init_Timer(10);

    // Bucle principal
    while(1)
    {
        sEOS_Dispatch_Tasks(); // ejecutar tareas
        // sEOS_Go_To_Sleep(); // modo idle bajo consumo
    }
}