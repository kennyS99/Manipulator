#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "can.h"
#include "timer.h"
#include "motor_control.h"
/************************************************
User manual：
CAN1 connet to the motor, open serial port, take a look if there is any message print out,Serial port baud rate 115200

motor control command: below is a example for motor speed contro,data=2000，
01000010200000000000*/

/*************************************************/
#define TIME3_COUNT	    15 //定时器计数15ms，用于获取编码器初始encoder

int main(void)
{
   	u8 key;
	u8 i=0,t=1,j=0;
	u16 times=0;
	u16 len=0;
	u8 cnt=0;
	u8 canbuf[200]={0};
	u16 canbufchange_ascil[100]={0};
	u8 rdata;
	u8 res;
	u8 Get_STDid;
	u8 *str;
	motor_state getstate;
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART
	
	LED_Init();						//初始化LED	
	KEY_Init();						//初始化KEY
  TIM3_Init(TIME3_COUNT-1,8400-1);       //定时器3初始化，定时器时钟为84M，分频系数为8400-1，
                                    //所以定时器3的频率为84M/8400=10K-->0.01ms，自动重装载为5000-1，那么定时器周期就是500ms-->5000*0.01ms,此时的5000==TIME3_COUNT
	
 	CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_3TQ,CAN_BS1_3TQ,6,CAN_MODE_NORMAL); //CAN初始化,波特率1000Kbps     
	CAN_Config();
	memset(str,0,len);	
    while(1)
    {
       if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\nyou send message len:%d,data:\r\n",len);
			memcpy(canbuf,USART_RX_BUF,len);
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
			//传入电机控制函数
			Motor_open_fanction_uart(canbuf,len);
			printf("%s\r\n",canbuf);
			for(i=0;i<len;i++)
			{
				printf("%x",canbuf[i]);
			}
			
		}else
		{
			times++;
			if(times%200==0)printf("\r\nplease send data use */ end \r\n");  
			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运
			delay_ms(10);   
		} 
		} 	
}

