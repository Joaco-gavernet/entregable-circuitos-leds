import serial
import threading
import time

# Replace this with the correct COM port or /dev/ttyUSBx
SERIAL_PORT = "COM5"           # For Windows (e.g., COM3), or "/dev/ttyUSB0" for Linux
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

def main():
    global current_mode
    
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
        print("Type commands and press Enter. Press Ctrl+C to exit.\n")

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
                    ser.write((command + '\n').encode('utf-8'))  # Send command with newline
                    ser.flush()  # Ensure data is sent immediately
            except KeyboardInterrupt:
                break
            except EOFError:
                break

    except serial.SerialException as e:
        print(f"[ERROR] Could not open serial port: {e}")
    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        if 'ser' in locals():
            ser.close()

if __name__ == "__main__":
    main()