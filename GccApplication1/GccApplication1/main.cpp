#include <avr/io.h> // Registros del microcontrolador
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 8MHz
#include <util/delay.h> // Retardos por software/*

unsigned char one[] = {
	0b01010101,
	0b10101010
};
  
unsigned char two[] = {
	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000
};

unsigned char three[] = {
	0b10000000,
	0b00000001,
	0b01000000,
	0b00000010,
	0b00100000,
	0b00000100,
	0b00010000,
	0b00001000
};

int main(void)
{
  DDRC = ~(1<<PORTC0) & ~(1<<PORTC1); 
  PORTC |= (1 << PC0) | (1 << PC1); //Configuracion Pull-up C0 y C1

  DDRD = 0xFF; // Configura como salida


  unsigned char act = 0, i = 0; 
  while(1){
    if (!(PINC & (1<<PINC0))) {
		act++; 
		act %= 3; 
		i = 0; 
		_delay_ms(100); 
	}
    if (!(PINC & (1<<PINC1))) {
      if ((act + 1) & 1) PORTB |= (1<<3); 
      if ((act + 1) & 2) PORTB |= (1<<4); 
    } else {
      PORTB &= ~(1<<3); 
      PORTB &= ~(1<<3); 
    } 

    if (act == 0) PORTD = one[i], ++i, i %= 2; 
    else if (act == 1) PORTD = two[i], ++i, i %= 8; 
    else if (act == 2) PORTD = three[i], ++i, i %= 8; 
    _delay_ms(100); 
  }

  return 0;
}
