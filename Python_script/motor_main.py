import time  # 导入时间包 
# from motor import *  # 导入串口包
from gui import MotorControlGUI
def main():
    try:
        # create a MotorControlGUI instance to get user input and display motor ID and angle
        gui = MotorControlGUI()
        while True:          
            motorID = gui.selected_motor + 1
            angle = gui.outputs[gui.selected_motor]
            print(f"Set motor {motorID} to {angle}")
            # motor = Motor_control(motorID)
            # motor.angle_control(200, angle)
            # motor.get_multileAngle()
            # uart_get()
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("Exiting program.")


if __name__ == '__main__':
    main()