# === Configuration ===
MCU=atmega328p
F_CPU=16000000UL
BAUD=115200
PROGRAMMER=arduino
PORT=/dev/tty.usbserial-140

# === Tools ===
CC=avr-gcc
OBJCOPY=avr-objcopy

# === Files ===
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
DEP=$(SRC:.c=.d)
TARGET=main

# Add AVR-specific flags and includes
CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -std=gnu11 -I/usr/local/avr/include -I/usr/avr/include
LDFLAGS=-mmcu=$(MCU)

# === Rules ===
all: $(TARGET).hex

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

-include $(DEP)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

upload: $(TARGET).hex
	avrdude -v -p$(MCU) -c$(PROGRAMMER) -P$(PORT) -b$(BAUD) -D -Uflash:w:$<:i

clean:
	rm -f *.o *.elf *.hex *.d

monitor:
	./serial_console.py

