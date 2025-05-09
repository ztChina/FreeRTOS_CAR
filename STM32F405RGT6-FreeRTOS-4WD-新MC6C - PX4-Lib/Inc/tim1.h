#ifndef __TIM1_H
#define __TIM1_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_IC_InitTypeDef sConfigIC;

void MX_TIM1_Init(void);//TIM1-���벶���ʼ������

typedef struct
{
	__IO uint16_t uwIC_Value1;//����ͨ����һ�β��񵽱����źŵļ�����ֵ
	__IO uint16_t uwIC_Value2;//����ͨ���ڶ��β��񵽱����źŵļ�����ֵ
	__IO uint32_t uwIC_DiffCapture;//����ͨ�����β�������źŵļ�������ֵ
	__IO uint8_t uhIC_CaptureIndex;//����ͨ���ķ�ת��־λ
	__IO uint8_t TIM_IT_OverCount;//��ʱ������жϼ���ֵ
	__IO uint8_t UPdatePulseFlag;//��������ĸ��±�־λ
}CapturePulse_Struct;//��������ṹ�����

void CapturePulse_Init(CapturePulse_Struct* CapturePulse);//���������ʼ������
void CapturePulseCallback(CapturePulse_Struct *CapturePulse, TIM_HandleTypeDef *htim);//��������ص�����

#ifdef __cplusplus
}
#endif

#endif
