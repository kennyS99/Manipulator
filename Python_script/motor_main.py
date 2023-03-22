from motor import *  # 导入串口包

import time  # 导入时间包 



def main():
    # motor1 = Motor_control(1)
    motor1 = Motor_control(1)
    # motor3 = Motor_control(3)
    # motor1.speed_control(360)
    try:
        while True:          
            angle = int(input("Enter angle: "))
            motor1.angle_control(360,angle)
            time.sleep(0.5)
            # motor1.get_multileAngle()
            uart_get()
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("Exiting program.")

    

if __name__ == '__main__':
    main()

