#include "main.h"
#include "lcd.h"
#include "teclado4x4.h"



char * palabras [] = {"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve", "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa",
"SERIE", "SalUd", "Salud", "Silla", "Tecla", "Valor", "Verde", "YnHRz", "hARdD", "silla"};

void clearLCD(uint8_t x, uint8_t y) {
    LCDGotoXY(x, y);  // Move cursor to desired position
    LCDstring((uint8_t*)" ", 1);  // Write a space character
}



void main(void) {
    LCD_Init();
    LCDclr();
    
    // Initialize keypad
    DDRB = 0x0F;  // Set first 4 pins as outputs
    PORTB = 0x0F; // Set them high
    
    uint8_t key = 0;
    char key_str[2];  // Buffer for key number string
    
    while(1) {
        if(KEYPAD_Scan(&key)) {  // If a key is pressed
            LCDclr();  // Clear display
            LCDGotoXY(0, 0);  // Move to first line
            
            // Convert key number to string
            key_str[0] = '0' + (key / 10);  // Tens digit
            key_str[1] = '0' + (key % 10);  // Units digit
            
            // Display key number
            if(key < 10) {
                LCDstring((uint8_t*)&key_str[1], 1);  // Single digit
            } else {
                LCDstring((uint8_t*)key_str, 2);  // Two digits
            }
        }
        _delay_ms(100);  
    }
}