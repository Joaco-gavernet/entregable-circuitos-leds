#include "Seos.h"



void sEOS_Init_Timer(uint16_t period)
{
    TCCR0A = 0;  // Clear TCCR0A
    TCCR0B = 0;  // Clear TCCR0B
    TCNT0 = 0;   // Initialize counter value to 0
    OCR0A = 249;  // Set compare value for 100ms with prescaler 1024
    TCCR0A |= (1 << WGM01);  // Set CTC mode
    TCCR0B |= (1 << CS02) | (1 << CS00);   // Set prescaler to 1024
    TIMSK0 |= (1 << OCIE0A); // Enable Timer0 compare interrupt
    sei();  // Enable global interrupts
}

void sEOS_Dispatch_Tasks (void) {
    // actualizar MEF cada 100 ms
    if ( MEF_flag) {
        MEF_flag = 0;
        JUEGO_Update(); // MEF
    }
}