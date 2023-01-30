#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//CAN驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									////////////////////////////////////////////////
  
////////////////  //////////////////
CAN_HandleTypeDef	hcan1;   //CAN1句柄
CAN_TxHeaderTypeDef	Tx1Header;      //发送
CAN_RxHeaderTypeDef	Rx1Header;      //接收

////CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+tbs2+1)*brp); 其中tbs1和tbs2我们只用关注标识符上标志的序号，例如CAN_BS2_1TQ，我们就认为tbs2=1来计算即可。
//mode:CAN_MODE_NORMAL,普通模式;CAN_MODE_LOOPBACK,回环模式;
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_MODE_LOOPBACK);
//则波特率为:42M/((6+7+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
	CAN_InitTypeDef		CAN1_InitConf;
    
    hcan1.Instance=CAN1;
	
	hcan1.Init = CAN1_InitConf;
	
    hcan1.Init.Prescaler=brp;				//分频系数(Fdiv)为brp+1
    hcan1.Init.Mode=mode;					//模式设置 
    hcan1.Init.SyncJumpWidth=tsjw;			//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    hcan1.Init.TimeSeg1=tbs1;				//tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    hcan1.Init.TimeSeg2=tbs2;				//tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    hcan1.Init.TimeTriggeredMode=DISABLE;	//非时间触发通信模式 
    hcan1.Init.AutoBusOff=DISABLE;			//软件自动离线管理
    hcan1.Init.AutoWakeUp=DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    hcan1.Init.AutoRetransmission=ENABLE;	//禁止报文自动传送 
    hcan1.Init.ReceiveFifoLocked=DISABLE;	//报文不锁定,新的覆盖旧的 
    hcan1.Init.TransmitFifoPriority=DISABLE;	//优先级由报文标识符决定 
	
    if(HAL_CAN_Init(&hcan1)!=HAL_OK)			//初始化
		return 1;
    return 0;
}

//CAN底层驱动，引脚配置，时钟配置，中断配置
//此函数会被HAL_CAN_Init()调用
//hcan:CAN句柄
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //使能CAN1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			    //开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;   //PA11,12
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //快速
    GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //复用为CAN1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化
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

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u8 i=0;
	u32 TxMailbox;
	u8 message[8];
    Tx1Header.StdId=0X12;        //标准标识符
    Tx1Header.ExtId=0x12;        //扩展标识符(29位)
    Tx1Header.IDE=CAN_ID_STD;    //使用标准帧
    Tx1Header.RTR=CAN_RTR_DATA;  //数据帧
    Tx1Header.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&hcan1, &Tx1Header, message, &TxMailbox) != HAL_OK)//发送
	{
		return 1;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 3) {}
    return 0;
}

