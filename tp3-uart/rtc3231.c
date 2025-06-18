#include "rtc3231.h"

volatile rtc_op_t rtc_current_op = RTC_OP_NONE;
volatile rtc_state_t rtc_state = RTC_IDLE;
volatile rtc_datetime_t rtc_datetime;
volatile rtc_datetime_t rtc_write_datetime;
volatile uint8_t rtc_data_ready = 0;
volatile uint8_t alarm_active = 0;
volatile uint8_t alarm_count = 0;

void rtc_init(void) {
    // Velocidad de 100kHz @ 8MHz
    TWSR = 0x00;
    TWBR = 32;
    TWCR = (1 << TWEN);  // Enable TWI
}

void rtc_start_read_datetime(void) {
    if (rtc_state != RTC_IDLE) return;
    rtc_state = RTC_START;
    rtc_current_op = RTC_OP_READ_DATETIME; 
    rtc_data_ready = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

void rtc_get_datetime(rtc_datetime_t* datetime) {
    datetime->seconds = rtc_datetime.seconds;
    datetime->minutes = rtc_datetime.minutes;
    datetime->hours = rtc_datetime.hours;
    datetime->day_of_week = rtc_datetime.day_of_week;
    datetime->day_of_month = rtc_datetime.day_of_month;
    datetime->month = rtc_datetime.month;
    datetime->year = rtc_datetime.year;
}

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

void rtc_set_alarm(rtc_datetime_t* datetime) {
    if (rtc_state != RTC_IDLE) return;
    rtc_current_op = RTC_OP_WRITE_ALARM;
    rtc_state = RTC_START;
    rtc_write_datetime.seconds = datetime->seconds;
    rtc_write_datetime.minutes = datetime->minutes;
    rtc_write_datetime.hours = datetime->hours;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

void rtc_clear_alarm_flag(void) {
    if (rtc_state != RTC_IDLE) return;
    rtc_current_op = RTC_OP_CLEAR_ALARM;
    rtc_state = RTC_START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}


