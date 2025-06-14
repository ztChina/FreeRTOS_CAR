#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

typedef struct
{	
	__IO int32_t CaptureCount;	//编码器捕获计数值
	__IO int16_t CaptureCount_Last;//缓存上一次的TIMx->CNT计数器值
	__IO int16_t Capture_D_Value;//编码器前后2次捕获计数的差值
}Encoder_Struct;//电机编码器结构体参数

extern __IO uint16_t TIM1_OverflowCount;//定时器1溢出次数计数值
extern __IO uint16_t TIM2_OverflowCount;//定时器2溢出次数计数值
extern __IO uint16_t TIM3_OverflowCount;//定时器3溢出次数计数值
extern __IO uint16_t TIM4_OverflowCount;//定时器4溢出次数计数值
extern __IO uint16_t TIM5_OverflowCount;//定时器5溢出次数计数值

void Encoder_Struct_Init(Encoder_Struct *Encoder);//初始化编码器结构体参数
void Encoder_UpdataValue(Encoder_Struct *Encoder, int16_t CaptureCount, int8_t Signed);	

#ifdef __cplusplus
}
#endif

#endif
