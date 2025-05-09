#include "TimCapture.h"
#include "ShareWare.h"


/*-----------------------------------------------------------
函数功能: 超声波初始化函数
输入参数: 超声波结构体参数，超声波Trig对应的GPIOx，超声波Trig对应的GPIO_PIN
返 回 值: None
说    明: None
 -----------------------------------------------------------*/											
void US_Init(US_Struct* US, GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN)
{
	US->uwIC_Value1 = 0;//捕获通道第一次捕获到边沿信号的计数器值
	US->uwIC_Value2 = 0;//捕获通道第二次捕获到边沿信号的计数器值
	US->uwIC_DiffCapture = 0;//捕获通道两次捕获边沿信号的计数器差值
	US->uhIC_CaptureIndex = 0;//捕获通道的反转标志位
	US->Tim_IT_Flag = 0;//定时器溢出中断标志位
	US->UPdateFlag = 0;//超声波测距的更新标志位
	US->Distance = 0;//距离 mm
	US->GPIOx = GPIOx;
	US->GPIO_PIN = GPIO_PIN;

	/* 定义IO硬件初始化结构体变量 */
	GPIO_InitTypeDef GPIO_InitStruct;
	if(GPIOx == GPIOA) 			__HAL_RCC_GPIOA_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	else if(GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	else if(GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	else if(GPIOx == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	else if(GPIOx == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	else if(GPIOx == GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	else if(GPIOx == GPIOG) __HAL_RCC_GPIOG_CLK_ENABLE();//使能(开启)引脚对应IO端口时钟
	
	/*GPIO引脚配置*/
	GPIO_InitStruct.Pin = GPIO_PIN;//设定对应IO引脚编号
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//设定对应IO引脚为输出模式
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//设定对应IO引脚操作速度
	HAL_GPIO_WritePin(GPIOx, GPIO_PIN, GPIO_PIN_RESET);//控制对应IO引脚输出低电平 
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);//初始化对应IO引脚
}



/*-----------------------------------------------------------
函数功能: 触发超声波模块发射声波
输入参数: None
返 回 值: None
说    明: 100ms循环调用该函数，单片机会触发超声波模块发射声波信号
 -----------------------------------------------------------*/
void US_Trig(void)
{
	static uint8_t Task = 0;
	if(Task)
	{
		Task = 0;
		US1.UPdateFlag=0;//超声波测距的更新标志位清零
		HAL_GPIO_WritePin(US1.GPIOx, US1.GPIO_PIN, GPIO_PIN_SET);//控制对应IO引脚输出高电平 
		delay_us(10);//us延时函数
		HAL_GPIO_WritePin(US1.GPIOx, US1.GPIO_PIN, GPIO_PIN_RESET);//控制对应IO引脚输出低电平 
	}
	else 
	{
		Task = 1;		
		US2.UPdateFlag=0;//超声波测距的更新标志位清零
		HAL_GPIO_WritePin(US2.GPIOx, US2.GPIO_PIN, GPIO_PIN_SET);//控制对应IO引脚输出高电平 
		delay_us(10);//us延时函数
		HAL_GPIO_WritePin(US2.GPIOx, US2.GPIO_PIN, GPIO_PIN_RESET);//控制对应IO引脚输出低电平 
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//输入捕获中断函数
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
	if(US->UPdateFlag) return;//防止回荡声波信号二次触发造成干扰
	
	if(US->uhIC_CaptureIndex == 0)
	{
		/* Get the 1st Input Capture value */				
		US->uhIC_CaptureIndex = 1;
		
		switch((uint8_t)htim->Channel)
		{
			case HAL_TIM_ACTIVE_CHANNEL_1:
			{
				US->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//读取TIM-CH1通道计数值
				sConfigIC->ICPolarity = TIM_ICPOLARITY_FALLING;//下降沿触发
				HAL_TIM_IC_ConfigChannel(htim, sConfigIC, TIM_CHANNEL_1);//配置TIM-CH1通道					
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);// 清除TIM-CH1中断标志位
				HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);//TIM-CH1通道开启输入捕获中断
				US->Tim_IT_Flag = 0;//定时器溢出中断标志位
			}break;
			
			case HAL_TIM_ACTIVE_CHANNEL_3:
			{
				US->uwIC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);//读取TIM-CH3通道计数值
				sConfigIC->ICPolarity = TIM_ICPOLARITY_FALLING;//下降沿触发
				HAL_TIM_IC_ConfigChannel(htim, sConfigIC, TIM_CHANNEL_3);//配置TIM-CH3通道					
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC3);// 清除TIM-CH3中断标志位
				HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_3);//TIM-CH3通道开启输入捕获中断
				US->Tim_IT_Flag = 0;//定时器溢出中断标志位
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
				US->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//读取TIM-CH1通道计数值
				sConfigIC->ICPolarity = TIM_ICPOLARITY_RISING;//上升沿触发
				HAL_TIM_IC_ConfigChannel(htim, sConfigIC, TIM_CHANNEL_1);//配置TIM-CH1通道					
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);// 清除TIM-CH1中断标志位
				HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);//TIM-CH1通道开启输入捕获中断
			}break;
			
			case HAL_TIM_ACTIVE_CHANNEL_3:
			{
				US->uwIC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);//读取TIM-CH3通道计数值
				sConfigIC->ICPolarity = TIM_ICPOLARITY_RISING;//上升沿触发
				HAL_TIM_IC_ConfigChannel(htim, sConfigIC, TIM_CHANNEL_3);//配置TIM-CH3通道					
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC3);// 清除TIM-CH3中断标志位
				HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_3);//TIM-CH3通道开启输入捕获中断
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
		else US->uwIC_DiffCapture=22989;//直接赋值时间，计算后等于4000mm
		
		US->Distance = US->uwIC_DiffCapture * 0.174f;//超声波测得的距离 mm
		if(US->Distance >	4000) US->Distance = 4000;//限定最大距离4000mm
		US->UPdateFlag=1;//超声波测量距离更新标志位							
	}
}
