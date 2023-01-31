import serial  # 导入串口包
import re
try:
    ser = serial.Serial("/dev/ttyACM1",115200,timeout = 5)  # 开启com3口，波特率115200，超时5
    ser.flushInput()  # 清空缓冲区
    print("serial open success!")
except:
    print("serial open fail!")
########################打包函数######################################   
class Motor_control:
    def speed_control(motor_id, speed):
        data = [0,0,0,0]
        data[0] = motor_id
        data[1] = speed
        for i in range(2):
            Motor_uart.senddata(1,data)
        print("speed_control-->motor_id=%d,speed=%d"%(motor_id,speed))
        

########################打包函数######################################  
class Motor_uart:
    def senddata(id,data):
        data_end=[0,0,0,0]
        if id <10 :
            id_end = "0"+str(id)
        else:
            id_end = str(id)

        for i in range(4):

            if data[i]<10:
                data_end[i] = "0000"+str(data[i])
            elif data[i]<100:
                data_end[i] = "000"+str(data[i])
            elif data[i]<1000:
                data_end[i] = "00"+str(data[i])
            elif data[i]<10000:
                data_end[i] = "0"+str(data[i])
            else:
                data_end[i] = str(data[i])  
                
            # print("data_end:", i,data_end[i])    
        ser.write((id_end+str(data_end[0])+str(data_end[1])+str(data_end[2])+str(data_end[3])+"*/").encode('ascii')) 

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