import serial as ser 
import time
a = "222asfg"
b = 678
c = "I am Dunkle.T.."



se = ser.Serial(port="/dev/ttyTHS1",baudrate=9600,timeout = 5)  # 开启com3口，波特率115200，超时5

while True:
    se.write(a.encode('utf-8'))
    # se.write(str(b).encode('utf-8'))
    # se.write(c.encode('utf-8'))
    # se.write("\r\n".encode())
    print(se.readline().decode('latin-1'))
    time.sleep(1)
    print("send ok")
    print(se.is_open)
