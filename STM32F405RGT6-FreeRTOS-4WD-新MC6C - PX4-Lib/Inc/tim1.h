#ifndef __TIM1_H
#define __TIM1_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_IC_InitTypeDef sConfigIC;

void MX_TIM1_Init(void);//TIM1-输入捕获初始化函数

typedef struct
{
	__IO uint16_t uwIC_Value1;//捕获通道第一次捕获到边沿信号的计数器值
	__IO uint16_t uwIC_Value2;//捕获通道第二次捕获到边沿信号的计数器值
	__IO uint32_t uwIC_DiffCapture;//捕获通道两次捕获边沿信号的计数器差值
	__IO uint8_t uhIC_CaptureIndex;//捕获通道的反转标志位
	__IO uint8_t TIM_IT_OverCount;//定时器溢出中断计数值
	__IO uint8_t UPdatePulseFlag;//捕获到脉宽的更新标志位
}CapturePulse_Struct;//捕获脉宽结构体参数

void CapturePulse_Init(CapturePulse_Struct* CapturePulse);//捕获脉宽初始化函数
void CapturePulseCallback(CapturePulse_Struct *CapturePulse, TIM_HandleTypeDef *htim);//捕获脉宽回调函数

#ifdef __cplusplus
}
#endif

#endif
