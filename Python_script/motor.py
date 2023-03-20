import serial as ser 
#/dev/ttyTHS2
try:
    se = ser.Serial(port="/dev/ttyUSB0",baudrate=115200,timeout = 5)  # 开启com3口，波特率115200，超时5
    se.flushInput()  # clean the contents of the serial buffer
    print("connected to: " + se.portstr)
except Exception as e:
    print(str(e))
    exit(1)
    
##############################################################   
class Motor_control(object):
    def __init__(self, motorID):
        self.motorID = motorID


    def speed_control(self, motorSpeed):
        data = [0,0,0,0]
        data[0] = self.motorID
        data[1] = motorSpeed * 100
        for x in range(0, 2):
            uart_send(1,data)
        # print("speed_control-->motor_id=%d,speed=%d"%(self.motorID,motorSpeed))
    def motor_stop(self):
        data = [0,0,0,0]
        data[0] = self.motorID
        for x in range(0, 2):
            uart_send(4,data)
    def angle_control(self, motorSpeed, motorAngle):
        data = [0,0,0,0]
        data[0] = self.motorID
        data[1] = motorSpeed
        #136000 is negative
        #236000 is positive
        if motorAngle > 0:
            new_motorAngle = "2" + str(motorAngle) + "00"
        elif motorAngle < 0:
            
            new_motorAngle = "1" + str(abs(motorAngle)) + "00"
        elif motorAngle == 0:
            new_motorAngle = "00000" + str(motorAngle)
        # print("new_motorAngle =" + new_motorAngle)
        data[2] = int(new_motorAngle)
        print("new_motorAngle =" + str(data[2]))
        for x in range(0, 2):
            uart_send(5,data)

##############################################################  
def to_2d_array(num):
    num_str = str(num).zfill(5)
    return [[int(num_str[i + j]) for i in range(0, 5, 5)] for j in range(5)]

def uart_send(id,data):
    if id <10 :
        id_end = "0"+str(id)
    else:
        id_end = str(id)

    memory = to_2d_array(data[0])+to_2d_array(data[1])+to_2d_array(data[2])+to_2d_array(data[3])
    to_1d_array = sum(memory, [])
    to_str = ''.join([str(x) for x in to_1d_array])
    total_data = str(id_end + to_str) + "*/"
     
    se.write(total_data.encode('utf-8')) 

def uart_get():
    bytesToRead = se.inWaiting()
    data = se.read(bytesToRead).decode('UTF-8')
    data = data.replace(" outputstate.","")
    data = data.replace("outputstate.","")
    data = data.replace("uart_get_","")
    lst = data.strip().split(',')
    for i in lst:
        if i.find("temperature") != -1:
            print(i,end=', ')
        if i.find("iq") != -1:
            print(i,end=', ')
        if i.find("speed") != -1:
            print(i,end=', ')
        if i.find("encoder") != -1:
            print(i,end=', ')
        if i.find("MotorAngle") != -1:
            print(i,end=', ')
            
