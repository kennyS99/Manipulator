# import threading
# import tkinter as tk
# from motor import *  # 导入串口包

# class MotorControlGUI:
#     def __init__(self):
#         self.root = tk.Tk()
#         self.outputs = [0, 0, 0]
#         self.selected_motor = 0
#         self.selected_increment = 1
#         self.new_output_available = False
#         self.updated_motor_id = None
#         self.updated_motor_angle = None
#         self.gui_closed = False
        
#         # create buttons to select motor and control output
#         self.motor1_button = tk.Button(self.root, text="Motor 1", command=lambda: self.update_selected_motor(0))
#         self.motor2_button = tk.Button(self.root, text="Motor 2", command=lambda: self.update_selected_motor(1))
#         self.motor3_button = tk.Button(self.root, text="Motor 3", command=lambda: self.update_selected_motor(2))
#         self.motor1_button.grid(row=0, column=0)
#         self.motor2_button.grid(row=0, column=1)
#         self.motor3_button.grid(row=0, column=2)
#         self.coarse_button = tk.Button(self.root, text="Coarse +30", command=lambda: self.update_selected_increment(30))
#         self.fine_button = tk.Button(self.root, text="Fine +1", command=lambda: self.update_selected_increment(1))
#         self.coarse_button.grid(row=1, column=0, pady=10)
#         self.fine_button.grid(row=1, column=2, pady=10)
        
#         # create labels to display current output values
#         self.output_labels = []
#         for i in range(3):
#             label = tk.Label(self.root, text=f"Motor {i+1}: {self.outputs[i]}")
#             label.grid(row=i, column=3)
#             self.output_labels.append(label)
        
#         # bind "w" and "s" keys to increase and decrease output for the selected motor
#         self.root.bind("<KeyPress-w>", lambda event: self.set_motor_output(self.selected_motor, self.selected_increment))
#         self.root.bind("<KeyPress-s>", lambda event: self.set_motor_output(self.selected_motor, -self.selected_increment))
        
#         # create a thread to periodically check for new motor output values
#         self.update_thread = threading.Thread(target=self.update_output_values)
#         self.update_thread.daemon = True
#         self.update_thread.start()

#         self.root.protocol("WM_DELETE_WINDOW", quit)
#         # start the GUI mainloop
#         self.root.mainloop()
    
#     def set_motor_output(self, motor_number, increment):
#         self.outputs[motor_number] += increment
#         # limit output to range -359 to +359
#         self.outputs[motor_number] = max(min(self.outputs[motor_number], 359), -359)
#         self.output_labels[motor_number].config(text=f"Motor {motor_number+1}: {self.outputs[motor_number]}")
#         self.updated_motor_id = motor_number + 1
#         self.updated_motor_angle = self.outputs[motor_number]
#         self.new_output_available = True
#         motorID = motor_number + 1
#         angle = self.outputs[motor_number]
#         motor = Motor_control(motorID)
#         motor.angle_control(200, angle)
#         print(f"Select {motorID} to {angle}")
#         # print(f"Select {motor_number+1} to {self.outputs[motor_number]}")
    
#     def update_selected_motor(self, motor_number):
#         self.selected_motor = motor_number
#         print(f"Selected motor {self.selected_motor+1}")
    
#     def update_selected_increment(self, increment):
#         self.selected_increment = increment
#         print(f"Selected increment {self.selected_increment}")
        
#     def update_output_values(self):
#         while True:
#             if self.new_output_available:
#                 self.new_output_available = False
               



import tkinter as tk
from motor import Motor_control

class MotorControlGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.selected_motor = tk.StringVar()
        self.angle_value = tk.StringVar()
        
        # create a label and drop-down menu to select motor ID
        motor_label = tk.Label(self.root, text="Select Motor ID:")
        motor_label.grid(row=0, column=0)
        motor_dropdown = tk.OptionMenu(self.root, self.selected_motor, "1", "2", "3")
        motor_dropdown.grid(row=0, column=1)
        #motor1 
        #motor2 400 to -700
        #motor3 520 to -700
        # create a label and entry box to enter the angle value
        angle_label = tk.Label(self.root, text="Enter Angle Value:")
        angle_label.grid(row=1, column=0)
        self.angle_entry = tk.Entry(self.root, textvariable=self.angle_value)
        self.angle_entry.grid(row=1, column=1)
        
        # create a button to set the motor angle
        set_button = tk.Button(self.root, text="Set Angle", command=self.set_motor_angle)
        set_button.grid(row=2, column=0, columnspan=2, pady=10)
        
        # bind the Enter key to the set_motor_angle() method
        self.angle_entry.bind('<Return>', lambda event: self.set_motor_angle())
        
        self.root.mainloop()
    
    def set_motor_angle(self):
        motorID = int(self.selected_motor.get())
        angle = int(self.angle_value.get())
        
        # use the Motor_control class to set the angle for the selected motor
        motor = Motor_control(motorID)
        motor.angle_control(200, angle)
        print(f"Set motor {motorID} to {angle}")
        
        # clear the angle entry box after setting the motor angle
        self.angle_entry.delete(0, tk.END)

if __name__ == '__main__':
    gui = MotorControlGUI()

