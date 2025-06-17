#ifndef RTC3231_H
#define RTC3231_H

#ifndef MAIN_H
	#include "main.h"
#endif

#define DS3231_ADDRESS 0x68  // 7-bit address
#define READ_ADDRESS ((DS3231_ADDRESS << 1) | 1)
#define WRITE_ADDRESS ((DS3231_ADDRESS << 1) | 0)


// Estados para la máquina de estados del driver
typedef enum {
    RTC_IDLE,
    RTC_START,
    RTC_WRITE_ADDRESS,
    RTC_WRITE_REGISTER,
    RTC_REPEATED_START,
    RTC_READ_ADDRESS,
    // lectura de hora
    RTC_READ_SECONDS,
    RTC_READ_MINUTES,
    RTC_READ_HOURS,
    // escritura de hora
    RTC_WRITE_SECONDS,
    RTC_WRITE_MINUTES,
    RTC_WRITE_HOURS,
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
    RTC_OP_READ,
    RTC_OP_WRITE_TIME,
    RTC_OP_WRITE_ALARM,
    RTC_OP_CLEAR_ALARM ,
    RTC_OP_WRITE_ALARM_CTRL,
} rtc_op_t;


extern volatile rtc_op_t rtc_current_op;
extern volatile rtc_state_t rtc_state;
extern volatile uint8_t rtc_seconds, rtc_minutes, rtc_hours;
extern volatile uint8_t rtc_write_h, rtc_write_m, rtc_write_s;
extern volatile uint8_t rtc_data_ready;
extern volatile uint8_t alarm_active;
extern volatile uint8_t alarm_count;

void rtc_init(void);
void rtc_start_read(void);
void rtc_get_time(uint8_t* h, uint8_t* m, uint8_t* s);
void rtc_set_time(uint8_t h, uint8_t m, uint8_t s);
void rtc_set_alarm(uint8_t h, uint8_t m, uint8_t s);
void rtc_clear_alarm_flag(void);
void rtc_write_alarm_ctrl(void);
void rtc_read_alarm_ctrl(void);


#endif


