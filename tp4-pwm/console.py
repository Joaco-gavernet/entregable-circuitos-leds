import serial
import threading
import time

# Replace this with the correct COM port or /dev/ttyUSBx
SERIAL_PORT = "/dev/tty.usbserial-2130"          # For Windows (e.g., COM3), or "/dev/ttyUSB0" for Linux
BAUD_RATE = 9600               # Must match UBRR config (e.g., 9600 bps)

# Global state variables
current_mode = 0  # 0 = Command Mode, 1 = Data Mode
mode_lock = threading.Lock()

def read_serial(ser):
    """Thread function to continuously read from serial port"""
    global current_mode
    
    while True:
        if ser.in_waiting:
            try:
                data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
                if data:
                    # Check for mode change signals
                    if "CMD_MODE" in data:
                        with mode_lock:
                            current_mode = 0
                    elif "DATA_MODE" in data:
                        with mode_lock:
                            current_mode = 1
                    
                    # Print the data (but filter out mode signals)
                    if not data.startswith("CMD_MODE") and not data.startswith("DATA_MODE"):
                        print(data, end='', flush=True)
            except:
                break

def print_usage_examples():
    """Print usage examples for common commands"""
    print("\n=== PWM RGB Control Commands ===")
    print("1                   # RED color")
    print("2                   # GREEN color")
    print("3                   # BLUE color")
    print("4                   # CIAN color")
    print("5                   # AMARILLO color")
    print("6                   # MAGENTA color")
    print("7                   # WHITE color")
    print("8                   # BLACK (off)")
    print("m                   # Show menu")
    print("help                # Show this help")
    print("===============================\n")

def main():
    global current_mode
    
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
        print("Type color commands and press Enter. Press Ctrl+C to exit.")
        print("Use the potentiometer to adjust brightness (0-100%)")
        print_usage_examples()

        # Start reading thread
        read_thread = threading.Thread(target=read_serial, args=(ser,), daemon=True)
        read_thread.start()

        # Main input loop
        while True:
            try:
                time.sleep(0.5)  # TO-DO: check if possible to avoid blocking delays
                print("> ", end='', flush=True) 
                command = input()  # Get user input
                if command.strip():  # Only send non-empty commands
                    if command.lower() == 'help':
                        print_usage_examples()
                    else:
                        ser.write((command + '\n').encode('utf-8'))  # Send command with newline
                        ser.flush()  # Ensure data is sent immediately
            except KeyboardInterrupt:
                break
            except EOFError:
                break

    except serial.SerialException as e:
        print(f"[ERROR] Could not open serial port: {e}")
        print(f"Please check if {SERIAL_PORT} is correct and the device is connected.")
    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        if 'ser' in locals():
            ser.close()

if __name__ == "__main__":
    main()