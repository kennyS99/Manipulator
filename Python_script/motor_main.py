from motor import Motor_uart  # 导入串口包
from motor import Motor_control  # 导入串口包
from motor import ser

import time  # 导入时间包  

# ser = serial.Serial("/dev/ttyACM0",115200,timeout = 5)  # 开启com3口，波特率115200，超时5
# ser.flushInput()  # 清空缓冲区



            

def main():

    
    while True:
        Motor_control.speed_control(1,4000)
        # line = ser.readline()
        line = ser.read(ser.in_waiting)
        # print("now-->",line)
        print(Motor_uart.get_data(line))
        time.sleep(0.5)

if __name__ == '__main__':
    main()

