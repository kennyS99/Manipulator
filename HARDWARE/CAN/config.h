
#ifndef		__CONFIG_H
#define		__CONFIG_H

#include	"main.h"

typedef unsigned    char 							u8;
typedef unsigned 	short  int 	        u16;
typedef unsigned    int 							u32;

typedef unsigned    char 							uint8_t;
typedef unsigned 	short  int 	        uint16_t;
typedef          	short  int 	        int16_t;
typedef unsigned    int 							uint32_t;
typedef             int 							int32_t;


#define LED2_ON   	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);	
#define LED2_OFF    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);	

#define LED3_ON     HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);	
#define LED3_OFF    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);	

#define BEEP_ON   	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);	
#define BEEP_OFF   	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);	

#endif
