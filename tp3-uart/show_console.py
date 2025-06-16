import serial

# Replace this with the correct COM port or /dev/ttyUSBx
SERIAL_PORT = "COM5"           # For Windows (e.g., COM3), or "/dev/ttyUSB0" for Linux
BAUD_RATE = 9600               # Must match UBRR config (e.g., 9600 bps)

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
        print("Press Ctrl+C to exit.\n")

        while True:
            if ser.in_waiting:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    print(f"> {line}")  # print UART message

    except serial.SerialException as e:
        print(f"[ERROR] Could not open serial port: {e}")
    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        if 'ser' in locals():
            ser.close()

if __name__ == "__main__":
    main()