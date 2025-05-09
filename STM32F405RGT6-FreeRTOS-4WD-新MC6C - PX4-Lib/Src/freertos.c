/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "ShareWare.h"

osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId IndicatorDeviceHandle;
osThreadId IMUHandle;
osThreadId KinematicsHandle;
osThreadId MotionControlHandle;
osThreadId BMSHandle;
osThreadId SerialInterfaceHandle;



void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void IndicatorDeviceTask(void const * argument);
void IMUTask(void const * argument);
void KinematicsTask(void const * argument);
void MotionControlTask(void const * argument);
void BMSTask(void const * argument);
void SerialInterfaceTask(void const * argument);

void MX_FREERTOS_Init(void);

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) 
{

//  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
//  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

//  osThreadDef(myTask02, StartTask02, osPriorityNormal, 0, 20);
//  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);





	//osThreadDef(...)：定义一个线程的 名字，任务函数、优先级、（在stm32中一般是0）、栈区大小。
	//osThreadCreate(...)：根据定义创建任务实例并启动，返回一个句柄
	//osPriorityNormal : 一般任务		osPriorityAboveNormal ：高于一般任务
	osThreadDef(IndicatorDevice, IndicatorDeviceTask, osPriorityNormal, 0, 512);
  IndicatorDeviceHandle = osThreadCreate(osThread(IndicatorDevice), NULL);//初始动作提示，负责LED指示灯、BEEP的响声
	
	osThreadDef(IMU, IMUTask, osPriorityAboveNormal, 0, 512);
  IMUHandle = osThreadCreate(osThread(IMU), NULL);//IMU线程
	
	osThreadDef(Kinematics, KinematicsTask, osPriorityNormal, 0, 512);
  KinematicsHandle = osThreadCreate(osThread(Kinematics), NULL);//动力学计算
	
	osThreadDef(MotionControl, MotionControlTask, osPriorityNormal, 0, 512);
  MotionControlHandle = osThreadCreate(osThread(MotionControl), NULL);//电机控制
	
	osThreadDef(SerialInterface, SerialInterfaceTask, osPriorityNormal, 0, 512);
  SerialInterfaceHandle = osThreadCreate(osThread(SerialInterface), NULL);//串口通信，上位机信息
	
	osThreadDef(BMS, BMSTask, osPriorityNormal, 0, 512);
  BMSHandle = osThreadCreate(osThread(BMS), NULL);//电池管理系统（BMS）
	
}


void StartDefaultTask(void const * argument)
{
  for(;;)
  {

    osDelay(50);
  }
}


void StartTask02(void const * argument)
{
  for(;;)
  {
		//IndicatorDeviceControl(&LED_Red, 2, 100);//绿色LED控制函数
    osDelay(1000);
//		vTaskSuspend(NULL);//挂起该任务
  }
}


void task(void)//任务恢复
{
//	BaseType_t YieldRequired;
//	YieldRequired = xTaskResumeFromISR(myTask02Handle);
//	if(YieldRequired == pdTRUE)
//	{
//		portYIELD_FROM_ISR(YieldRequired);//注意一下带ISR是中断函数中才调用的，B站视频中该函数是在按键中断函数中调用的
//	}			
}

void IndicatorDeviceTask(void const * argument)
{
	taskENTER_CRITICAL();//进入临界区，临界区内不可被打断
	IndicatorDevice_Init(&Buzzer, GPIOC, GPIO_PIN_13);//提示设备初始化配置函数
	IndicatorDevice_Init(&LED_Gre, GPIOC, GPIO_PIN_14);//提示设备初始化配置函数
	IndicatorDevice_Init(&LED_Red, GPIOC, GPIO_PIN_15);//提示设备初始化配置函数
	taskEXIT_CRITICAL();//退出临界区
	
	IndicatorDeviceControl(&Buzzer, 2, 50);//蜂鸣器控制函数
	IndicatorDeviceControl(&LED_Gre, 2, 50);//绿色LED控制函数
	IndicatorDeviceControl(&LED_Red, 2, 50);//红色LED控制函数
	
  for(;;)
  {
		osDelay(1);
		IndicatorDeviceLoop(&Buzzer);//指示设备循环函数
		IndicatorDeviceLoop(&LED_Gre);//指示设备循环函数
		IndicatorDeviceLoop(&LED_Red);//指示设备循环函数   
  }
}


void IMUTask(void const * argument)
{
	taskENTER_CRITICAL();//进入临界区
	MX_SPI1_Init();//SPI1初始化函数
	SPI1_CSX_GPIO_Init();//SPI1片选IO初始化
	taskEXIT_CRITICAL();//退出临界区
	
	while(!ICM42605_Init())//等待传感器初始化成功
	{
		osDelay(100); 		
		IndicatorDeviceControl(&LED_Red, 1, 100);//红色LED控制函数
		IndicatorDeviceControl(&Buzzer, 1, 100);//蜂鸣器控制函数
	}
	
	for(;;)
	{
		osDelay(1);
		ICM42605_Read_FIFO(&ICM42605);//读ICM42605 FIFO值		
	}
}

void KinematicsTask(void const * argument)
{
	taskENTER_CRITICAL();//进入临界区
	MX_TIM8_Init();//TIM8-PWM初始化函数
	MX_TIM2_Init();//TIM2-编码器模式初始化
	MX_TIM3_Init();//TIM3-编码器模式初始化
	MX_TIM4_Init();//TIM4-编码器模式初始化
	MX_TIM5_Init();//TIM5-编码器模式初始化
	Motor1_GPIO_Init();//电机驱动1 IO初始化函数
  Motor2_GPIO_Init();//电机驱动2 IO初始化函数
  Motor3_GPIO_Init();//电机驱动3 IO初始化函数
  Motor4_GPIO_Init();//电机驱动4 IO初始化函数
	Encoder_Struct_Init(&Encoder_M1);//初始化编码器结构体参数
	Encoder_Struct_Init(&Encoder_M2);//初始化编码器结构体参数
	Encoder_Struct_Init(&Encoder_M3);//初始化编码器结构体参数
	Encoder_Struct_Init(&Encoder_M4);//初始化编码器结构体参数
	LPF_Struct_Init(&LPF_RPM1, 0.05);//低通滤波结构体参数初始化
	LPF_Struct_Init(&LPF_RPM2, 0.05);//低通滤波结构体参数初始化
	LPF_Struct_Init(&LPF_RPM3, 0.05);//低通滤波结构体参数初始化
	LPF_Struct_Init(&LPF_RPM4, 0.05);//低通滤波结构体参数初始化
//	IncPID_Init(&PID_M1, -3.95, -0.811, -0.62);//增量式PID参数初始化
//	IncPID_Init(&PID_M2, -3.95, -0.811, -0.62);//增量式PID参数初始化
//	IncPID_Init(&PID_M3, -3.95, -0.811, -0.62);//增量式PID参数初始化
//	IncPID_Init(&PID_M4, -3.95, -0.811, -0.62);//增量式PID参数初始化

	IncPID_Init(&PID_M1, -2.5f, -0.8f, -0.3f);//增量式PID参数初始化
	IncPID_Init(&PID_M2, -2.5f, -0.8f, -0.3f);//增量式PID参数初始化
	IncPID_Init(&PID_M3, -2.5f, -0.8f, -0.3f);//增量式PID参数初始化
	IncPID_Init(&PID_M4, -2.5f, -0.8f, -0.3f);//增量式PID参数初始化
	taskEXIT_CRITICAL();//退出临界区
	
	uint16_t tim1=0,tim2=0,tim3=0,tim4=0;
	
	for(;;)
	{
		osDelay(5);
		Kinematics_4WD_CalculateRPM(&Kinematics_Inverse);//4WD运动学逆解函数
		/*-------对“目标转速“进行限值-------*/	
		Kinematics_Inverse.M1_RPM = Constrain(Kinematics_Inverse.M1_RPM, -RPM_MAX, RPM_MAX);
		Kinematics_Inverse.M2_RPM = Constrain(Kinematics_Inverse.M2_RPM, -RPM_MAX, RPM_MAX);
		Kinematics_Inverse.M3_RPM = Constrain(Kinematics_Inverse.M3_RPM, -RPM_MAX, RPM_MAX);
		Kinematics_Inverse.M4_RPM = Constrain(Kinematics_Inverse.M4_RPM, -RPM_MAX, RPM_MAX);
				
		LPF_RPM1.SampleValue = Kinematics_Inverse.M1_RPM;//通过采样转速值给低通滤波器结构体中的采集值
		VelControl.M1_SetRPM = LowPassFilter(&LPF_RPM1); //转速值经过低通滤波器后数值会更加平滑			
		LPF_RPM2.SampleValue = Kinematics_Inverse.M2_RPM;//通过采样转速值给低通滤波器结构体中的采集值
		VelControl.M2_SetRPM = LowPassFilter(&LPF_RPM2); //转速值经过低通滤波器后数值会更加平滑	
		LPF_RPM3.SampleValue = Kinematics_Inverse.M3_RPM;//通过采样转速值给低通滤波器结构体中的采集值
		VelControl.M3_SetRPM = LowPassFilter(&LPF_RPM3); //转速值经过低通滤波器后数值会更加平滑	
		LPF_RPM4.SampleValue = Kinematics_Inverse.M4_RPM;//通过采样转速值给低通滤波器结构体中的采集值
		VelControl.M4_SetRPM = LowPassFilter(&LPF_RPM4); //转速值经过低通滤波器后数值会更加平滑		
		
		/*---------------PX4 PWM Control---------------*/
		if(CapturePulse_1.UPdatePulseFlag) 
		{ 
			if(CapturePulse_1.uwIC_DiffCapture > 900 && CapturePulse_1.uwIC_DiffCapture < 2100) tim1 = 10;
			CapturePulse_1.uwIC_DiffCapture = Constrain(CapturePulse_1.uwIC_DiffCapture, 1000, 2000);//约束大小
			CapturePulse_1.UPdatePulseFlag = 0; 
		}else if(tim1) tim1--;
		
		if(CapturePulse_2.UPdatePulseFlag) 
		{ 
			if(CapturePulse_2.uwIC_DiffCapture > 900 && CapturePulse_2.uwIC_DiffCapture < 2100) tim2 = 10;
			CapturePulse_2.uwIC_DiffCapture = Constrain(CapturePulse_2.uwIC_DiffCapture, 1000, 2000);//约束大小
			CapturePulse_2.UPdatePulseFlag = 0; 
		}else if(tim2) tim2--;
		
		if(CapturePulse_3.UPdatePulseFlag) 
		{ 
			if(CapturePulse_3.uwIC_DiffCapture > 900 && CapturePulse_3.uwIC_DiffCapture < 2100) tim3 = 10;
			CapturePulse_3.uwIC_DiffCapture = Constrain(CapturePulse_3.uwIC_DiffCapture, 1000, 2000);//约束大小
			CapturePulse_3.UPdatePulseFlag = 0; 
		}else if(tim3) tim3--;
		
		if(CapturePulse_4.UPdatePulseFlag) 
		{ 
			if(CapturePulse_4.uwIC_DiffCapture > 900 && CapturePulse_4.uwIC_DiffCapture < 2100) tim4 = 10;
			CapturePulse_4.uwIC_DiffCapture = Constrain(CapturePulse_4.uwIC_DiffCapture, 1000, 2000);//约束大小
			CapturePulse_4.UPdatePulseFlag = 0; 
		}else if(tim4) tim4--;
		
		if(tim1 || tim2 || tim3 || tim4)
		{
			if(tim1)
			{
				int32_t PWM = CapturePulse_1.uwIC_DiffCapture - 1500;			
				if(PWM > 0)
				{			
					Motor1_PWM = Motor3_PWM = -(PWM * MotorPWM_Max / 500);										
					Motor1_Reverse();//电机1反转
					Motor3_Reverse();//电机3反转
				}
				else
				{
					Motor1_PWM = Motor3_PWM = (PWM * MotorPWM_Max / 500);
					Motor1_Forward();//电机1正传
					Motor3_Forward();//电机3正传
				}
			}else Motor1_PWM = Motor3_PWM = MotorPWM_Min;
			
			if(tim2)
			{
				int32_t PWM = CapturePulse_2.uwIC_DiffCapture - 1500;			
				if(PWM > 0)
				{			
					Motor2_PWM = Motor4_PWM = -(PWM * MotorPWM_Max / 500);
					Motor2_Reverse();//电机2反转
					Motor4_Reverse();//电机4反转					
				}
				else
				{
					Motor2_PWM = Motor4_PWM = (PWM * MotorPWM_Max / 500);
					Motor2_Forward();//电机2正传
					Motor4_Forward();//电机4正传			
				}
			}else Motor2_PWM = Motor4_PWM = MotorPWM_Min;
		}
		else 
		{
			/*----------------------堵转保护----------------------*/
			static uint16_t M1_StallTime = 0;//M1电机堵转时间
			static uint16_t M2_StallTime = 0;//M1电机堵转时间
			static uint16_t M3_StallTime = 0;//M1电机堵转时间
			static uint16_t M4_StallTime = 0;//M1电机堵转时间
			static uint8_t Mx_StallFlag = 0; //电机堵转标志位

			if(fabs(VelControl.M1_SetRPM) > 10)//低于这个转速不启用保护，低转速下编码器数值不稳定
			{
				if(fabs(VelControl.M1_SetRPM - VelControl.M1_RPM) > fabs(VelControl.M1_SetRPM / 2.0)) M1_StallTime++;
				else if(M1_StallTime) M1_StallTime--;
			}else if(M1_StallTime) M1_StallTime--;
			
			if(fabs(VelControl.M2_SetRPM) > 10)//低于这个转速不启用保护，低转速下编码器数值不稳定
			{
				if(fabs(VelControl.M2_SetRPM - VelControl.M2_RPM) > fabs(VelControl.M2_SetRPM / 2.0)) M2_StallTime++;
				else if(M2_StallTime) M2_StallTime--;
			}else if(M2_StallTime) M2_StallTime--;
			
			if(fabs(VelControl.M3_SetRPM) > 10)//低于这个转速不启用保护，低转速下编码器数值不稳定
			{
				if(fabs(VelControl.M3_SetRPM - VelControl.M3_RPM) > fabs(VelControl.M3_SetRPM / 2.0)) M3_StallTime++;
				else if(M3_StallTime) M3_StallTime--;
			}else if(M3_StallTime) M3_StallTime--;
			
			if(fabs(VelControl.M4_SetRPM) > 10)//低于这个转速不启用保护，低转速下编码器数值不稳定
			{
				if(fabs(VelControl.M4_SetRPM - VelControl.M4_RPM) > fabs(VelControl.M4_SetRPM / 2.0)) M4_StallTime++;
				else if(M4_StallTime) M4_StallTime--;
			}else if(M4_StallTime) M4_StallTime--;
			
			if((M1_StallTime > 600) || (M2_StallTime > 600) || (M3_StallTime > 600) || (M4_StallTime > 600) || Mx_StallFlag)//触发堵转保护
			{
				VelControl.M1_SetRPM = VelControl.M2_SetRPM = VelControl.M3_SetRPM = VelControl.M4_SetRPM = 0;
				Mx_StallFlag = 1;
				IndicatorDeviceControl(&Buzzer, 1, 500);//蜂鸣器控制函数
			}
			
			VelocityControl_4WD(&VelControl);//4WD速度闭环控制函数
		}
		/*---------------------------------------------*/		
			
		/*----------------------车体当前线速度角速度采样滤波----------------------*/
		static uint8_t i = 0;
		i++;
		Kinematics_Forward.M1_RPM += VelControl.M1_RPM;
		Kinematics_Forward.M2_RPM += VelControl.M2_RPM;
		Kinematics_Forward.M3_RPM += VelControl.M3_RPM;
		Kinematics_Forward.M4_RPM += VelControl.M4_RPM;
		
		if(i > 1) 
		{
			Kinematics_Forward.M1_RPM = (float)Kinematics_Forward.M1_RPM / i;//两次取平均值
			Kinematics_Forward.M2_RPM = (float)Kinematics_Forward.M2_RPM / i;
			Kinematics_Forward.M3_RPM = (float)Kinematics_Forward.M3_RPM / i;
			Kinematics_Forward.M4_RPM = (float)Kinematics_Forward.M4_RPM / i;
			Kinematics_4WD_GetVelocities(&Kinematics_Forward);//4WD运动学正解函数
			i = Kinematics_Forward.M1_RPM = Kinematics_Forward.M2_RPM = Kinematics_Forward.M3_RPM = Kinematics_Forward.M4_RPM = 0;//清0
		}	
	}
}

void MotionControlTask(void const * argument)
{
	taskENTER_CRITICAL();//进入临界区
	MX_UART4_Init();//UART4初始化函数
	RC_Init(&RC);
	taskEXIT_CRITICAL();//退出临界区
	
	for(;;)
	{
		osDelay(10);
		if(RC.LifeTime) RC.LifeTime--;

		if(RC.LifeTime && RC.Switch==RC_DOWN)//判断遥控器与接收机是否处于正常通信状态
		{		
			SystemState = 0x05;//遥控状态
			IndicatorDeviceControl(&LED_Gre, 1, 50);//绿色LED控制函数	
			LPF_RPM1.a = LPF_RPM2.a = LPF_RPM3.a = LPF_RPM4.a = 0.05f;
			Kinematics_Inverse.Linear_X = RC.Left_Y * 1.5f;//左侧Y摇杆控制车体X轴线速度
			//Kinematics_Inverse.Linear_Y = RC.Left_X * 1.5f;//左侧X摇杆控制车体Y轴线速度
			Kinematics_Inverse.Angular_Z = RC.Right_X * 2.0f;//右侧X摇杆控制车体Z轴角速度	
			DataCom_RX.Linear_X = DataCom_RX.Linear_Y = DataCom_RX.Angular_Z = 0;	
			DataCom_RX.HeartbeatTime = 0;//心跳包时间
		}
		else if(DataCom_RX.HeartbeatTime)//如果心跳周期维持正常
		{
			SystemState = 0x06;//线控状态
			LPF_RPM1.a = LPF_RPM2.a = LPF_RPM3.a = LPF_RPM4.a = 1.0f;
			Kinematics_Inverse.Linear_X = DataCom_RX.Linear_X;
			//Kinematics_Inverse.Linear_Y = DataCom_RX.Linear_Y;
			Kinematics_Inverse.Angular_Z = DataCom_RX.Angular_Z;
			DataCom_RX.HeartbeatTime--;
		}
		else//心跳超时刹车
		{
			SystemState = 0x00;//待机状态
			LPF_RPM1.a = LPF_RPM2.a = LPF_RPM3.a = LPF_RPM4.a = 0.05f;
			DataCom_RX.Linear_X = DataCom_RX.Linear_Y = DataCom_RX.Angular_Z = 0;									
			Kinematics_Inverse.Linear_X = 0;//控制车体线速度 m/s
			Kinematics_Inverse.Linear_Y = 0;//控制车体线速度 m/s
			Kinematics_Inverse.Angular_Z = 0;//控制车体角速度 rad/s			
		}		
	}
}

void SerialInterfaceTask(void const * argument)
{
	taskENTER_CRITICAL();//进入临界区	
	MX_USART2_Init();//USART2初始化函数
	MastMessage_Queue = xQueueCreate(100, sizeof(uint8_t));//创建主机消息队列，队列项长度是100个uint8_t大小
	ParseData_Init(&ParseData_Mast, 30);//解析数据参数初始化
	taskEXIT_CRITICAL();//退出临界区
	
	for(;;)
	{		
		osDelay(1);	
		if(MastMessage_Queue != NULL)//消息队列MastMessage_Queue创建成功
		{
			while(!ParseData_Mast.Flag)//判断解析出的数据是否已处理
			{
				uint8_t Data;
				if(xQueueReceive(MastMessage_Queue, &Data, 0) == pdTRUE)//若从主机消息队列中成功获取到数据则执行,0参数表示不等待
				{
					ParseDataFunction(&ParseData_Mast, Data);//解析数据
				}
			}
		}
		
		if(HAL_UART_GetState(&huart2) != HAL_UART_STATE_BUSY_TX)//避免串口发送未完成又执行串口发送
		{				
			if(ParseData_Mast.Flag)//若解析出一帧数据，则进入
			{
				IndicatorDeviceControl(&LED_Gre, 1, 10);//绿色LED控制函数				
				DataCom_RX_Function(&DataCom_RX, (uint8_t*)ParseData_Mast.Buff);//数据接收接口
				ParseData_Mast.Flag = 0;//数据解析状态标志位清0
			}			
		}
	}
}

void BMSTask(void const * argument)
{
	taskENTER_CRITICAL();//进入临界区	
	MX_ADC1_Init();//ADC1初始化函数
	Battery_Init(&Battery, 0.3f);//电池状态初始化函数
	taskEXIT_CRITICAL();//退出临界区
	
	for(;;)
	{
		osDelay(500);//延时函数必须放方面，否则Battery.Voltage等于nan
		UpdateBatteryInfor(&Battery, (uint16_t*)ADC1_DMA_Buff);//更新电池状态信息		
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
