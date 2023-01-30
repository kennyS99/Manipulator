#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//CAN��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									////////////////////////////////////////////////
  
////////////////  //////////////////
CAN_HandleTypeDef	hcan1;   //CAN1���
CAN_TxHeaderTypeDef	Tx1Header;      //����
CAN_RxHeaderTypeDef	Rx1Header;      //����

////CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+tbs2+1)*brp); ����tbs1��tbs2����ֻ�ù�ע��ʶ���ϱ�־����ţ�����CAN_BS2_1TQ�����Ǿ���Ϊtbs2=1�����㼴�ɡ�
//mode:CAN_MODE_NORMAL,��ͨģʽ;CAN_MODE_LOOPBACK,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_MODE_LOOPBACK);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
	CAN_InitTypeDef		CAN1_InitConf;
    
    hcan1.Instance=CAN1;
	
	hcan1.Init = CAN1_InitConf;
	
    hcan1.Init.Prescaler=brp;				//��Ƶϵ��(Fdiv)Ϊbrp+1
    hcan1.Init.Mode=mode;					//ģʽ���� 
    hcan1.Init.SyncJumpWidth=tsjw;			//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    hcan1.Init.TimeSeg1=tbs1;				//tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    hcan1.Init.TimeSeg2=tbs2;				//tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    hcan1.Init.TimeTriggeredMode=DISABLE;	//��ʱ�䴥��ͨ��ģʽ 
    hcan1.Init.AutoBusOff=DISABLE;			//����Զ����߹���
    hcan1.Init.AutoWakeUp=DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    hcan1.Init.AutoRetransmission=ENABLE;	//��ֹ�����Զ����� 
    hcan1.Init.ReceiveFifoLocked=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ� 
    hcan1.Init.TransmitFifoPriority=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
	
    if(HAL_CAN_Init(&hcan1)!=HAL_OK)			//��ʼ��
		return 1;
    return 0;
}

//CAN�ײ��������������ã�ʱ�����ã��ж�����
//�˺����ᱻHAL_CAN_Init()����
//hcan:CAN���
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //ʹ��CAN1ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			    //����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;   //PA11,12
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //����
    GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //����ΪCAN1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //��ʼ��
}

void CAN_Config(void)
{
  CAN_FilterTypeDef  sFilterConfig;

  /*##-2- Configure the CAN Filter ###########################################*/
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
    while(1)
	  {
	  }
  }

  /*##-3- Start the CAN peripheral ###########################################*/
  if (HAL_CAN_Start(&hcan1) != HAL_OK)
  {
    /* Start Error */
    while(1)
	  {
	  }
  }

  /*##-4- Activate CAN RX notification #######################################*/
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    /* Notification Error */
    while(1)
	  {
	  }
  }

  /*##-5- Configure Transmission process #####################################*/
  Tx1Header.StdId = 0x321;
  Tx1Header.ExtId = 0x01;
  Tx1Header.RTR = CAN_RTR_DATA;
  Tx1Header.IDE = CAN_ID_STD;
  Tx1Header.DLC = 2;
  Tx1Header.TransmitGlobalTime = DISABLE;
}

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u8 i=0;
	u32 TxMailbox;
	u8 message[8];
    Tx1Header.StdId=0X12;        //��׼��ʶ��
    Tx1Header.ExtId=0x12;        //��չ��ʶ��(29λ)
    Tx1Header.IDE=CAN_ID_STD;    //ʹ�ñ�׼֡
    Tx1Header.RTR=CAN_RTR_DATA;  //����֡
    Tx1Header.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&hcan1, &Tx1Header, message, &TxMailbox) != HAL_OK)//����
	{
		return 1;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 3) {}
    return 0;
}

