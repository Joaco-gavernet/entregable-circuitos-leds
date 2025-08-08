# Proyecto PWM RGB con Control UART

## Descripción
Sistema de control de LED RGB mediante PWM con comunicación UART para selección de colores y potenciómetro para ajuste de brillo.

## Características
- **3 canales PWM:** 2 por hardware (TIMER1) + 1 por software
- **Frecuencia:** 60Hz con resolución de 8 bits
- **Control UART:** Selección de colores mediante comandos
- **Control analógico:** Potenciómetro para ajuste de brillo
- **Arquitectura modular:** Código organizado en módulos separados

## Conexiones Hardware

### LED RGB
- **PB1 (Pin 15):** Canal Rojo - PWM Hardware (OC1A)
- **PB2 (Pin 16):** Canal Verde - PWM Hardware (OC1B)  
- **PB5 (Pin 19):** Canal Azul - PWM Software
- **GND:** Terminal común del LED

### Potenciómetro
- **Terminal 1:** VCC (5V)
- **Terminal 2 (Cursor):** ADC3 (Pin 26, PC3)
- **Terminal 3:** GND

### UART
- **TX (Pin 3):** Transmisión de datos
- **RX (Pin 2):** Recepción de comandos

## Comandos UART

| Comando | Color | Valores RGB |
|---------|-------|-------------|
| `1` | RED | 0, 255, 255 |
| `2` | GREEN | 255, 0, 255 |
| `3` | BLUE | 255, 255, 0 |
| `4` | CIAN | 255, 0, 0 |
| `5` | AMARILLO | 0, 0, 255 |
| `6` | MAGENTA | 0, 255, 0 |
| `7` | WHITE | 0, 0, 0 |
| `8` | BLACK | 255, 255, 255 |
| `m` | Mostrar menú | - |

## Estructura del Código

### Módulos
- **main.c/h:** Programa principal e inicialización
- **timer1_pwm.c/h:** PWM por hardware (PB1, PB2)
- **soft_pwm.c/h:** PWM por software (PB5)
- **adc.c/h:** Lectura del potenciómetro
- **color_control.c/h:** Control de colores RGB
- **serialPort.c/h:** Comunicación UART

### Funcionamiento
1. **Inicialización:** Configuración de PWM, ADC y UART
2. **Bucle principal:** 
   - Lectura del potenciómetro
   - Aplicación de brillo a colores seleccionados
   - Verificación de comandos UART
3. **Interrupciones:** PWM por software a 60Hz

## Compilación y Uso

### Compilar
```bash
make
```

### Subir al microcontrolador
```bash
make upload
```

### Monitorear UART
```bash
make monitor
```

## Configuración Técnica

### PWM Hardware (TIMER1)
- **Modo:** Fast PWM 8 bits
- **Prescaler:** 1024
- **Frecuencia:** ~60.79Hz
- **Pines:** PB1 (OC1A), PB2 (OC1B)

### PWM Software (TIMER0)
- **Modo:** CTC
- **Prescaler:** 8
- **OCR0A:** 129
- **Frecuencia:** 60Hz
- **Pin:** PB5

### ADC
- **Canal:** ADC3 (PC3)
- **Referencia:** AVcc
- **Prescaler:** 128
- **Resolución:** 10 bits

### UART
- **Velocidad:** 9600 bps
- **Configuración:** 8N1
- **Recepción:** Polling

## Notas Importantes
- El potenciómetro controla el brillo global (0-100%)
- Los comandos UART seleccionan el color base
- No se usan delays bloqueantes
- Todas las temporizaciones son por interrupciones 