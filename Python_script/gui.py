import threading
import tkinter as tk

class MotorControlGUI:
    def __init__(self):
        self.root = tk.Tk()
        self.outputs = [0, 0, 0]
        self.selected_motor = 0
        self.selected_increment = 1
        self.new_output_available = False
        self.updated_motor_id = None
        self.updated_motor_angle = None
        self.gui_closed = False
        
        # create buttons to select motor and control output
        self.motor1_button = tk.Button(self.root, text="Motor 1", command=lambda: self.update_selected_motor(0))
        self.motor2_button = tk.Button(self.root, text="Motor 2", command=lambda: self.update_selected_motor(1))
        self.motor3_button = tk.Button(self.root, text="Motor 3", command=lambda: self.update_selected_motor(2))
        self.motor1_button.grid(row=0, column=0)
        self.motor2_button.grid(row=0, column=1)
        self.motor3_button.grid(row=0, column=2)
        self.coarse_button = tk.Button(self.root, text="Coarse +5", command=lambda: self.update_selected_increment(5))
        self.fine_button = tk.Button(self.root, text="Fine +1", command=lambda: self.update_selected_increment(1))
        self.coarse_button.grid(row=1, column=0, pady=10)
        self.fine_button.grid(row=1, column=2, pady=10)
        
        # create labels to display current output values
        self.output_labels = []
        for i in range(3):
            label = tk.Label(self.root, text=f"Motor {i+1}: {self.outputs[i]}")
            label.grid(row=i, column=3)
            self.output_labels.append(label)
        
        # bind "w" and "s" keys to increase and decrease output for the selected motor
        self.root.bind("<KeyPress-w>", lambda event: self.set_motor_output(self.selected_motor, self.selected_increment))
        self.root.bind("<KeyPress-s>", lambda event: self.set_motor_output(self.selected_motor, -self.selected_increment))
        
        # create a thread to periodically check for new motor output values
        self.update_thread = threading.Thread(target=self.update_output_values)
        self.update_thread.daemon = True
        self.update_thread.start()

        self.root.protocol("WM_DELETE_WINDOW", quit)
        # start the GUI mainloop
        self.root.mainloop()
    
    def set_motor_output(self, motor_number, increment):
        self.outputs[motor_number] += increment
        # limit output to range -359 to +359
        self.outputs[motor_number] = max(min(self.outputs[motor_number], 359), -359)
        self.output_labels[motor_number].config(text=f"Motor {motor_number+1}: {self.outputs[motor_number]}")
        self.updated_motor_id = motor_number + 1
        self.updated_motor_angle = self.outputs[motor_number]
        self.new_output_available = True
        print(f"Set motor {motor_number+1} to {self.outputs[motor_number]}")
    
    def update_selected_motor(self, motor_number):
        self.selected_motor = motor_number
        print(f"Selected motor {self.selected_motor+1}")
    
    def update_selected_increment(self, increment):
        self.selected_increment = increment
        print(f"Selected increment {self.selected_increment}")
        
    def update_output_values(self):
        while True:
            if self.new_output_available:
                self.new_output_available = False
               



# gui = MotorControlGUI()