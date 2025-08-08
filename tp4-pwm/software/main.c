#include "main.h"


int main() {
    uint16_t adc_value;
    uint16_t brightness;
    char uart_command;
    
    // Inicializar módulos
    timer1_pwm_init();
    soft_pwm_init();
    adc_init();
    SerialPort_Init(); // Configuración UART 9600bps
    color_control_init();
    sei(); // Habilitar interrupciones globales
    
    // Mostrar menú inicial
    color_control_print_menu();

    while (1) {
        // Leer potenciómetro para ajustar brillo
        // char buffer[10];
        adc_value = adc_read(3);
        // Calculo en float ya que debemos conocer fraccion exacta de potenciometro
        brightness = (uint16_t)(((float)adc_value/1023) * 255);
        
        // Aplicar brillo y colores actuales
        color_control_set_brightness(brightness);
        color_control_apply_colors();
        
        // Verificar si hay comando UART disponible
        if (UCSR0A & (1 << RXC0)) {
            uart_command = SerialPort_Recive_Data();
            
            if (uart_command == 'm' || uart_command == 'M') color_control_print_menu();
            else if (uart_command == '\r' || uart_command == '\n') continue;
            else color_control_set_color(uart_command);
        }
    }
    return 0;
}