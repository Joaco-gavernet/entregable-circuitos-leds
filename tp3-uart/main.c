#include "main.h"

volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t message_ready = 0;
volatile uint8_t led_state = 0;  // 0 = OFF, 1 = ON
volatile uint8_t mode_change_requested = 0;
volatile uint8_t timer_flag_1s = 0;

void uart_setup(void) {
  SerialPort_Init(0x33);
  SerialPort_RX_Enable();
  SerialPort_RX_Interrupt_Enable();
  SerialPort_TX_Enable();
  sei();
}

uint8_t bcd_to_decimal(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8_t decimal_to_bcd(uint8_t decimal) {
    return ((decimal / 10) << 4) | (decimal % 10);
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

void process_command(char* command) {
  char* newline = strchr(command, '\n');
  if (newline) *newline = '\0';
  
  char* cr = strchr(command, '\r');
  if (cr) *cr = '\0';
  
  // Process the command
  if (strcmp(command, "1") == 0) {
    led_on();
    SerialPort_Send_String("LED turned ON\n");
    rtc_start_read_datetime(); // Start reading datetime when LED is turned ON
  } else if (strcmp(command, "0") == 0) {
    led_off();
    SerialPort_Send_String("LED turned OFF\n");
    // Stop reading datetime when LED is turned OFF
  } else if (strcmp(command, "toggle") == 0) {
    led_toggle();
    if (led_state) {
      SerialPort_Send_String("LED toggled ON\n");
      rtc_start_read_datetime(); // Start reading datetime when LED is toggled ON
    } else {
      SerialPort_Send_String("LED toggled OFF\n");
      // Stop reading datetime when LED is toggled OFF
    }
  } else if (strcmp(command, "status") == 0) {
    SerialPort_Send_String("LED is ");
    if (led_state) {
      SerialPort_Send_String("ON\n");
    } else {
      SerialPort_Send_String("OFF\n");
    }
  } else if (strcmp(command, "datetime") == 0) {
    SerialPort_Send_String("DateTime: ");
    char datetime_str[64];
    
    char debug_str[128];
    sprintf(debug_str, "%02X:%02X:%02X %02X/%02X/%02X/%02X  ", 
            rtc_datetime.seconds, rtc_datetime.minutes, rtc_datetime.hours,
            rtc_datetime.day_of_week, rtc_datetime.day_of_month, rtc_datetime.month, rtc_datetime.year);
    SerialPort_Send_String(debug_str);

    // Convert BCD to decimal for time fields
    uint8_t second = bcd_to_decimal(rtc_datetime.seconds);
    uint8_t minute = bcd_to_decimal(rtc_datetime.minutes & 0x7F);
    uint8_t hour   = bcd_to_decimal(rtc_datetime.hours & 0x3F);

    // Convert BCD to decimal for date fields
    uint8_t day_of_week = bcd_to_decimal(rtc_datetime.day_of_week & 0x07);
    uint8_t day_of_month = bcd_to_decimal(rtc_datetime.day_of_month & 0x3F);
    uint8_t month = bcd_to_decimal(rtc_datetime.month & 0x1F);
    uint8_t year = bcd_to_decimal(rtc_datetime.year);
    
    // Day of week names
    char* day_names[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char* day_name = (day_of_week >= 1 && day_of_week <= 7) ? day_names[day_of_week - 1] : "???";

    sprintf(datetime_str, "%s %02d/%02d/20%02d %02d:%02d:%02d\n", 
            day_name, day_of_month, month, year, hour, minute, second);
    SerialPort_Send_String(datetime_str);
    
    if (led_state == 1) {
      rtc_start_read_datetime(); // Reiniciar lectura para el siguiente segundo
    }
  } else if (strcmp(command, "setdate") == 0) {
    SerialPort_Send_String("Usage: setdate dow,dom,mo,yr (e.g., setdate 2,17,6,25)\n");
  } else if (strncmp(command, "setdate ", 8) == 0) {
    // Parse date command: setdate dow,dom,mo,yr
    char* date_part = command + 8;
    char* token = strtok(date_part, ",");
    if (token) {
      uint8_t dow = atoi(token);
      token = strtok(NULL, ",");
      if (token) {
        uint8_t dom = atoi(token);
        token = strtok(NULL, ",");
        if (token) {
          uint8_t mo = atoi(token);
          token = strtok(NULL, ",");
          if (token) {
            uint8_t yr = atoi(token);
            
            // Validate input ranges
            if (dow < 1 || dow > 7) {
              SerialPort_Send_String("Error: Day of week must be 1-7 (1=Sun, 2=Mon, ..., 7=Sat)\n");
            } else if (dom < 1 || dom > 31) {
              SerialPort_Send_String("Error: Day of month must be 1-31\n");
            } else if (mo < 1 || mo > 12) {
              SerialPort_Send_String("Error: Month must be 1-12\n");
            } else if (yr > 99) {
              SerialPort_Send_String("Error: Year must be 0-99 (20xx)\n");
            } else {
              // Create datetime structure and set date
              rtc_datetime_t new_datetime;
              rtc_get_datetime(&new_datetime); // Get current values
              new_datetime.day_of_week = decimal_to_bcd(dow);
              new_datetime.day_of_month = decimal_to_bcd(dom);
              new_datetime.month = decimal_to_bcd(mo);
              new_datetime.year = decimal_to_bcd(yr);
              
              rtc_set_datetime(&new_datetime);
              
              SerialPort_Send_String("Date set to: ");
              char* day_names[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
              char* day_name = (dow >= 1 && dow <= 7) ? day_names[dow - 1] : "???";
              char date_str[32];
              sprintf(date_str, "%s %02d/%02d/20%02d\n", day_name, dom, mo, yr);
              SerialPort_Send_String(date_str);
            }
          } else {
            SerialPort_Send_String("Invalid date format. Use: setdate dow,dom,mo,yr\n");
          }
        } else {
          SerialPort_Send_String("Invalid date format. Use: setdate dow,dom,mo,yr\n");
        }
      } else {
        SerialPort_Send_String("Invalid date format. Use: setdate dow,dom,mo,yr\n");
      }
    } else {
      SerialPort_Send_String("Invalid date format. Use: setdate dow,dom,mo,yr\n");
    }
  } else if (strcmp(command, "settime") == 0) {
    SerialPort_Send_String("Usage: settime h,m,s (e.g., settime 14,30,0)\n");
  } else if (strncmp(command, "settime ", 8) == 0) {
    // Parse time command: settime h,m,s
    char* time_part = command + 8;
    char* token = strtok(time_part, ",");
    if (token) {
      uint8_t h = atoi(token);
      token = strtok(NULL, ",");
      if (token) {
        uint8_t m = atoi(token);
        token = strtok(NULL, ",");
        if (token) {
          uint8_t s = atoi(token);
          
          // Validate input ranges
          if (h > 23) {
            SerialPort_Send_String("Error: Hour must be 0-23\n");
          } else if (m > 59) {
            SerialPort_Send_String("Error: Minute must be 0-59\n");
          } else if (s > 59) {
            SerialPort_Send_String("Error: Second must be 0-59\n");
          } else {
            // Create datetime structure and set time
            rtc_datetime_t new_datetime;
            rtc_get_datetime(&new_datetime); // Get current values
            new_datetime.hours = decimal_to_bcd(h);
            new_datetime.minutes = decimal_to_bcd(m);
            new_datetime.seconds = decimal_to_bcd(s);
            
            rtc_set_datetime(&new_datetime);
            
            SerialPort_Send_String("Time set to: ");
            char time_str[32];
            sprintf(time_str, "%02d:%02d:%02d\n", h, m, s);
            SerialPort_Send_String(time_str);
          }
        } else {
          SerialPort_Send_String("Invalid time format. Use: settime h,m,s\n");
        }
      } else {
        SerialPort_Send_String("Invalid time format. Use: settime h,m,s\n");
      }
    } else {
      SerialPort_Send_String("Invalid time format. Use: settime h,m,s\n");
    }
  } else if (strcmp(command, "set_alarm") == 0) {
    SerialPort_Send_String("Usage: set_alarm h,m,s (e.g., set_alarm 14,30,0)\n");
  } else if (strncmp(command, "set_alarm ", 10) == 0) {
    // Parse alarm command: set_alarm h,m,s
    char* alarm_part = command + 10;
    char* token = strtok(alarm_part, ",");
    if (token) {
      uint8_t h = atoi(token);
      token = strtok(NULL, ",");
      if (token) {
        uint8_t m = atoi(token);
        token = strtok(NULL, ",");
        if (token) {
          uint8_t s = atoi(token);
          
          // Validate input ranges
          if (h > 23) {
            SerialPort_Send_String("Error: Hour must be 0-23\n");
          } else if (m > 59) {
            SerialPort_Send_String("Error: Minute must be 0-59\n");
          } else if (s > 59) {
            SerialPort_Send_String("Error: Second must be 0-59\n");
          } else {
            // Set alarm using rtc_set_alarm function
            rtc_datetime_t new_datetime;
            rtc_get_datetime(&new_datetime); // Get current values
            new_datetime.hours = decimal_to_bcd(h);
            new_datetime.minutes = decimal_to_bcd(m);
            new_datetime.seconds = decimal_to_bcd(s);
            
            rtc_set_alarm(&new_datetime);
            
            SerialPort_Send_String("Alarm set to: ");
            char alarm_str[32];
            sprintf(alarm_str, "%02d:%02d:%02d\n", h, m, s);
            SerialPort_Send_String(alarm_str);
          }
        } else {
          SerialPort_Send_String("Invalid alarm format. Use: set_alarm h,m,s\n");
        }
      } else {
        SerialPort_Send_String("Invalid alarm format. Use: set_alarm h,m,s\n");
      }
    } else {
      SerialPort_Send_String("Invalid alarm format. Use: set_alarm h,m,s\n");
    }
  } else if (strcmp(command, "help") == 0) {
    SerialPort_Send_String("Commands:\n");
    SerialPort_Send_String("  1 - Turn LED ON (starts datetime display)\n");
    SerialPort_Send_String("  0 - Turn LED OFF (stops datetime display)\n");
    SerialPort_Send_String("  toggle - Toggle LED state\n");
    SerialPort_Send_String("  status - Show LED status\n");
    SerialPort_Send_String("  datetime - Read and display date and time once\n");
    SerialPort_Send_String("  setdate dow,dom,mo,yr - Set date (e.g., setdate 2,17,6,25)\n");
    SerialPort_Send_String("  settime h,m,s - Set time (e.g., settime 14,30,0)\n");
    SerialPort_Send_String("  set_alarm h,m,s - Set alarm (e.g., set_alarm 14,30,0)\n");
    SerialPort_Send_String("  help - Show this help\n");
  } else {
    SerialPort_Send_String("Unknown command: '");
    SerialPort_Send_String(command);
    SerialPort_Send_String("'\nType 'help' for available commands\n");
  }
}

void send_data_message(void) {
  SerialPort_Send_String("Data message from Arduino: LED is ");
  if (led_state) SerialPort_Send_String("ON\n");
  else SerialPort_Send_String("OFF\n");
}

// === Timer 1s para tareas cíclicas ===
void timer1_init(void) {
    TCCR1B |= (1 << WGM12);                  // CTC
    OCR1A = 15624;                           // 1s @ 8MHz y prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);     // Prescaler 1024
    TIMSK1 |= (1 << OCIE1A);                 // Enable compare interrupt
}

// === INT0 para interrupción del RTC ===
void ext_int0_init(void) {
    EICRA |= (1 << ISC01);   // Flanco descendente
    EICRA &= ~(1 << ISC00);
    EIMSK |= (1 << INT0);    // Habilitar INT0
}

int main(void) {
  uart_setup();
  led_setup();
  ext_int0_init(); // Habilitar interrupción INT0 (ALARMA)
  timer1_init(); // Inicializar interrupción de 1s para tareas periódicas
  rtc_init(); // Inicializar RTC

  // Foreground/Background or event-driven architecture
  while(1) {
    if (message_ready) {
      message_ready = 0; 
      process_command((char*)rx_buffer);
    }

    if (timer_flag_1s) {
      timer_flag_1s = 0;
      if (rtc_data_ready && led_state == 1) {
        SerialPort_Send_String("DateTime: ");
        char datetime_str[64];

        // Convert BCD to decimal for time fields
        uint8_t second = bcd_to_decimal(rtc_datetime.seconds);
        uint8_t minute = bcd_to_decimal(rtc_datetime.minutes & 0x7F);
        uint8_t hour   = bcd_to_decimal(rtc_datetime.hours & 0x3F);

        // Convert BCD to decimal for date fields
        uint8_t day_of_week = bcd_to_decimal(rtc_datetime.day_of_week & 0x07);
        uint8_t day_of_month = bcd_to_decimal(rtc_datetime.day_of_month & 0x3F);
        uint8_t month = bcd_to_decimal(rtc_datetime.month & 0x1F);
        uint8_t year = bcd_to_decimal(rtc_datetime.year);
        
        // Day of week names
        char* day_names[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        char* day_name = (day_of_week >= 1 && day_of_week <= 7) ? day_names[day_of_week - 1] : "???";

        sprintf(datetime_str, "%s %02d/%02d/20%02d %02d:%02d:%02d\n", 
                day_name, day_of_month, month, year, hour, minute, second);
        SerialPort_Send_String(datetime_str);
        
        rtc_start_read_datetime(); // Continue reading for next second
      }
    }

    if (alarm_active) {
      alarm_count++;
      char alarm_count_str[10];
      itoa(alarm_count, alarm_count_str, 10);
      SerialPort_Send_String("Alarm count: ");
      SerialPort_Send_String(alarm_count_str);
      SerialPort_Send_String("\n");

      if (alarm_count >= 5) {
        alarm_active = 0; // detener alarma
      }
    }
  }

  return 0;
}
