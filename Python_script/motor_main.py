import time  # 导入时间包 
from gui import MotorControlGUI
from motor import *  # 导入串口包
def main():
    try:
        # create a MotorControlGUI instance to get user input and display motor ID and angle
        # gui = MotorControlGUI() 
        while True:
            
            # motorID = gui.selected_motor + 1
            # angle = gui.outputs[gui.selected_motor]
            motorID = int(input("Enter motorID: "))
            angle = int(input("Enter angle: "))
            
            motor = Motor_control(motorID)
            motor.angle_control(200, angle)
            print(f"try {motorID} to {angle}")
            motor.get_multileAngle()
            uart_get()
         
    except KeyboardInterrupt:
        print("Exiting program.")


if __name__ == '__main__':
    main()