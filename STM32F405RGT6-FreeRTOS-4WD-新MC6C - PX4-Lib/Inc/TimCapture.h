#ifndef __TIMCAPTURE_H
#define __TIMCAPTURE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
	 
typedef struct
{
	__IO uint8_t DevID;//�豸ID��
	__IO uint16_t uwIC_Value1;//����ͨ����һ�β��񵽱����źŵļ�����ֵ
	__IO uint16_t uwIC_Value2;//����ͨ���ڶ��β��񵽱����źŵļ�����ֵ
	__IO uint16_t uwIC_DiffCapture;//����ͨ�����β�������źŵļ�������ֵ
	__IO uint8_t uhIC_CaptureIndex;//����ͨ���ķ�ת��־λ
	__IO uint8_t Tim_IT_Flag;//��ʱ������жϱ�־λ
	__IO uint8_t UPdateFlag;//���������ĸ��±�־λ
	__IO uint16_t Distance;//���� mm
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_PIN;
}US_Struct;//�������ṹ�����

void US_Init(US_Struct* US, GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN);//��������ʼ������
void US_Trig(void);//����������ģ�鷢������
void US_DistanceUpdate(US_Struct *US, TIM_HandleTypeDef *htim, TIM_IC_InitTypeDef *sConfigIC);
	
#ifdef __cplusplus
}
#endif

#endif
