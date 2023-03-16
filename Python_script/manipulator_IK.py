import numpy as np
from roboticstoolbox import DHRobot, RevoluteDH
from spatialmath import SE3
from IPython.display import display

robot = DHRobot(
    [   
        RevoluteDH(alpha=-np.pi/2),
        RevoluteDH(a=10),
        RevoluteDH(a=10)
    ], name="Capstone")

target_pose = SE3(5, 5, 5)

ik_solution = robot.ikine_LMS(target_pose)

theta1, theta2, theta3 = ik_solution.q
# Convert the joint angles to degrees
theta1_deg = np.rad2deg(theta1)
theta2_deg = np.rad2deg(theta2)
theta3_deg = np.rad2deg(theta3)

# Print the joint angles in degrees
print("Theta1 (in degrees):", theta1_deg)
print("Theta2 (in degrees):", theta2_deg)
print("Theta3 (in degrees):", theta3_deg)



