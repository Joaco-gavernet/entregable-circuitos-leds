ISR(TWI_vect) {
    switch (rtc_state) {
        case START:
            TWDR = WRITE_ADDRESS;
            rtc_state = WRITE_ADDRESS;
            break;

        case WRITE_ADDRESS:
            if (rtc_current_op == RTC_OP_READ) {
                TWDR = 0x00; // leer desde segundos
                rtc_state = WRITE_REGISTER;
            } else if (rtc_current_op == RTC_OP_WRITE_TIME) {
                TWDR = 0x00; // escribir desde segundos
                rtc_state = WRITE_REGISTER;
            } else if (rtc_current_op == RTC_OP_WRITE_ALARM) {
                TWDR = 0x07; // dirección de alarma 1: segundos
                rtc_state = WRITE_ALARM_REGISTER;
            } else if (rtc_current_op == RTC_OP_CLEAR_ALARM) {
                TWDR = 0x0F;  // Registro status DS3231
                rtc_state = WRITE_CLEAR_ALARM;
            } else if (rtc_current_op == RTC_OP_WRITE_ALARM_CTRL) {
                TWDR = 0x0E; // Dirección del registro CONTROL
                rtc_state = WRITE_ALARM_CTRL_REG;
            }
            break;

        case WRITE_REGISTER:
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);  // repeated start
            rtc_state = REPEATED_START;
            return;

        case REPEATED_START:
            TWDR = READ_ADDRESS;
            rtc_state = READ_ADDRESS;
            break;

        case READ_ADDRESS:
            rtc_state = READ_SECONDS;
            TWCR |= (1 << TWEA);
            break;

        case READ_SECONDS:
            rtc_seconds = TWDR;
            rtc_state = READ_MINUTES;
            TWCR |= (1 << TWEA);
            break;

        case READ_MINUTES:
            rtc_minutes = TWDR;
            rtc_state = READ_HOURS;
            TWCR |= (1 << TWEA);
            break;

        case READ_HOURS:
            rtc_hours = TWDR;
            rtc_state = DONE;
            break;

        case WRITE_ALARM_REGISTER:
            TWDR = rtc_write_s;
            rtc_state = WRITE_ALARM_SECONDS;
            break;

        case WRITE_ALARM_SECONDS:
            TWDR = rtc_write_m;
            rtc_state = WRITE_ALARM_MINUTES;
            break;

        case WRITE_ALARM_MINUTES:
            TWDR = rtc_write_h;
            rtc_state = WRITE_ALARM_HOURS;
            break;

        case WRITE_ALARM_HOURS:
            rtc_current_op = RTC_OP_WRITE_ALARM_CTRL;
            rtc_state = START;
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE); // nuevo START
            return;

        case WRITE_CLEAR_ALARM:
            TWDR = 0x00;  // Escribimos 0 en A1F (bit 0 del reg 0x0F)
            rtc_state = DONE;
            break;

        case WRITE_SECONDS:
            TWDR = rtc_write_s;
            rtc_state = WRITE_MINUTES;
            break;

        case WRITE_MINUTES:
            TWDR = rtc_write_m;
            rtc_state = WRITE_HOURS;
            break;

        case WRITE_HOURS:
            TWDR = rtc_write_h;
            rtc_state = DONE;
            break;

        case WRITE_ALARM_CTRL_REG:
            TWDR = 0b00000101; // INTCN=1, A1IE=1
            rtc_state = WRITE_ALARM_CTRL_VALUE;
            break;

        case WRITE_ALARM_CTRL_VALUE:
            rtc_state = DONE;
            break;
            
        case DONE:
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // STOP
            rtc_data_ready = 1;
            rtc_state = IDLE;
            rtc_current_op = RTC_OP_NONE;
            return;
    }

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
}

ISR(INT0_vect) {
    alarma_activa = 1;
    alarma_count = 0;
    rtc_clear_alarm_flag();  // Limpiamos A1F (bit 0 de reg 0x0F)
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

ISR(TIMER1_COMPA_vect) {
    timer_flag_1s = 1;
}