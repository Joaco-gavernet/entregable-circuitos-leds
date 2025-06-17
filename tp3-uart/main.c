#include "main.h"

volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t message_ready = 1;

void uart_setup(void) {
  // TO-DO: check how to setup for background-foreground
  SerialPort_Init(0x33);
  SerialPort_RX_Enable();
  SerialPort_RX_Interrupt_Enable();
  SerialPort_TX_Enable();
  sei();
}

ISR(USART_RX_vect) {
  char c = SerialPort_Recive_Data();

  if (c == '\n') {
    rx_buffer[rx_index] = '\0';     // End of string
    rx_index = 0;
    message_ready = 1;
  } else if (rx_index < RX_BUFFER_SIZE - 1) {
    rx_buffer[rx_index++] = c;
  }
}

int main(void) {
  uart_setup();

	SerialPort_Send_String("Ready. \n");

  while(1) {
    if (message_ready) {
      SerialPort_Wait_For_TX_Buffer_Free(); //9600bps formato 8N1, 10bits, 10.Tbit=10/9600=1ms 
      SerialPort_Send_Data('h');
    }
  }
	
	return 0;
}