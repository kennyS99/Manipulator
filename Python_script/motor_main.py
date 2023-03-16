from motor import *  # 导入串口包
import serial

import time  # 导入时间包 



def main():
    # motor1 = Motor_control(1)
    motor2 = Motor_control(2)
    # motor3 = Motor_control(3)
    # motor1.speed_control(360)
    motor2.angle_control(360,-360)
    # motor3.speed_control(360)
    # time.sleep(2)
    # # motor1.motor_stop()
    # motor2.angle_control(360,-360)
    # motor3.motor_stop()
    while True:
        # line = ser.readline()
        # line = ser.read(ser.in_waiting)
        # print("now-->",line)
        # print(uart_get(line))
        uart_get()
        time.sleep(0.5)

    

if __name__ == '__main__':
    main()

