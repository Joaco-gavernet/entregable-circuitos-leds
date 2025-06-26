#include "main.h"

volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t message_ready = 0;
volatile uint8_t mode = 0;  // 0 = OFF, 1 = ON
volatile uint8_t mode_change_requested = 0;
volatile uint8_t timer_flag_1s = 0;



uint8_t bcd_to_decimal(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8_t decimal_to_bcd(uint8_t decimal) {
    return ((decimal / 10) << 4) | (decimal % 10);
}

// === Timer 1s para tareas cíclicas ===
void timer1_init(void) {
    TCCR1B |= (1 << WGM12);                  // CTC
    OCR1A = 15624;                           // 1s @ 8MHz y prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);     // Prescaler 1024
    TIMSK1 |= (1 << OCIE1A);                 // Enable compare interrupt
}

// === INT0 para interrupción del RTC ===
void ext_int0_init(void) {
    EICRA |= (1 << ISC01);   // Flanco descendente
    EICRA &= ~(1 << ISC00);
    EIMSK |= (1 << INT0);    // Habilitar INT0
}

// === UART para comunicación con PC ===
void uart_setup(void) {
    SerialPort_Init(0x67); // 9600 baudios
    SerialPort_RX_Enable();
    SerialPort_RX_Interrupt_Enable();
    SerialPort_TX_Enable();
    sei();
}

int main(void) {
    uart_setup();
    led_setup();
    ext_int0_init(); // Habilitar interrupción INT0 (ALARMA)
    timer1_init(); // Inicializar interrupción de 1s para tareas periódicas
    rtc_init(); // Inicializar RTC

    // Foreground/Background
    while(1) {
        if (message_ready) {
            message_ready = 0; 
            process_command((char*)rx_buffer);
        }
        if (timer_flag_1s) { // Interrupciones periodicas(1s)
            timer_flag_1s = 0;
            if (rtc_data_ready && mode == 1) {
                display_datetime();
                rtc_start_read_datetime(); // Continue reading for next second
            }
        }
        if (alarm_active) handle_alarm();
    }

    return 0;
}
