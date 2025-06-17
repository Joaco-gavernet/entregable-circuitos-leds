#ifndef RTC3231_H
#define RTC3231_H

#ifndef MAIN_H
	#include "main.h"
#endif

#define DS3231_ADDRESS  0x68  // 7-bit address
#define READ_ADDRESS    ((DS3231_ADDRESS << 1) | 1)
#define WRITE_ADDRESS   ((DS3231_ADDRESS << 1) | 0)

void rtc_init(void);
void rtc_start_read(void);
void rtc_get_time(uint8_t* h, uint8_t* m, uint8_t* s);
void rtc_set_time(uint8_t h, uint8_t m, uint8_t s);
void rtc_set_alarm(uint8_t h, uint8_t m, uint8_t s);
void rtc_clear_alarm_flag(void);
void rtc_write_alarm_ctrl(void);
void rtc_read_alarm_ctrl(void);


#endif


