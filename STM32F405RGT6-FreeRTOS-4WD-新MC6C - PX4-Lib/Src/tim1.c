#include "tim1.h"
#include "ShareWare.h"

TIM_HandleTypeDef htim1;
TIM_IC_InitTypeDef sConfigIC = {0};

/*-----------------------------------------------------------
��������: TIM1-���벶���ʼ������
�������: None
�� �� ֵ: None
˵    ��: TIM1-����Ϊ���벶��ģʽ�������ƽ����ʱ�䳤�� us
 -----------------------------------------------------------*/
void MX_TIM1_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 167;//��ʱ��Ԥ��Ƶ
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;//���ϼ���ģʽ
  htim1.Init.Period = 0xFFFF;//��ʱ������
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ
  htim1.Init.RepetitionCounter = 0;//���ظ�����
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;//�����Զ���װֵ
  HAL_TIM_IC_Init(&htim1);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;//����������
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;//��ӳ��IO
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;//����Ԥ��Ƶ��Ϊ0
  sConfigIC.ICFilter = 15;//���벶���˲��� ȡֵ0~15
	
  HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1);//����TIM1-CH1ͨ��
	HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2);//����TIM1-CH2ͨ��
	HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_3);//����TIM1-CH3ͨ��
  HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_4);//����TIM1-CH4ͨ��
	
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC1);//���TIM2-CH1�жϱ�־λ
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC2);//���TIM2-CH2�жϱ�־λ
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC3);//���TIM2-CH3�жϱ�־λ
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC4);//���TIM2-CH4�жϱ�־λ

	HAL_TIM_Base_Start_IT(&htim1);//����TIM1��ʱ���ж� 
  
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);//TIM1-CH1ͨ���������벶���ж�
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);//TIM1-CH2ͨ���������벶���ж�
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);//TIM1-CH3ͨ���������벶���ж�
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);//TIM1-CH4ͨ���������벶���ж�
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//���벶���жϺ���
{
	if(htim->Instance==TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			CapturePulseCallback(&CapturePulse_1, htim);
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			CapturePulseCallback(&CapturePulse_2, htim);
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			CapturePulseCallback(&CapturePulse_3, htim);
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			CapturePulseCallback(&CapturePulse_4, htim);
		}
	}		
}



/*-----------------------------------------------------------
��������: ���������ʼ������
�������: ��������ṹ�����
�� �� ֵ: None
˵    ��: None
 -----------------------------------------------------------*/
void CapturePulse_Init(CapturePulse_Struct* CapturePulse)
{
	CapturePulse->uwIC_Value1 = 0;//����ͨ����һ�β��񵽱����źŵļ�����ֵ
	CapturePulse->uwIC_Value2 = 0;//����ͨ���ڶ��β��񵽱����źŵļ�����ֵ
	CapturePulse->uwIC_DiffCapture = 0;//����ͨ�����β�������źŵļ�������ֵ
	CapturePulse->uhIC_CaptureIndex = 0;//����ͨ���ķ�ת��־λ
	CapturePulse->TIM_IT_OverCount = 0;//��ʱ������жϼ���ֵ
	CapturePulse->UPdatePulseFlag = 0;//��������ĸ��±�־λ
}

/*-----------------------------------------------------------
��������: ��������ص�����
�������: ��������ṹ���������ʱ�����
�� �� ֵ: None
˵    ��: None
 -----------------------------------------------------------*/
void CapturePulseCallback(CapturePulse_Struct *CapturePulse, TIM_HandleTypeDef *htim)
{
	TIM_IC_InitTypeDef sConfigIC = {0};
	
	if(CapturePulse->UPdatePulseFlag) return;//��ֹ�ص��źŶ��δ�����ɸ���
	
	if(!CapturePulse->uhIC_CaptureIndex)
	{
		/* Get the 1st Input Capture value */				
		CapturePulse->uhIC_CaptureIndex = 1;
		
		switch((uint8_t)htim->Channel)
		{
			case HAL_TIM_ACTIVE_CHANNEL_1:
			{
				CapturePulse->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//��ȡTIM-CH1ͨ������ֵ			
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);//�½��ش���				
				CapturePulse->TIM_IT_OverCount = 0;//��ʱ������жϼ���ֵ
			}break;
			
			case HAL_TIM_ACTIVE_CHANNEL_2:
			{
				CapturePulse->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);//��ȡTIM-CH2ͨ������ֵ			
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);//�½��ش���				
				CapturePulse->TIM_IT_OverCount = 0;//��ʱ������жϼ���ֵ
			}break;	
			
			case HAL_TIM_ACTIVE_CHANNEL_3:
			{
				CapturePulse->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);//��ȡTIM-CH3ͨ������ֵ			
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);//�½��ش���				
				CapturePulse->TIM_IT_OverCount = 0;//��ʱ������жϼ���ֵ
			}break;	
			
			case HAL_TIM_ACTIVE_CHANNEL_4:
			{
				CapturePulse->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);//��ȡTIM-CH4ͨ������ֵ			
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);//�½��ش���				
				CapturePulse->TIM_IT_OverCount = 0;//��ʱ������жϼ���ֵ
			}break;	
			default: break;
		}
	}
	else if(CapturePulse->uhIC_CaptureIndex)
	{
		/* Get the 2nd Input Capture value */
		CapturePulse->uhIC_CaptureIndex = 0;
		
		switch((uint8_t)htim->Channel)
		{
			case HAL_TIM_ACTIVE_CHANNEL_1:
			{
				CapturePulse->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//��ȡTIM-CH1ͨ������ֵ				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);//�����ش���			
			}break;

			case HAL_TIM_ACTIVE_CHANNEL_2:
			{
				CapturePulse->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);//��ȡTIM-CH2ͨ������ֵ				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);//�����ش���			
			}break;	
			
			case HAL_TIM_ACTIVE_CHANNEL_3:
			{
				CapturePulse->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);//��ȡTIM-CH3ͨ������ֵ				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);//�����ش���			
			}break;	
			
			case HAL_TIM_ACTIVE_CHANNEL_4:
			{
				CapturePulse->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);//��ȡTIM-CH4ͨ������ֵ				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);//�����ش���			
			}break;	
			default: break;
		}	
			
		/* Capture computation */
		CapturePulse->uwIC_DiffCapture = (65536 * CapturePulse->TIM_IT_OverCount) + CapturePulse->uwIC_Value2 - CapturePulse->uwIC_Value1;//����ͨ�����β�������źŵļ�������ֵ
		
		CapturePulse->UPdatePulseFlag = 1;//��������ĸ��±�־λ			
	}
}



