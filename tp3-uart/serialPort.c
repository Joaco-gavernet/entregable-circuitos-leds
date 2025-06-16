#include "serialPort.h"

// Buffer definitions
volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

// Command processing state
typedef enum {
	IDLE,
	WAITING_FOR_COMMAND,
	PROCESSING_COMMAND
} State_t;

static volatile State_t currentState = IDLE;
static volatile uint8_t receivedCommand = 0;
static volatile uint8_t commandProcessed = 1;  // Start as processed to allow new commands

// Function prototypes
static void processCommand(uint8_t cmd);
static void sendResponse(const char* response);

// ------ Definiciones de Funciones Públicas -------------------

// Inicialización de Puerto Serie
void SerialPort_Init(uint8_t config){
	// config = 0x33 ==> Configuro UART 9600bps, 8 bit data, 1 stop @ F_CPU = 8MHz.
	// config = 0x25 ==> Configuro UART 9600bps, 8 bit data, 1 stop @ F_CPU = 4Hz.
	UCSR0B = 0;
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UBRR0H = (unsigned char)(config>>8);
	UBRR0L = (unsigned char)config;
}

// Inicialización de Transmisor
void SerialPort_TX_Enable(void){
	UCSR0B |= (1<<TXEN0);
}

void SerialPort_TX_Interrupt_Enable(void){
	UCSR0B |= (1<<UDRIE0);
}

void SerialPort_TX_Interrupt_Disable(void){
	UCSR0B &= ~(1<<UDRIE0);
}

// Inicialización de Receptor
void SerialPort_RX_Enable(void){
	UCSR0B |= (1<<RXEN0);
}

void SerialPort_RX_Interrupt_Enable(void){
	UCSR0B |= (1<<RXCIE0);
}

void SerialPort_Wait_For_TX_Buffer_Free(void) {
	while(!(UCSR0A & (1<<UDRE0)));
}

// Transmisión
void SerialPort_Send_Data(char data){
	SerialPort_Wait_For_TX_Buffer_Free();
	UDR0 = data;
}

void SerialPort_Send_String(char * msg){
	while(*msg){
		SerialPort_Send_Data(*msg++);
	}
}

// Reception
char SerialPort_Receive_Data(void){
	uint8_t data;
	
	// Wait until there's data in the buffer
	while(rx_head == rx_tail);
	
	data = rx_buffer[rx_tail];
	rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
	
	return data;
}

// Interrupt Service Routines
ISR(USART_RX_vect){
	uint8_t received = UDR0;
	
	// Only process new commands if we're ready
	if (commandProcessed) {
		receivedCommand = received;
		commandProcessed = 0;
		currentState = WAITING_FOR_COMMAND;
	}
}

ISR(USART_UDRE_vect){
	if(tx_head != tx_tail){
		UDR0 = tx_buffer[tx_tail];
		tx_tail = (tx_tail + 1) % TX_BUFFER_SIZE;
	} else {
		// Buffer empty, disable interrupt
		UCSR0B &= ~(1<<UDRIE0);
	}
}

void SerialPort_Send_uint8_t(uint8_t num){
	SerialPort_Wait_For_TX_Buffer_Free();
	SerialPort_Send_Data('0' + (num/100));
	num %= 100;
	SerialPort_Wait_For_TX_Buffer_Free();
	SerialPort_Send_Data('0' + (num/10));
	num %= 10;
	SerialPort_Wait_For_TX_Buffer_Free();
	SerialPort_Send_Data('0' + num);
}

/***************************************************************
	This function writes a integer type value to UART
	Arguments:
	1)int val	: Value to print
	2)unsigned int field_length :total length of field in which the value is printed
	must be between 1-5 if it is -1 the field length is no of digits in the val
****************************************************************/
void SerialPort_send_int16_t(int val,unsigned int field_length)
{
	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(val)
	{
	str[i]=val%10;
	val=val/10;
	i--;
	}
	if(field_length==-1)
		while(str[j]==0) j++;
	else
		j=5-field_length;

	if(val<0) {
		SerialPort_Wait_For_TX_Buffer_Free();
		SerialPort_Send_Data('-');
		}
	for(i=j;i<5;i++)
	{
	SerialPort_Wait_For_TX_Buffer_Free();
	SerialPort_Send_Data('0'+str[i]);
	}
}

// Process received commands
static void processCommand(uint8_t cmd) {
	switch(cmd) {
		case '1':
			sendResponse("Command 1 received\r\n");
			break;
		case '2':
			sendResponse("Command 2 received\r\n");
			break;
		case '3':
			sendResponse("Command 3 received\r\n");
			break;
		case 'h':
			sendResponse("Available commands:\r\n");
			sendResponse("1 - Command 1\r\n");
			sendResponse("2 - Command 2\r\n");
			sendResponse("3 - Command 3\r\n");
			sendResponse("h - Help\r\n");
			break;
		default:
			sendResponse("Unknown command\r\n");
			break;
	}
}

// Send response through UART
static void sendResponse(const char* response) {
	SerialPort_Send_String((char*)response);
}

// Main processing function to be called from main loop
void SerialPort_Process(void) {
	if (!commandProcessed && currentState == WAITING_FOR_COMMAND) {
		processCommand(receivedCommand);
		commandProcessed = 1;
		currentState = IDLE;
	}
}