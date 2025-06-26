#include "command.h"
#include "led.h"

/***************************************************************
* Propósito de la función: Configura una nueva fecha en el RTC, manteniendo la hora actual.
* Parámetros de entrada (tipo, rango y formato):
*   - dom: uint8_t, día del mes (1-31)
*   - mo: uint8_t, mes (1-12)
*   - yr: uint8_t, año (0-99, representa 2000-2099)
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void set_date(uint8_t dom, uint8_t mo, uint8_t yr) {
    rtc_datetime_t new_datetime;
    rtc_get_datetime(&new_datetime);
    new_datetime.day_of_week = decimal_to_bcd(2); // Hardcoded to Monday
    new_datetime.day_of_month = decimal_to_bcd(dom);
    new_datetime.month = decimal_to_bcd(mo);
    new_datetime.year = decimal_to_bcd(yr);
    
    rtc_set_datetime(&new_datetime);
    
    SerialPort_Send_String("Nueva fecha: ");
    char date_str[32];
    sprintf(date_str, "%02d/%02d/20%02d\n", dom, mo, yr);
    SerialPort_Send_String(date_str);
}

/***************************************************************
* Propósito de la función: Configura una nueva hora en el RTC, manteniendo la fecha actual.
* Parámetros de entrada (tipo, rango y formato):
*   - h: uint8_t, hora (0-23)
*   - m: uint8_t, minutos (0-59)
*   - s: uint8_t, segundos (0-59)
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void set_time(uint8_t h, uint8_t m, uint8_t s) {
    rtc_datetime_t new_datetime;
    rtc_get_datetime(&new_datetime);
    new_datetime.hours = decimal_to_bcd(h);
    new_datetime.minutes = decimal_to_bcd(m);
    new_datetime.seconds = decimal_to_bcd(s);
    
    rtc_set_datetime(&new_datetime);
    
    SerialPort_Send_String("Nueva hora: ");
    char time_str[32];
    sprintf(time_str, "%02d:%02d:%02d\n", h, m, s);
    SerialPort_Send_String(time_str);
}

/***************************************************************
* Propósito de la función: Configura una nueva alarma en el RTC.
* Parámetros de entrada (tipo, rango y formato):
*   - h: uint8_t, hora (0-23)
*   - m: uint8_t, minutos (0-59)
*   - s: uint8_t, segundos (0-59)
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void set_alarm(uint8_t h, uint8_t m, uint8_t s) {
    rtc_datetime_t new_datetime;
    rtc_get_datetime(&new_datetime);
    new_datetime.hours = decimal_to_bcd(h);
    new_datetime.minutes = decimal_to_bcd(m);
    new_datetime.seconds = decimal_to_bcd(s);
    
    rtc_set_alarm(&new_datetime);
    
    SerialPort_Send_String("Alarma configurada: ");
    char alarm_str[32];
    sprintf(alarm_str, "%02d:%02d:%02d\n", h, m, s);
    SerialPort_Send_String(alarm_str);
}

/***************************************************************
* Propósito de la función: Muestra la fecha y hora actual en formato legible.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void display_datetime(void) {
    // Convert BCD to decimal for time fields
    uint8_t second = bcd_to_decimal(rtc_datetime.seconds);
    uint8_t minute = bcd_to_decimal(rtc_datetime.minutes & 0x7F);
    uint8_t hour = bcd_to_decimal(rtc_datetime.hours & 0x3F);
    
    // Convert BCD to decimal for date fields
    uint8_t day_of_month = bcd_to_decimal(rtc_datetime.day_of_month & 0x3F);
    uint8_t month = bcd_to_decimal(rtc_datetime.month & 0x1F);
    uint8_t year = bcd_to_decimal(rtc_datetime.year);
    
    char date_str[32];
    char time_str[32];
    
    sprintf(date_str, "FECHA: %02d/%02d/20%02d", day_of_month, month, year);
    sprintf(time_str, "HORA: %02d:%02d:%02d\n", hour, minute, second);
    
    SerialPort_Send_String(date_str);
    SerialPort_Send_String(" ");
    SerialPort_Send_String(time_str);
}

/***************************************************************
* Propósito de la función: Muestra la lista de comandos disponibles y su formato.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void display_help(void) {
    SerialPort_Send_String("Commands:\n");
    SerialPort_Send_String("  ON - Display date and time\n");
    SerialPort_Send_String("  OFF - Stop display date and time\n");
    SerialPort_Send_String("  SET DATE dom,mo,yr - Set date (e.g., SET DATE 17,6,25)\n");
    SerialPort_Send_String("  SET TIME h,m,s - Set time (e.g., SET TIME 14,30,0)\n");
    SerialPort_Send_String("  SET ALARM h,m,s - Set alarm (e.g., SET ALARM 14,30,0)\n");
    SerialPort_Send_String("  HELP - Show this help\n");
}

/***************************************************************
* Propósito de la función: Maneja la activación de la alarma, contando las veces que suena y deteniéndola después de 5 veces.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void handle_alarm(void) {
    alarm_count++;
    char alarm_count_str[10];
    itoa(alarm_count, alarm_count_str, 10);
    SerialPort_Send_String("Alarm count: ");
    SerialPort_Send_String(alarm_count_str);
    SerialPort_Send_String("\n");

    if (alarm_count >= 5) {
        alarm_active = 0;
    }
}

/***************************************************************
* Propósito de la función: Procesa los comandos recibidos por UART y ejecuta la acción correspondiente.
* Parámetros de entrada (tipo, rango y formato):
*   - command: char*, cadena de caracteres con el comando a procesar
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void process_command(char* command) {
    char* newline = strchr(command, '\n');
    if (newline) *newline = '\0';
    
    char* cr = strchr(command, '\r');
    if (cr) *cr = '\0';
    
    if (strcmp(command, "ON") == 0) {
        led_on();
        mode = 1;
        rtc_start_read_datetime();
    } else if (strcmp(command, "OFF") == 0) {
        led_off();
        mode = 0;
    } else if (strcmp(command, "SET DATE") == 0) {
        SerialPort_Send_String("Usage: SET DATE dom,mo,yr (e.g., SET DATE 17,6,25)\n");
    } else if (strncmp(command, "SET DATE ", 9) == 0) {
        char* date_part = command + 9;
        char* token = strtok(date_part, ",");
        if (token) {
            uint8_t dom = atoi(token);
            token = strtok(NULL, ",");
            if (token) {
                uint8_t mo = atoi(token);
                token = strtok(NULL, ",");
                if (token) {
                    uint8_t yr = atoi(token);
                    
                    if (dom < 1 || dom > 31) {
                        SerialPort_Send_String("Error: Day of month must be 1-31\n");
                    } else if (mo < 1 || mo > 12) {
                        SerialPort_Send_String("Error: Month must be 1-12\n");
                    } else if (yr > 99) {
                        SerialPort_Send_String("Error: Year must be 0-99 (20xx)\n");
                    } else {
                        set_date(dom, mo, yr);
                    }
                } else {
                    SerialPort_Send_String("Invalid date format. Use: SET DATE dom,mo,yr\n");
                }
            } else {
                SerialPort_Send_String("Invalid date format. Use: SET DATE dom,mo,yr\n");
            }
        } else {
            SerialPort_Send_String("Invalid date format. Use: SET DATE dom,mo,yr\n");
        }
    } else if (strcmp(command, "SET TIME") == 0) {
        SerialPort_Send_String("Usage: SET TIME h,m,s (e.g., SET TIME 14,30,0)\n");
    } else if (strncmp(command, "SET TIME ", 9) == 0) {
        char* time_part = command + 9;
        char* token = strtok(time_part, ",");
        if (token) {
            uint8_t h = atoi(token);
            token = strtok(NULL, ",");
            if (token) {
                uint8_t m = atoi(token);
                token = strtok(NULL, ",");
                if (token) {
                    uint8_t s = atoi(token);
                    
                    if (h > 23) {
                        SerialPort_Send_String("Error: Hour must be 0-23\n");
                    } else if (m > 59) {
                        SerialPort_Send_String("Error: Minute must be 0-59\n");
                    } else if (s > 59) {
                        SerialPort_Send_String("Error: Second must be 0-59\n");
                    } else {
                        set_time(h, m, s);
                    }
                } else {
                    SerialPort_Send_String("Invalid time format. Use: SET TIME h,m,s\n");
                }
            } else {
                SerialPort_Send_String("Invalid time format. Use: SET TIME h,m,s\n");
            }
        } else {
            SerialPort_Send_String("Invalid time format. Use: SET TIME h,m,s\n");
        }
    } else if (strcmp(command, "SET ALARM") == 0) {
        SerialPort_Send_String("Usage: SET ALARM h,m,s (e.g., SET ALARM 14,30,0)\n");
    } else if (strncmp(command, "SET ALARM ", 10) == 0) {
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
                    
                    if (h > 23) {
                        SerialPort_Send_String("Error: Hour must be 0-23\n");
                    } else if (m > 59) {
                        SerialPort_Send_String("Error: Minute must be 0-59\n");
                    } else if (s > 59) {
                        SerialPort_Send_String("Error: Second must be 0-59\n");
                    } else {
                        set_alarm(h, m, s);
                    }
                } else {
                    SerialPort_Send_String("Invalid alarm format. Use: SET ALARM h,m,s\n");
                }
            } else {
                SerialPort_Send_String("Invalid alarm format. Use: SET ALARM h,m,s\n");
            }
        } else {
            SerialPort_Send_String("Invalid alarm format. Use: SET ALARM h,m,s\n");
        }
    } else if (strcmp(command, "HELP") == 0) {
        display_help();
    } else {
        SerialPort_Send_String("Unknown command: '");
        SerialPort_Send_String(command);
        SerialPort_Send_String("'\nType 'HELP' for available commands\n");
    }
} 