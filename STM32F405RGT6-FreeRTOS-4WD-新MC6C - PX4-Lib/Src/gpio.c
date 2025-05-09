/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
	
	
	
	/* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();//ʹ��(����)���Ŷ�ӦIO�˿�ʱ��
	
	/*�������оƬ1��ӦGPIO��������*/
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;//�趨��Ӧ����IO���
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//�趨��Ӧ����IOΪ���ģʽ
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;//���Ӧ����IO�����ٶ�
	GPIO_InitStruct.Pull = GPIO_NOPULL;//�ڲ�����
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);//��ʼ����Ӧ����IO  
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
