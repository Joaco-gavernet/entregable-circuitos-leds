#include "serialPort.h"

int main(void) {
    // Initialize UART at 9600 baud rate
    // For 8MHz crystal: 0x33, for 4MHz crystal: 0x25
    SerialPort_Init(0x33);
    
    // Enable UART transmitter and receiver
    SerialPort_TX_Enable();
    SerialPort_RX_Enable();
    
    // Enable UART interrupts
    SerialPort_TX_Interrupt_Enable();
    SerialPort_RX_Interrupt_Enable();
    
    // Enable global interrupts
    sei();
    
    // Send welcome message
    SerialPort_Send_String("UART Communication Ready\r\n");
    
    while(1) {
        SerialPort_Process();  // Process any received commands
    }
}