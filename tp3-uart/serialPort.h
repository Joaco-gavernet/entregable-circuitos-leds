/*
 * serialPort.h
 *
 * Created: 07/10/2020 03:02:42 p. m.
 *  Author: vfperri
 */ 

#ifndef SERIALPORT_H_
	#define SERIALPORT_H_

	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <stdint.h>

	// Buffer sizes
	#define TX_BUFFER_SIZE 64
	#define RX_BUFFER_SIZE 64

	// Buffer declarations
	extern volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
	extern volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
	extern volatile uint8_t tx_head;
	extern volatile uint8_t tx_tail;
	extern volatile uint8_t rx_head;
	extern volatile uint8_t rx_tail;

	// -------- Prototipos de funciones Publicas ---------------
	
	// Inicializacion de Puerto Serie
	void SerialPort_Init(uint8_t config);

	// Inicializacion de Transmisor
	void SerialPort_TX_Enable(void);
	void SerialPort_TX_Interrupt_Enable(void);
	void SerialPort_TX_Interrupt_Disable(void);
	
	// Inicializacion de Receptor
	void SerialPort_RX_Enable(void);
	void SerialPort_RX_Interrupt_Enable(void);
	void SerialPort_Wait_For_TX_Buffer_Free(void);
	
	// Transmision
	void SerialPort_Send_Data(char data);
	void SerialPort_Send_String(char * msg);
	void SerialPort_Send_uint8_t(uint8_t num);
	void SerialPort_send_int16_t(int val, unsigned int field_length);
	void SerialPort_Process(void);  // New function for command processing

	// Recepcion
	char SerialPort_Receive_Data(void);

	// Interrupt Service Routines
	ISR(USART_RX_vect);
	ISR(USART_UDRE_vect);

#endif /* SERIALPORT_H_ */