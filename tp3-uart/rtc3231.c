#include "rtc3231.h"

volatile rtc_op_t rtc_current_op = RTC_OP_NONE;
volatile rtc_state_t rtc_state = RTC_IDLE;
volatile uint8_t rtc_seconds, rtc_minutes, rtc_hours;
volatile uint8_t rtc_write_h, rtc_write_m, rtc_write_s;
volatile uint8_t rtc_data_ready = 0;
volatile uint8_t alarm_active = 0;
volatile uint8_t alarm_count = 0;

void rtc_init(void) {
    // Velocidad de 100kHz @ 8MHz
    TWSR = 0x00;
    TWBR = 32;
    TWCR = (1 << TWEN);  // Enable TWI
}

void rtc_start_read(void) {
    if (rtc_state != RTC_IDLE) return;
    rtc_state = RTC_START;
    rtc_current_op = RTC_OP_READ; 
    rtc_data_ready = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}


void rtc_get_time(uint8_t* h, uint8_t* m, uint8_t* s) {
    *h = rtc_hours;
    *m = rtc_minutes;
    *s = rtc_seconds;
}

void rtc_set_time(uint8_t h, uint8_t m, uint8_t s) {
    if (rtc_state != RTC_IDLE) return;
    rtc_current_op = RTC_OP_WRITE_TIME;
    rtc_write_h = h;
    rtc_write_m = m;
    rtc_write_s = s;
    rtc_state = RTC_START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

void rtc_set_alarm(uint8_t h, uint8_t m, uint8_t s) {
    if (rtc_state != RTC_IDLE) return;
    rtc_current_op = RTC_OP_WRITE_ALARM;
    rtc_write_h = h & 0x7F;
    rtc_write_m = m & 0x7F;
    rtc_write_s = s & 0x7F;
    rtc_state = RTC_START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

void rtc_clear_alarm_flag(void) {
    if (rtc_state != RTC_IDLE) return;
    rtc_current_op = RTC_OP_CLEAR_ALARM;
    rtc_state = RTC_START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}


