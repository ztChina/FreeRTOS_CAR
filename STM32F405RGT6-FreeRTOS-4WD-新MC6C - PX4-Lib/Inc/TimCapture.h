#ifndef __TIMCAPTURE_H
#define __TIMCAPTURE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
	 
typedef struct
{
	__IO uint8_t DevID;//设备ID号
	__IO uint16_t uwIC_Value1;//捕获通道第一次捕获到边沿信号的计数器值
	__IO uint16_t uwIC_Value2;//捕获通道第二次捕获到边沿信号的计数器值
	__IO uint16_t uwIC_DiffCapture;//捕获通道两次捕获边沿信号的计数器差值
	__IO uint8_t uhIC_CaptureIndex;//捕获通道的反转标志位
	__IO uint8_t Tim_IT_Flag;//定时器溢出中断标志位
	__IO uint8_t UPdateFlag;//超声波测距的更新标志位
	__IO uint16_t Distance;//距离 mm
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_PIN;
}US_Struct;//超声波结构体参数

void US_Init(US_Struct* US, GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN);//超声波初始化函数
void US_Trig(void);//触发超声波模块发射声波
void US_DistanceUpdate(US_Struct *US, TIM_HandleTypeDef *htim, TIM_IC_InitTypeDef *sConfigIC);
	
#ifdef __cplusplus
}
#endif

#endif
