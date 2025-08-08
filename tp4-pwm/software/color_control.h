#ifndef COLOR_CONTROL_H
#define COLOR_CONTROL_H
#include "main.h"

// Estructura para almacenar valores RGB
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb_color_t;

void color_control_init(void);
void color_control_set_color(uint8_t command);
void color_control_set_brightness(uint8_t brightness);
void color_control_apply_colors(void);
void color_control_print_menu(void);

#endif // COLOR_CONTROL_H 