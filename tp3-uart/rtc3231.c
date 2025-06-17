#include "rtc3231.h"

// Estados para la máquina de estados del driver
typedef enum {
    IDLE,
    START,
    WRITE_ADDRESS,
    WRITE_REGISTER,
    REPEATED_START,
    READ_ADDRESS,
    // lectura de hora
    READ_SECONDS,
    READ_MINUTES,
    READ_HOURS,
    // escritura de hora
    WRITE_SECONDS,
    WRITE_MINUTES,
    WRITE_HOURS,
    // escritura de alarma
    WRITE_ALARM_REGISTER,
    WRITE_ALARM_SECONDS,
    WRITE_ALARM_MINUTES,
    WRITE_ALARM_HOURS,
    WRITE_ALARM_CTRL_REG,     
    WRITE_ALARM_CTRL_VALUE, 
    WRITE_CLEAR_ALARM,
    DONE
} rtc_state_t;

typedef enum {
    RTC_OP_NONE,
    RTC_OP_READ,
    RTC_OP_WRITE_TIME,
    RTC_OP_WRITE_ALARM,
    RTC_OP_CLEAR_ALARM ,
    RTC_OP_WRITE_ALARM_CTRL,
} rtc_op_t;


volatile rtc_op_t rtc_current_op = RTC_OP_NONE;
volatile rtc_state_t rtc_state = IDLE;
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
    if (rtc_state != IDLE) return;
    rtc_state = START;
    rtc_data_ready = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}


void rtc_get_time(uint8_t* h, uint8_t* m, uint8_t* s) {
    *h = rtc_hours;
    *m = rtc_minutes;
    *s = rtc_seconds;
}

void rtc_set_time(uint8_t h, uint8_t m, uint8_t s) {
    if (rtc_state != IDLE) return;
    rtc_current_op = RTC_OP_WRITE_TIME;
    rtc_write_h = h;
    rtc_write_m = m;
    rtc_write_s = s;
    rtc_state = START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

void rtc_set_alarm(uint8_t h, uint8_t m, uint8_t s) {
    if (rtc_state != IDLE) return;
    rtc_current_op = RTC_OP_WRITE_ALARM;
    rtc_write_h = h & 0x7F;
    rtc_write_m = m & 0x7F;
    rtc_write_s = s & 0x7F;
    rtc_state = START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

void rtc_clear_alarm_flag(void) {
    if (rtc_state != IDLE) return;
    rtc_current_op = RTC_OP_CLEAR_ALARM;
    rtc_state = START;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}


