#include "rtc3231.h"

volatile rtc_op_t rtc_current_op = RTC_OP_NONE;
volatile rtc_state_t rtc_state = RTC_IDLE;
volatile rtc_datetime_t rtc_datetime;
volatile rtc_datetime_t rtc_write_datetime;
volatile uint8_t rtc_data_ready = 0;
volatile uint8_t alarm_active = 0;
volatile uint8_t alarm_count = 0;

/***************************************************************
* Propósito de la función: Inicializa el módulo TWI (Two Wire Interface) para la comunicación con el RTC DS3231.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void rtc_init(void) {
    // Velocidad de 100kHz @ 8MHz
    TWSR = 0x00;
    TWBR = 32;
    TWCR = (1 << TWEN);  // Enable TWI

    // Assuming DS3231 INT pin is connected to Arduino digital pin 2 (INT0)
    EICRA |= (1 << ISC01); // Trigger INT0 on falling edge
    EIMSK |= (1 << INT0);  // Enable external interrupt INT0
}

/***************************************************************
* Propósito de la función: Inicia la FSM para la lectura de fecha y hora desde el RTC DS3231.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void rtc_start_read_datetime(void) {
    if (rtc_state != RTC_IDLE) return;
    rtc_state = RTC_START;
    rtc_current_op = RTC_OP_READ_DATETIME; 
    rtc_data_ready = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

/***************************************************************
* Propósito de la función: Obtiene la fecha y hora actual del RTC
* Parámetros de entrada (tipo, rango y formato): 
*   - datetime: Puntero a estructura rtc_datetime_t donde se almacenará la fecha y hora
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void rtc_get_datetime(rtc_datetime_t* datetime) {
    datetime->seconds = rtc_datetime.seconds;
    datetime->minutes = rtc_datetime.minutes;
    datetime->hours = rtc_datetime.hours;
    datetime->day_of_week = rtc_datetime.day_of_week;
    datetime->day_of_month = rtc_datetime.day_of_month;
    datetime->month = rtc_datetime.month;
    datetime->year = rtc_datetime.year;
}

/***************************************************************
* Propósito de la función: Configura la fecha y hora en el RTC DS3231.
* Parámetros de entrada (tipo, rango y formato):
*   - datetime: Puntero a struct rtc_datetime_t con la nueva fecha y hora a configurar
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void rtc_set_datetime(rtc_datetime_t* datetime) {
    if (rtc_state != RTC_IDLE) return;
    rtc_current_op = RTC_OP_WRITE_DATETIME;
    rtc_write_datetime.seconds = datetime->seconds;
    rtc_write_datetime.minutes = datetime->minutes;
    rtc_write_datetime.hours = datetime->hours;
    rtc_write_datetime.day_of_week = datetime->day_of_week;
    rtc_write_datetime.day_of_month = datetime->day_of_month;
    rtc_write_datetime.month = datetime->month;
    rtc_write_datetime.year = datetime->year;
    rtc_state = RTC_START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

/***************************************************************
* Propósito de la función: Configura la alarma en el RTC DS3231.
* Parámetros de entrada (tipo, rango y formato):
*   - datetime: Puntero a struct rtc_datetime_t con la hora de la alarma a configurar
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void rtc_set_alarm(rtc_datetime_t* datetime) {
    if (rtc_state != RTC_IDLE) return;
    rtc_current_op = RTC_OP_WRITE_ALARM;
    rtc_state = RTC_START;
    rtc_write_datetime.seconds = datetime->seconds;
    rtc_write_datetime.minutes = datetime->minutes;
    rtc_write_datetime.hours = datetime->hours;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

/***************************************************************
* Propósito de la función: Limpia el flag de alarma en el RTC DS3231.
* Parámetros de entrada (tipo, rango y formato): Ninguno
* Parámetros de salida (tipo, rango y formato): Ninguno
* Autor: Valeria Garcia, Joaquin Gavernet, Bautista Garcia
***************************************************************/
void rtc_clear_alarm_flag(void) {
    if (rtc_state != RTC_IDLE) return;
    rtc_current_op = RTC_OP_CLEAR_ALARM;
    rtc_state = RTC_START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}


