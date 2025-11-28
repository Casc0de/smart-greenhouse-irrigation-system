import serial
import json

# puerto = "/dev/ttyACM0"  # Cambia si es necesario
puerto = "COM3"  # Cambia si es necesario
baudrate = 115200

ser = serial.Serial(puerto, baudrate, timeout=1)
print("Leyendo datos del Arduino...")

while True:
    linea = ser.readline().decode().strip()
    if not linea:
        continue

    try:
        datos = json.loads(linea)
        print("Recibido:", datos)
        ser.write(b'ACK\n')  # Enviar recibido 
    except json.JSONDecodeError:
        print("Línea inválida (no es JSON):", linea)
