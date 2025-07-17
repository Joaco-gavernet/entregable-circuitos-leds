#include "color_control.h"
#include "timer1_pwm.h"
#include "soft_pwm.h"
#include "serialPort.h"

static rgb_color_t current_color = {0, 0, 0};
static uint8_t current_brightness = 255;

void color_control_init(void) {
    // Habilitar recepción UART
    SerialPort_RX_Enable();
    current_color.red = 0;
    current_color.green = 0;
    current_color.blue = 0;
    current_brightness = 255;
}

void color_control_set_brightness(uint8_t brightness) {
    current_brightness = brightness;
}

void color_control_set_color(uint8_t command) {
    switch(command) {
        case '1': // RED
            current_color.red = 255;
            current_color.green = 0;
            current_color.blue = 0;
            break;
        case '2': // GREEN
            current_color.red = 0;
            current_color.green = 255;
            current_color.blue = 0;
            break;
        case '3': // BLUE
            current_color.red = 0;
            current_color.green = 0;
            current_color.blue = 255;
            break;
        case '4': // CIAN
            current_color.red = 0;
            current_color.green = 255;
            current_color.blue = 255;
            break;
        case '5': // AMARILLO
            current_color.red = 255;
            current_color.green = 255;
            current_color.blue = 0;
            break;
        case '6': // MAGENTA
            current_color.red = 255;
            current_color.green = 0;
            current_color.blue = 255;
            break;
        case '7': // WHITE
            current_color.red = 255;
            current_color.green = 255;
            current_color.blue = 255;
            break;
        case '8': // BLACK
            current_color.red = 0;
            current_color.green = 0;
            current_color.blue = 0;
            break;
        default:
            SerialPort_Send_String("Comando invalido\r\n");
            return;
    }
    
    color_control_apply_colors();
    SerialPort_Send_String("Color aplicado\r\n");
}

void color_control_apply_colors(void) {
    uint8_t red_value = (((float)current_brightness/255) * current_color.red);
    uint8_t green_value = (((float)current_brightness/255) * current_color.green);
    uint8_t blue_value = (((float)current_brightness/255) * current_color.blue);

    
    timer1_pwm_set(1, green_value);   // PB1 - Verde
    timer1_pwm_set(2, red_value); // PB2 - Rojo
    soft_pwm_set(blue_value);       // PB5 - Azul
}

void color_control_print_menu(void) {
    SerialPort_Send_String("\r\n=== MENU DE COLORES ===\r\n");
    SerialPort_Send_String("1 - RED\r\n");
    SerialPort_Send_String("2 - GREEN\r\n");
    SerialPort_Send_String("3 - BLUE\r\n");
    SerialPort_Send_String("4 - CIAN\r\n");
    SerialPort_Send_String("5 - AMARILLO\r\n");
    SerialPort_Send_String("6 - MAGENTA\r\n");
    SerialPort_Send_String("7 - WHITE\r\n");
    SerialPort_Send_String("8 - BLACK\r\n");
    SerialPort_Send_String("m - Mostrar menu\r\n");
    SerialPort_Send_String("=====================\r\n");
} 