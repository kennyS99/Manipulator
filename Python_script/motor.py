import serial as ser # 导入串口包

try:
    se = ser.Serial("/dev/ttyACM1",115200,timeout = 5)  # 开启com3口，波特率115200，超时5
    se.flushInput()  # 清空缓冲区
    print("serial open success!")
except:
    print("serial open fail!")
########################打包函数######################################   
class Motor_control(object):
    def __init__(self, motorID):
        self.motorID = motorID


    def speed_control(self, motorSpeed):
        data = [0,0,0,0]
        data[0] = self.motorID
        data[1] = motorSpeed * 100
        for i in range(2):
            senddata(1,data)
        print("speed_control-->motor_id=%d,speed=%d"%(self.motorID,motorSpeed))
    
    # def angle_control(self, motorAngle):

        

########################打包函数######################################  
def to_2d_array(num):
    num_str = str(num).zfill(5)
    return [[int(num_str[i + j]) for i in range(0, 5, 5)] for j in range(5)]

def senddata(id,data):
    data_end=[0,0,0,0]
    if id <10 :
        id_end = "0"+str(id)
    else:
        id_end = str(id)

    memory = to_2d_array(data[0])+to_2d_array(data[1])+to_2d_array(data[2])+to_2d_array(data[3])
    to_1d_array = sum(memory, [])
    to_str = ''.join([str(x) for x in to_1d_array])
        # print("data_end:", i,data_end[i])    
    # se.write((id_end+str(data_end[0])+str(data_end[1])+str(data_end[2])+str(data_end[3])+"*/").encode('ascii')) 

def get_data(data):
    ptr = ['null','temp','iq','speed','encoder','']
    data = str(data, encoding="'ISO-8859-1'")
    change_data = data[data.find('outputstate.temperature'):data.find('outputstate_end')]
    print ("change_data--->",change_data)

    if 'outputstate' in change_data:
            line1 = str(change_data)
            ouput_data = line1.split('outputstate.')
            for i in range(5):
                print("outputstate%d-->"%i, ouput_data[i])
                ouput_data[i] = re.sub("\D", "", ouput_data[i])
                print("outputstate.%s-->"%ptr[i],ouput_data[i])
            return ouput_data