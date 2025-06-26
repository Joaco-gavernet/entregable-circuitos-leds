#ifndef COMMAND_H
#define COMMAND_H

#include "main.h"

// Command processing functions
void process_command(char* command);
void set_date(uint8_t dom, uint8_t mo, uint8_t yr);
void set_time(uint8_t h, uint8_t m, uint8_t s);
void set_alarm(uint8_t h, uint8_t m, uint8_t s);
void display_datetime(void);
void display_help(void);
void handle_alarm(void);


#endif // COMMAND_H 