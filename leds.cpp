#include <avr/io.h> // Registros del microcontrolador
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 8MHz
#include <util/delay.h> // Retardos por software/*



int main(void)
{
  DDRC = ~(1<<PORTC0) & ~(1<<PORTC1); // Configura como entrada
  DDRD = 0xFF; // Configura como salida

  vector<bitset<8>> one = {
    bitset<8>("01010101"), 
    bitset<8>("10101010") 
  }; 

  vector<bitset<8>> two = {
    bitset<8>("00000001"), 
    bitset<8>("00000010"), 
    bitset<8>("00000100"), 
    bitset<8>("00001000"), 
    bitset<8>("00010000"), 
    bitset<8>("00100000"), 
    bitset<8>("01000000"), 
    bitset<8>("10000000") 
  }; 

  vector<bitset<8>> three = {
    bitset<8>("10000000"), 
    bitset<8>("00000001"), 
    bitset<8>("01000000"), 
    bitset<8>("00000010"), 
    bitset<8>("00100000"), 
    bitset<8>("00000100"), 
    bitset<8>("00010000"), 
    bitset<8>("00001000"), 
  }; 

  unsigned char act = 0, i = 0; 
  while(1){
    if (PINC & (1<<PINC0)) (act++) %= 3; 
    if (PINC & (1<<PINC1)) {
      PORTB |= (1<<3); 
      PORTB |= (1<<3); 
    } else {
      PORTB &= ~(1<<3); 
      PORTB &= ~(1<<3); 
    } 

    if (act == 0) PORTD = one[i], (++i) %= 2; 
    else if (act == 1) PORTD = two[i], (++i) %= 8; 
    else if (act == 2) PORTD = three[i], (++i) %= 8; 
    _delay_ms(100); 
  }

  return 0;
}
