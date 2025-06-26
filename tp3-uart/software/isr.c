#include "main.h"

/***************************************************************
* Propósito de la función: FSM encargada del protocolo I2C para comunicacion con el RTC-DS3231
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
ISR(TWI_vect) {
    switch (rtc_state) {
        case RTC_START:
            TWDR = WRITE_ADDRESS;
            rtc_state = RTC_WRITE_ADDRESS;
            break;

        case RTC_WRITE_ADDRESS:
            if (rtc_current_op == RTC_OP_READ_DATETIME) {
                TWDR = 0x00; // leer desde segundos
                rtc_state = RTC_WRITE_REGISTER;
            } else if (rtc_current_op == RTC_OP_WRITE_DATETIME) {
                TWDR = 0x00; // escribir desde segundos
                rtc_state = RTC_WRITE_SECONDS;
            } else if (rtc_current_op == RTC_OP_WRITE_ALARM) {
                TWDR = 0x07; // dirección de alarma 1: segundos
                rtc_state = RTC_WRITE_ALARM_REGISTER;
            } else if (rtc_current_op == RTC_OP_CLEAR_ALARM) {
                TWDR = 0x0F;  // Registro status DS3231
                rtc_state = RTC_WRITE_CLEAR_ALARM;
            } else if (rtc_current_op == RTC_OP_WRITE_ALARM_CTRL) {
                TWDR = 0x0E; // Dirección del registro CONTROL
                rtc_state = RTC_WRITE_ALARM_CTRL_REG;
            }
            break;

        case RTC_WRITE_ALARM_REGISTER:
            TWDR = rtc_write_datetime.seconds & 0x7F; // A1M1 = 0 (match seconds)
            rtc_state = RTC_WRITE_ALARM_SECONDS;
            break;

        case RTC_WRITE_ALARM_SECONDS:
            TWDR = rtc_write_datetime.minutes & 0x7F; // A1M2 = 0
            rtc_state = RTC_WRITE_ALARM_MINUTES;
            break;

        case RTC_WRITE_ALARM_MINUTES:
            TWDR = rtc_write_datetime.hours   & 0x7F; // A1M3 = 0
            rtc_state = RTC_WRITE_ALARM_HOURS;
            break;

        case RTC_WRITE_ALARM_HOURS:
            TWDR = 0x80; // A1M4 = 1, DY/DT = 0 → ignore date
            rtc_state = RTC_WRITE_ALARM_DAY;
            break;

        case RTC_WRITE_ALARM_DAY:
            rtc_current_op = RTC_OP_WRITE_ALARM_CTRL;
            rtc_state = RTC_START;
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
            return;

        case RTC_WRITE_CLEAR_ALARM:
            TWDR = 0x00;  // Escribimos 0 en A1F (bit 0 del reg 0x0F)
            rtc_state = RTC_DONE;
            break;

        case RTC_WRITE_ALARM_CTRL_REG:
            TWDR = 0b00000101; // INTCN=1, A1IE=1
            rtc_state = RTC_WRITE_ALARM_CTRL_VALUE;
            break;

        case RTC_WRITE_ALARM_CTRL_VALUE:
            rtc_state = RTC_DONE;
            break;

        case RTC_WRITE_REGISTER:
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);  // repeated start
            rtc_state = RTC_REPEATED_START;
            return;

        case RTC_REPEATED_START:
            TWDR = READ_ADDRESS;
            rtc_state = RTC_READ_ADDRESS;
            break;

        case RTC_READ_ADDRESS:
            rtc_state = RTC_READ_SECONDS;
            TWCR |= (1 << TWEA) | (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
            return;

        case RTC_READ_SECONDS:
            rtc_datetime.seconds = TWDR;
            rtc_state = RTC_READ_MINUTES;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  // ACK
            return;

        case RTC_READ_MINUTES:
            rtc_datetime.minutes = TWDR;
            rtc_state = RTC_READ_HOURS;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  // ACK
            return;

        case RTC_READ_HOURS:
            rtc_datetime.hours = TWDR;
            rtc_state = RTC_READ_DAY_OF_WEEK;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  // ACK
            return;

        case RTC_READ_DAY_OF_WEEK:
            rtc_datetime.day_of_week = TWDR;
            rtc_state = RTC_READ_DAY_OF_MONTH;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  // ACK
            return;

        case RTC_READ_DAY_OF_MONTH:
            rtc_datetime.day_of_month = TWDR;
            rtc_state = RTC_READ_MONTH;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  // ACK
            return;

        case RTC_READ_MONTH:
            rtc_datetime.month = TWDR;
            rtc_state = RTC_READ_YEAR;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  // ACK
            return;

        case RTC_READ_YEAR:
            rtc_datetime.year = TWDR;
            rtc_state = RTC_DONE;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);  // NACK
            return;

        case RTC_WRITE_SECONDS:
            TWDR = rtc_write_datetime.seconds;
            rtc_state = RTC_WRITE_MINUTES;
            break;

        case RTC_WRITE_MINUTES:
            TWDR = rtc_write_datetime.minutes;
            rtc_state = RTC_WRITE_HOURS;
            break;

        case RTC_WRITE_HOURS:
            TWDR = rtc_write_datetime.hours;
            rtc_state = RTC_WRITE_DAY_OF_WEEK;
            break;

        case RTC_WRITE_DAY_OF_WEEK:
            TWDR = rtc_write_datetime.day_of_week;
            rtc_state = RTC_WRITE_DAY_OF_MONTH;
            break;

        case RTC_WRITE_DAY_OF_MONTH:
            TWDR = rtc_write_datetime.day_of_month;
            rtc_state = RTC_WRITE_MONTH;
            break;

        case RTC_WRITE_MONTH:
            TWDR = rtc_write_datetime.month;
            rtc_state = RTC_WRITE_YEAR;
            break;

        case RTC_WRITE_YEAR:
            TWDR = rtc_write_datetime.year;
            rtc_state = RTC_DONE;
            break;
            
        case RTC_DONE:
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // STOP
            rtc_data_ready = 1;
            rtc_state = RTC_IDLE;
            rtc_current_op = RTC_OP_NONE;
            return;
    }

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
}

/***************************************************************
* Propósito de la función: Maneja la interrupción externa INT0 cuando se activa la alarma del RTC.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
ISR(INT0_vect) {
    alarm_active = 1;
    alarm_count = 0;
    rtc_clear_alarm_flag();  // Limpiamos A1F (bit 0 de reg 0x0F)
}

/***************************************************************
* Propósito de la función: Maneja la interrupción de recepción UART, almacenando los caracteres recibidos en el buffer.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
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

/***************************************************************
* Propósito de la función: Maneja la interrupción del Timer1 (1s) para tareas periódicas (ej: fecha y hora)
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
ISR(TIMER1_COMPA_vect) {
    timer_flag_1s = 1;
}