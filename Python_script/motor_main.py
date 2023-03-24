from motor import *  # 导入串口包

import time  # 导入时间包 



def main():
    # motor1 = Motor_control(1)
    # motor1 = Motor_control(1)
    # motor2 = Motor_control(2)
    # motor3 = Motor_control(3)
    # motor3 = Motor_control(3)
    # motor1.speed_control(360)
    try:
        while True:          
            motorID = int(input("Enter motorID: "))
            motor = Motor_control(motorID)
            time.sleep(0.5)
            angle = int(input("Enter angle: "))
            motor.angle_control(200,angle)
            # motor1.get_multileAngle()
            time.sleep(0.5)
            uart_get()
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("Exiting program.")

    

if __name__ == '__main__':
    main()

