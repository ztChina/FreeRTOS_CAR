#include "TimCapture.h"
#include "ShareWare.h"


/*-----------------------------------------------------------
��������: ��������ʼ������
�������: �������ṹ�������������Trig��Ӧ��GPIOx��������Trig��Ӧ��GPIO_PIN
�� �� ֵ: None
˵    ��: None
 -----------------------------------------------------------*/											
void US_Init(US_Struct* US, GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN)
{
	US->uwIC_Value1 = 0;//����ͨ����һ�β��񵽱����źŵļ�����ֵ
	US->uwIC_Value2 = 0;//����ͨ���ڶ��β��񵽱����źŵļ�����ֵ
	US->uwIC_DiffCapture = 0;//����ͨ�����β�������źŵļ�������ֵ
	US->uhIC_CaptureIndex = 0;//����ͨ���ķ�ת��־λ
	US->Tim_IT_Flag = 0;//��ʱ������жϱ�־λ
	US->UPdateFlag = 0;//���������ĸ��±�־λ
	US->Distance = 0;//���� mm
	US->GPIOx = GPIOx;
	US->GPIO_PIN = GPIO_PIN;

	/* ����IOӲ����ʼ���ṹ����� */
	GPIO_InitTypeDef GPIO_InitStruct;
	if(GPIOx == GPIOA) 			__HAL_RCC_GPIOA_CLK_ENABLE();//ʹ��(����)���Ŷ�ӦIO�˿�ʱ��
	else if(GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();//ʹ��(����)���Ŷ�ӦIO�˿�ʱ��
	else if(GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();//ʹ��(����)���Ŷ�ӦIO�˿�ʱ��
	else if(GPIOx == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();//ʹ��(����)���Ŷ�ӦIO�˿�ʱ��
	else if(GPIOx == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();//ʹ��(����)���Ŷ�ӦIO�˿�ʱ��
	else if(GPIOx == GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();//ʹ��(����)���Ŷ�ӦIO�˿�ʱ��
	else if(GPIOx == GPIOG) __HAL_RCC_GPIOG_CLK_ENABLE();//ʹ��(����)���Ŷ�ӦIO�˿�ʱ��
	
	/*GPIO��������*/
	GPIO_InitStruct.Pin = GPIO_PIN;//�趨��ӦIO���ű��
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//�趨��ӦIO����Ϊ���ģʽ
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//�趨��ӦIO���Ų����ٶ�
	HAL_GPIO_WritePin(GPIOx, GPIO_PIN, GPIO_PIN_RESET);//���ƶ�ӦIO��������͵�ƽ 
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);//��ʼ����ӦIO����
}



/*-----------------------------------------------------------
��������: ����������ģ�鷢������
�������: None
�� �� ֵ: None
˵    ��: 100msѭ�����øú�������Ƭ���ᴥ��������ģ�鷢�������ź�
 -----------------------------------------------------------*/
void US_Trig(void)
{
	static uint8_t Task = 0;
	if(Task)
	{
		Task = 0;
		US1.UPdateFlag=0;//���������ĸ��±�־λ����
		HAL_GPIO_WritePin(US1.GPIOx, US1.GPIO_PIN, GPIO_PIN_SET);//���ƶ�ӦIO��������ߵ�ƽ 
		delay_us(10);//us��ʱ����
		HAL_GPIO_WritePin(US1.GPIOx, US1.GPIO_PIN, GPIO_PIN_RESET);//���ƶ�ӦIO��������͵�ƽ 
	}
	else 
	{
		Task = 1;		
		US2.UPdateFlag=0;//���������ĸ��±�־λ����
		HAL_GPIO_WritePin(US2.GPIOx, US2.GPIO_PIN, GPIO_PIN_SET);//���ƶ�ӦIO��������ߵ�ƽ 
		delay_us(10);//us��ʱ����
		HAL_GPIO_WritePin(US2.GPIOx, US2.GPIO_PIN, GPIO_PIN_RESET);//���ƶ�ӦIO��������͵�ƽ 
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//���벶���жϺ���
{
	if(htim->Instance==TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			US_DistanceUpdate(&US1, htim, &sConfigIC);
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			US_DistanceUpdate(&US2, htim, &sConfigIC);
		}
	}		
}


void US_DistanceUpdate(US_Struct *US, TIM_HandleTypeDef *htim, TIM_IC_InitTypeDef *sConfigIC)
{
	if(US->UPdateFlag) return;//��ֹ�ص������źŶ��δ�����ɸ���
	
	if(US->uhIC_CaptureIndex == 0)
	{
		/* Get the 1st Input Capture value */				
		US->uhIC_CaptureIndex = 1;
		
		switch((uint8_t)htim->Channel)
		{
			case HAL_TIM_ACTIVE_CHANNEL_1:
			{
				US->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//��ȡTIM-CH1ͨ������ֵ
				sConfigIC->ICPolarity = TIM_ICPOLARITY_FALLING;//�½��ش���
				HAL_TIM_IC_ConfigChannel(htim, sConfigIC, TIM_CHANNEL_1);//����TIM-CH1ͨ��					
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);// ���TIM-CH1�жϱ�־λ
				HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);//TIM-CH1ͨ���������벶���ж�
				US->Tim_IT_Flag = 0;//��ʱ������жϱ�־λ
			}break;
			
			case HAL_TIM_ACTIVE_CHANNEL_3:
			{
				US->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);//��ȡTIM-CH3ͨ������ֵ
				sConfigIC->ICPolarity = TIM_ICPOLARITY_FALLING;//�½��ش���
				HAL_TIM_IC_ConfigChannel(htim, sConfigIC, TIM_CHANNEL_3);//����TIM-CH3ͨ��					
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC3);// ���TIM-CH3�жϱ�־λ
				HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_3);//TIM-CH3ͨ���������벶���ж�
				US->Tim_IT_Flag = 0;//��ʱ������жϱ�־λ
			}break;
			
			default: while(1){}; break;
		}
	}
	else if(US->uhIC_CaptureIndex == 1)
	{
		/* Get the 2nd Input Capture value */
		US->uhIC_CaptureIndex = 0;
		
		switch((uint8_t)htim->Channel)
		{
			case HAL_TIM_ACTIVE_CHANNEL_1:
			{
				US->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//��ȡTIM-CH1ͨ������ֵ
				sConfigIC->ICPolarity = TIM_ICPOLARITY_RISING;//�����ش���
				HAL_TIM_IC_ConfigChannel(htim, sConfigIC, TIM_CHANNEL_1);//����TIM-CH1ͨ��					
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);// ���TIM-CH1�жϱ�־λ
				HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);//TIM-CH1ͨ���������벶���ж�
			}break;
			
			case HAL_TIM_ACTIVE_CHANNEL_3:
			{
				US->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);//��ȡTIM-CH3ͨ������ֵ
				sConfigIC->ICPolarity = TIM_ICPOLARITY_RISING;//�����ش���
				HAL_TIM_IC_ConfigChannel(htim, sConfigIC, TIM_CHANNEL_3);//����TIM-CH3ͨ��					
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC3);// ���TIM-CH3�жϱ�־λ
				HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_3);//TIM-CH3ͨ���������벶���ж�
			}break;
			
			default: while(1){}; break;
		}	
			
		/* Capture computation */
		if ( (US->uwIC_Value2 > US->uwIC_Value1) && (US->Tim_IT_Flag<1) )
		{
			US->uwIC_DiffCapture = (US->uwIC_Value2 - US->uwIC_Value1);				
		}
		else if ( (US->uwIC_Value2 < US->uwIC_Value1) && (US->Tim_IT_Flag<2) )
		{
			/* 0xFFFF is max TIM2_CCRx value */
			US->uwIC_DiffCapture = ((0xFFFF - US->uwIC_Value1) + US->uwIC_Value2) + 1;
		}
		else US->uwIC_DiffCapture=22989;//ֱ�Ӹ�ֵʱ�䣬��������4000mm
		
		US->Distance = US->uwIC_DiffCapture * 0.174f;//��������õľ��� mm
		if(US->Distance >	4000) US->Distance = 4000;//�޶�������4000mm
		US->UPdateFlag=1;//����������������±�־λ							
	}
}
