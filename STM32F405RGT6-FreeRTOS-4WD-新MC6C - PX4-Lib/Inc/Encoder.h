#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

typedef struct
{	
	__IO int32_t CaptureCount;	//�������������ֵ
	__IO int16_t CaptureCount_Last;//������һ�ε�TIMx->CNT������ֵ
	__IO int16_t Capture_D_Value;//������ǰ��2�β�������Ĳ�ֵ
}Encoder_Struct;//����������ṹ�����

extern __IO uint16_t TIM1_OverflowCount;//��ʱ��1�����������ֵ
extern __IO uint16_t TIM2_OverflowCount;//��ʱ��2�����������ֵ
extern __IO uint16_t TIM3_OverflowCount;//��ʱ��3�����������ֵ
extern __IO uint16_t TIM4_OverflowCount;//��ʱ��4�����������ֵ
extern __IO uint16_t TIM5_OverflowCount;//��ʱ��5�����������ֵ

void Encoder_Struct_Init(Encoder_Struct *Encoder);//��ʼ���������ṹ�����
void Encoder_UpdataValue(Encoder_Struct *Encoder, int16_t CaptureCount, int8_t Signed);	

#ifdef __cplusplus
}
#endif

#endif
