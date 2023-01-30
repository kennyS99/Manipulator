#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "can.h"
#include "motor_control.h"
/************************************************
ʹ��˵����
CAN1���ӵ�����򿪴��ڣ��۲��Ƿ��д�ӡ�����������Ϊ115200

�����������ָ�����ָ��Ϊ01������0��data=2000��
01000010200000000000*/
//������������ָ���mcu������Ӧָ�����ӡ����ֵ

/*************************************************/

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
    HAL_Init();                   	//��ʼ��HAL��    
    Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init(168);               	//��ʼ����ʱ����
	uart_init(115200);             	//��ʼ��USART
	
	LED_Init();						//��ʼ��LED	
	KEY_Init();						//��ʼ��KEY
 	LCD_Init();           			//��ʼ��LCD
	
 	CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_3TQ,CAN_BS1_3TQ,6,CAN_MODE_NORMAL); //CAN��ʼ��,������1000Kbps     
	CAN_Config();
	memset(str,0,len);	
    while(1)
    {
       if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\nyou send message len:%d,data:\r\n",len);
			memcpy(canbuf,USART_RX_BUF,len);
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
			//���������ƺ���
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
			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ������
			delay_ms(10);   
		} 
		} 	
}

