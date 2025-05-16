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
	
  DDRC = ~(1<<PORTC0) & ~(1<<PORTC1); //Configuracion de PORT C como entrada
  PORTC |= (1 << PC0) | (1 << PC1); //Configuracion Pull-up C0 y C1 

  DDRD = 0xFF; //Configuracion PORT D como salida
  DDRB = (1<<PORTB3) | (1<<PORTB4); //Configuracion PB4 y PB3 como salida

  unsigned char act = 0, i = 0; 
  while(1){
    if (!(PINC & (1<<PINC0))) { //Chequeo pulsador (C0) y actualizacion de secuencia
		act++; 
		act %= 3; 
		i = 0; 
		_delay_ms(100); //Delay de debounce: Un pulso cambia una unica secuencia
	}
    if (!(PINC & (1<<PINC1))) {//Chequeo pulsador (C1) y muestra de secuencia
      if ((act + 1) & 1) PORTB |= (1<<3);  
      if ((act + 1) & 2) PORTB |= (1<<4); 
    } else {
      PORTB &= ~(1<<3);  //Si no esta pulsado mantenemos B3 y B4 apagados
      PORTB &= ~(1<<4); 
    } 

	// Cambios de estados/secuencias
    if (act == 0) PORTD = one[i], ++i, i %= 2; 
    else if (act == 1) PORTD = two[i], ++i, i %= 8; 
    else if (act == 2) PORTD = three[i], ++i, i %= 8; 
    _delay_ms(100); 
  }

  return 0;
}
