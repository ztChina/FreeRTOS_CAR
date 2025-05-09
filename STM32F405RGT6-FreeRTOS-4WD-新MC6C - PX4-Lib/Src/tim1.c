#include "tim1.h"
#include "ShareWare.h"

TIM_HandleTypeDef htim1;
TIM_IC_InitTypeDef sConfigIC = {0};

/*-----------------------------------------------------------
函数功能: TIM1-输入捕获初始化函数
输入参数: None
返 回 值: None
说    明: TIM1-配置为输入捕获模式，捕获电平脉宽时间长度 us
 -----------------------------------------------------------*/
void MX_TIM1_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 167;//定时器预分频
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;//向上计数模式
  htim1.Init.Period = 0xFFFF;//定时器周期
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//时钟分频
  htim1.Init.RepetitionCounter = 0;//不重复计数
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;//禁用自动重装值
  HAL_TIM_IC_Init(&htim1);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;//捕获上升沿
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;//不映射IO
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;//捕获预分频器为0
  sConfigIC.ICFilter = 15;//输入捕获滤波器 取值0~15
	
  HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1);//配置TIM1-CH1通道
	HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2);//配置TIM1-CH2通道
	HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_3);//配置TIM1-CH3通道
  HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_4);//配置TIM1-CH4通道
	
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC1);//清除TIM2-CH1中断标志位
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC2);//清除TIM2-CH2中断标志位
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC3);//清除TIM2-CH3中断标志位
	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC4);//清除TIM2-CH4中断标志位

	HAL_TIM_Base_Start_IT(&htim1);//开启TIM1定时器中断 
  
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);//TIM1-CH1通道开启输入捕获中断
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);//TIM1-CH2通道开启输入捕获中断
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);//TIM1-CH3通道开启输入捕获中断
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);//TIM1-CH4通道开启输入捕获中断
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//输入捕获中断函数
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
函数功能: 捕获脉宽初始化函数
输入参数: 捕获脉宽结构体参数
返 回 值: None
说    明: None
 -----------------------------------------------------------*/
void CapturePulse_Init(CapturePulse_Struct* CapturePulse)
{
	CapturePulse->uwIC_Value1 = 0;//捕获通道第一次捕获到边沿信号的计数器值
	CapturePulse->uwIC_Value2 = 0;//捕获通道第二次捕获到边沿信号的计数器值
	CapturePulse->uwIC_DiffCapture = 0;//捕获通道两次捕获边沿信号的计数器差值
	CapturePulse->uhIC_CaptureIndex = 0;//捕获通道的反转标志位
	CapturePulse->TIM_IT_OverCount = 0;//定时器溢出中断计数值
	CapturePulse->UPdatePulseFlag = 0;//捕获到脉宽的更新标志位
}

/*-----------------------------------------------------------
函数功能: 捕获脉宽回调函数
输入参数: 捕获脉宽结构体参数，定时器句柄
返 回 值: None
说    明: None
 -----------------------------------------------------------*/
void CapturePulseCallback(CapturePulse_Struct *CapturePulse, TIM_HandleTypeDef *htim)
{
	TIM_IC_InitTypeDef sConfigIC = {0};
	
	if(CapturePulse->UPdatePulseFlag) return;//防止回荡信号二次触发造成干扰
	
	if(!CapturePulse->uhIC_CaptureIndex)
	{
		/* Get the 1st Input Capture value */				
		CapturePulse->uhIC_CaptureIndex = 1;
		
		switch((uint8_t)htim->Channel)
		{
			case HAL_TIM_ACTIVE_CHANNEL_1:
			{
				CapturePulse->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//读取TIM-CH1通道计数值			
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);//下降沿触发				
				CapturePulse->TIM_IT_OverCount = 0;//定时器溢出中断计数值
			}break;
			
			case HAL_TIM_ACTIVE_CHANNEL_2:
			{
				CapturePulse->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);//读取TIM-CH2通道计数值			
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);//下降沿触发				
				CapturePulse->TIM_IT_OverCount = 0;//定时器溢出中断计数值
			}break;	
			
			case HAL_TIM_ACTIVE_CHANNEL_3:
			{
				CapturePulse->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);//读取TIM-CH3通道计数值			
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);//下降沿触发				
				CapturePulse->TIM_IT_OverCount = 0;//定时器溢出中断计数值
			}break;	
			
			case HAL_TIM_ACTIVE_CHANNEL_4:
			{
				CapturePulse->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);//读取TIM-CH4通道计数值			
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);//下降沿触发				
				CapturePulse->TIM_IT_OverCount = 0;//定时器溢出中断计数值
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
				CapturePulse->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//读取TIM-CH1通道计数值				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);//上升沿触发			
			}break;

			case HAL_TIM_ACTIVE_CHANNEL_2:
			{
				CapturePulse->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);//读取TIM-CH2通道计数值				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);//上升沿触发			
			}break;	
			
			case HAL_TIM_ACTIVE_CHANNEL_3:
			{
				CapturePulse->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);//读取TIM-CH3通道计数值				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);//上升沿触发			
			}break;	
			
			case HAL_TIM_ACTIVE_CHANNEL_4:
			{
				CapturePulse->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);//读取TIM-CH4通道计数值				
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);//上升沿触发			
			}break;	
			default: break;
		}	
			
		/* Capture computation */
		CapturePulse->uwIC_DiffCapture = (65536 * CapturePulse->TIM_IT_OverCount) + CapturePulse->uwIC_Value2 - CapturePulse->uwIC_Value1;//捕获通道两次捕获边沿信号的计数器差值
		
		CapturePulse->UPdatePulseFlag = 1;//捕获到脉宽的更新标志位			
	}
}



