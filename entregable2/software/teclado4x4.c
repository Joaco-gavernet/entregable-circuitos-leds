#include "teclado4x4.h"

uint8_t KeypadUpdate(void)
{
    uint8_t r,c;
    PORTB |= 0x0F;
    for(c=0;c<4;c++){
        DDRB &= ~(0XFF);
        DDRB |= ~(0X80 >> c);
        for(r=0;r<4;r++){
            if(!(PINB & (0X08 >> r))){
                return (r*4 + c + 1);
            }
        }
    }
    return 0xFF;
}

uint8_t KEYPAD_Scan (uint8_t *pkey) 
{
    static uint8_t Old_key, Last_valid_key=0xFF; // no hay tecla presionada;
    uint8_t Key;
    
    Key = KeypadUpdate();
    if(Key==0xFF){
        Old_key=0xFF;	// no hay tecla presionada
        Last_valid_key=0xFF;
        return 0;
    }
    if(Key==Old_key) { //2da verificación
        if(Key!=Last_valid_key){ //evita múltiple detección
            *pkey=Key;
            Last_valid_key = Key;
            return 1;
        }
    }
    Old_key=Key; //1era verificación
    return 0;
}


