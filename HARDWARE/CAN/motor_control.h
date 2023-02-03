#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H
#include "config.h"
//----------------------------------------------------------------
//¶¨Òåµç»úIDÃ¶¾ÙÁÐ±í£»1-8
/**
* --------------------
* D1	D2	D3	D4	MODE
* OFF	OFF	OFF	OFF	0
* OFF	OFF	OFF	ON	1
* OFF	OFF	ON	ON	2
* OFF	OFF	ON	ON	3
* OFF	ON	OFF	OFF	4
* OFF	ON	OFF	ON	5
* OFF	ON	ON	OFF	6
* OFF	ON	ON	ON	7
* ON	ON	ON	ON	8
*
*/
//----------------------------------------------------------------
#define REAL_USED_MOTOR_NUM	   3

typedef enum
{
    _MOTOR_NUMBER_1 = 1,
    _MOTOR_NUMBER_2,
    _MOTOR_NUMBER_3,
    _MOTOR_NUMBER_4,
    _MOTOR_NUMBER_5,
    _MOTOR_NUMBER_6,
    _MOTOR_NUMBER_7,
    _MOTOR_NUMBER_8,
		_MOTOR_NUMBER_MAX = _MOTOR_NUMBER_8,
}MotorId;

typedef union
{
    uint8_t DATA[8];
    struct
    {
        uint8_t  ID:8;
        uint8_t  TEMP:8;
        uint16_t IQ:16;
        uint16_t SPEED:16;
        uint16_t ENCODER:16;
        /* data */
    }returndata;   
}speedControlreturn;

typedef struct
{
    uint8_t anglePidKp;
    uint8_t anglePidKi;
    uint8_t speedPidKp;
    uint8_t speedPidKi;
    uint8_t iqPidKp;
    uint8_t iqPidKi;

}motor_pid;

typedef struct
{
    uint16_t encoder;
    uint16_t encoderRaw;
    uint16_t encoderOffset;
		int64_t	 real_encoder;
}motor_Encoder;
extern motor_Encoder uart_get_encoder[_MOTOR_NUMBER_MAX];

typedef struct
{
    uint8_t temperature;
    uint16_t voltage;
    uint8_t errorState;
    int16_t iq;
    int16_t speed;
    uint16_t encoder;
    int16_t  phaseA_current;
    int16_t  phaseB_current;
    int16_t  phaseC_current;

}motor_state;


void Read_PID(MotorId Motor_ID,motor_pid* pid);
void Write_PID_to_RAM(MotorId Motor_ID,motor_pid pid);
void Write_PID_to_ROM(MotorId Motor_ID,motor_pid pid);
void Read_Accel(MotorId Motor_ID,int32_t* Accel);
void Write_Accel_to_RAM(MotorId Motor_ID,int32_t Accel);
void Read_Encoder(MotorId Motor_ID,motor_Encoder* Encoder);
void Write_Expect_Encoder_to_ROM(MotorId Motor_ID,motor_Encoder Encoder);
void Write_Current_Encoder_to_ROM(MotorId Motor_ID,motor_Encoder *Encoder);

void Read_MotorAngle(MotorId Motor_ID,int64_t* angle);
void Read_CircleAngle(MotorId Motor_ID,uint16_t* angle);

void Read_MotorState1(MotorId Motor_ID,motor_state* state);
void Read_MotorState2(MotorId Motor_ID,motor_state* state);
void Read_MotorState3(MotorId Motor_ID,motor_state* state);

void Clear_errorState(MotorId Motor_ID);

void Motor_Off(MotorId Motor_ID);
void Motor_Stop(MotorId Motor_ID);
void Motor_Run(MotorId Motor_ID);

motor_state iqControl(MotorId Motor_ID, int32_t iqControl);
motor_state speedControl(MotorId Motor_ID, int32_t speedControl);
motor_state Multi_angleControl_1(MotorId Motor_ID, int32_t angleControl);
motor_state Multi_angleControl_2(MotorId Motor_ID, uint16_t maxSpeed, int32_t angleControl);
motor_state Single_loop_angleControl_1(MotorId Motor_ID, uint8_t spinDirection, uint16_t angleControl);
motor_state Single_loop_angleControl_2(MotorId Motor_ID, uint8_t spinDirection, uint16_t maxSpeed, uint16_t angleControl);

void Motor_open_fanction_uart(uint8_t *buf,uint8_t size);  
int multiTurnEncoder(int currentEncoderValue);
void Error_Handler();

#endif

