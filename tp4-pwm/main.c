#include "main.h"

int main() {
    uint16_t adc_value;
    uint8_t brightness;
    char uart_command;
    
    // Inicializar módulos
    timer1_pwm_init();
    soft_pwm_init();
    adc_init();
    SerialPort_Init(0); // Configuración UART 9600bps
    color_control_init();
    sei(); // Habilitar interrupciones globales
    
    // Mostrar menú inicial
    color_control_print_menu();

    while (1) {
        // Leer potenciómetro para ajustar brillo
        adc_value = adc_read(3);
        brightness = (adc_value * 255) / 1023;
        
        // Aplicar brillo y colores actuales
        color_control_set_brightness(brightness);
        color_control_apply_colors();
        
        // Verificar si hay comando UART disponible
        if (UCSR0A & (1 << RXC0)) {
            uart_command = SerialPort_Recive_Data();
            
            if (uart_command == 'm' || uart_command == 'M') {
                color_control_print_menu();
            } else {
                color_control_set_color(uart_command);
            }
        }
    }
    return 0;
}