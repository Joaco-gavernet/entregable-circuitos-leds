#include "main.h"

volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t message_ready = 0;

// LED control variables
volatile uint8_t led_state = 0;  // 0 = OFF, 1 = ON

// State machine variables
volatile uint8_t current_mode = 0;  // 0 = Command Mode, 1 = Data Mode
volatile uint8_t mode_change_requested = 0;

void uart_setup(void) {
  SerialPort_Init(0x33);
  SerialPort_RX_Enable();
  SerialPort_RX_Interrupt_Enable();
  SerialPort_TX_Enable();
  sei();
}

void led_setup(void) {
  DDRB |= (1 << PB5);  // Set PB5 (pin 13) as output for built-in LED
  PORTB &= ~(1 << PB5);  // Start with LED OFF
}

void led_on(void) {
  PORTB |= (1 << PB5);  // Set LED ON
  led_state = 1;
}

void led_off(void) {
  PORTB &= ~(1 << PB5);  // Set LED OFF
  led_state = 0;
}

void led_toggle(void) {
  if (led_state) led_off();
  else led_on();
}

void switch_to_command_mode(void) {
  current_mode = 0;
}

void switch_to_data_mode(void) {
  current_mode = 1;
}

void process_command(char* command) {
  char* newline = strchr(command, '\n');
  if (newline) *newline = '\0';
  
  char* cr = strchr(command, '\r');
  if (cr) *cr = '\0';
  
  // Process the command
  if (strcmp(command, "1") == 0) {
    led_on();
    switch_to_data_mode();
    SerialPort_Send_String("LED turned ON\n");
    switch_to_command_mode();
  } else if (strcmp(command, "0") == 0) {
    led_off();
    switch_to_data_mode();
    SerialPort_Send_String("LED turned OFF\n");
    switch_to_command_mode();
  } else if (strcmp(command, "toggle") == 0) {
    led_toggle();
    switch_to_data_mode();
    if (led_state) {
      SerialPort_Send_String("LED toggled ON\n");
    } else {
      SerialPort_Send_String("LED toggled OFF\n");
    }
    switch_to_command_mode();
  } else if (strcmp(command, "status") == 0) {
    switch_to_data_mode();
    SerialPort_Send_String("LED is ");
    if (led_state) {
      SerialPort_Send_String("ON\n");
    } else {
      SerialPort_Send_String("OFF\n");
    }
    switch_to_command_mode();
  } else if (strcmp(command, "help") == 0) {
    switch_to_data_mode();
    SerialPort_Send_String("Commands:\n");
    SerialPort_Send_String("  1 - Turn LED ON\n");
    SerialPort_Send_String("  0 - Turn LED OFF\n");
    SerialPort_Send_String("  toggle - Toggle LED state\n");
    SerialPort_Send_String("  status - Show LED status\n");
    SerialPort_Send_String("  help - Show this help\n");
    switch_to_command_mode();
  } else {
    switch_to_data_mode();
    SerialPort_Send_String("Unknown command: '");
    SerialPort_Send_String(command);
    SerialPort_Send_String("'\nType 'help' for available commands\n");
    switch_to_command_mode();
  }
}

void send_data_message(void) {
  SerialPort_Send_String("Data message from Arduino: LED is ");
  if (led_state) SerialPort_Send_String("ON\n");
  else SerialPort_Send_String("OFF\n");
}

ISR(USART_RX_vect) {
  char c = SerialPort_Recive_Data();

  if (c == '\n' || c == '\r') {
    if (rx_index > 0) {  // Only process if we have data
      rx_buffer[rx_index] = '\0';     // End of string
      message_ready = 1;
    }
    rx_index = 0;
  } else if (rx_index < RX_BUFFER_SIZE - 1) {
    rx_buffer[rx_index++] = c;
  }
}

int main(void) {
  uart_setup();
  led_setup();

  current_mode = 0;

  // Foreground/Background or event-driven architecture
  while(1) {
    if (current_mode == 0) {
      // Command Mode - wait for user commands
      if (message_ready) {
        message_ready = 0; 
        process_command((char*)rx_buffer);
      }
    } else {
      // Data Mode - send periodic data
      // TO-DO: use this space to make the data reception from RTC, ONLY when activated
      send_data_message();
      _delay_ms(2000);
    }
  }

  return 0;
}