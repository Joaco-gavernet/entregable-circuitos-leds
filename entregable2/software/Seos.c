#include "Seos.h"

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

void sEOS_Dispatch_Tasks (void) {
    if (MEF_flag) {
        MEF_flag = 0;
        JUEGO_Update(); // MEF
    }
}

/* ISR para actualizar la MEF cada MEF_LIMIT ISR */
ISR(TIMER0_COMPA_vect) {
    if (++cont_MEF == MEF_LIMIT) {  
        MEF_flag = 1;
        cont_MEF = 0;
    }
}