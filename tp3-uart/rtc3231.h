#ifndef RTC3231_H
#define RTC3231_H

#ifndef MAIN_H
	#include "main.h"
#endif

#define DS3231_ADDRESS 0x68  // 7-bit address
#define READ_ADDRESS ((DS3231_ADDRESS << 1) | 1)
#define WRITE_ADDRESS ((DS3231_ADDRESS << 1) | 0)

// Unified datetime structure
typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day_of_week;
    uint8_t day_of_month;
    uint8_t month;
    uint8_t year;
} rtc_datetime_t;

// Estados para la máquina de estados del driver
typedef enum {
    RTC_IDLE,
    RTC_START,
    RTC_WRITE_ADDRESS,
    RTC_WRITE_REGISTER,
    RTC_REPEATED_START,
    RTC_READ_ADDRESS,
    // lectura completa de fecha y hora
    RTC_READ_SECONDS,
    RTC_READ_MINUTES,
    RTC_READ_HOURS,
    RTC_READ_DAY_OF_WEEK,
    RTC_READ_DAY_OF_MONTH,
    RTC_READ_MONTH,
    RTC_READ_YEAR,
    // escritura completa de fecha y hora
    RTC_WRITE_SECONDS,
    RTC_WRITE_MINUTES,
    RTC_WRITE_HOURS,
    RTC_WRITE_DAY_OF_WEEK,
    RTC_WRITE_DAY_OF_MONTH,
    RTC_WRITE_MONTH,
    RTC_WRITE_YEAR,
    // escritura de alarma
    RTC_WRITE_ALARM_REGISTER,
    RTC_WRITE_ALARM_SECONDS,
    RTC_WRITE_ALARM_MINUTES,
    RTC_WRITE_ALARM_HOURS,
    RTC_WRITE_ALARM_CTRL_REG,     
    RTC_WRITE_ALARM_CTRL_VALUE, 
    RTC_WRITE_CLEAR_ALARM,
    RTC_DONE
} rtc_state_t;

typedef enum {
    RTC_OP_NONE,
    RTC_OP_READ_DATETIME,
    RTC_OP_WRITE_DATETIME,
    RTC_OP_WRITE_ALARM,
    RTC_OP_CLEAR_ALARM,
    RTC_OP_WRITE_ALARM_CTRL,
} rtc_op_t;

extern volatile rtc_op_t rtc_current_op;
extern volatile rtc_state_t rtc_state;
extern volatile rtc_datetime_t rtc_datetime;
extern volatile rtc_datetime_t rtc_write_datetime;
extern volatile uint8_t rtc_data_ready;
extern volatile uint8_t alarm_active;
extern volatile uint8_t alarm_count;

void rtc_init(void);
void rtc_start_read_datetime(void);
void rtc_get_datetime(rtc_datetime_t* datetime);
void rtc_set_datetime(rtc_datetime_t* datetime);
void rtc_set_alarm(rtc_datetime_t* datetime);
void rtc_clear_alarm_flag(void);
void rtc_write_alarm_ctrl(void);
void rtc_read_alarm_ctrl(void);

#endif


