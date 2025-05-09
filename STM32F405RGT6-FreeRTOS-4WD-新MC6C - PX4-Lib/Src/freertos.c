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





	//osThreadDef(...)������һ���̵߳� ���֣������������ȼ�������stm32��һ����0����ջ����С��
	//osThreadCreate(...)�����ݶ��崴������ʵ��������������һ�����
	//osPriorityNormal : һ������		osPriorityAboveNormal ������һ������
	osThreadDef(IndicatorDevice, IndicatorDeviceTask, osPriorityNormal, 0, 512);
  IndicatorDeviceHandle = osThreadCreate(osThread(IndicatorDevice), NULL);//��ʼ������ʾ������LEDָʾ�ơ�BEEP������
	
	osThreadDef(IMU, IMUTask, osPriorityAboveNormal, 0, 512);
  IMUHandle = osThreadCreate(osThread(IMU), NULL);//IMU�߳�
	
	osThreadDef(Kinematics, KinematicsTask, osPriorityNormal, 0, 512);
  KinematicsHandle = osThreadCreate(osThread(Kinematics), NULL);//����ѧ����
	
	osThreadDef(MotionControl, MotionControlTask, osPriorityNormal, 0, 512);
  MotionControlHandle = osThreadCreate(osThread(MotionControl), NULL);//�������
	
	osThreadDef(SerialInterface, SerialInterfaceTask, osPriorityNormal, 0, 512);
  SerialInterfaceHandle = osThreadCreate(osThread(SerialInterface), NULL);//����ͨ�ţ���λ����Ϣ
	
	osThreadDef(BMS, BMSTask, osPriorityNormal, 0, 512);
  BMSHandle = osThreadCreate(osThread(BMS), NULL);//��ع���ϵͳ��BMS��
	
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
		//IndicatorDeviceControl(&LED_Red, 2, 100);//��ɫLED���ƺ���
    osDelay(1000);
//		vTaskSuspend(NULL);//���������
  }
}


void task(void)//����ָ�
{
//	BaseType_t YieldRequired;
//	YieldRequired = xTaskResumeFromISR(myTask02Handle);
//	if(YieldRequired == pdTRUE)
//	{
//		portYIELD_FROM_ISR(YieldRequired);//ע��һ�´�ISR���жϺ����вŵ��õģ�Bվ��Ƶ�иú������ڰ����жϺ����е��õ�
//	}			
}

void IndicatorDeviceTask(void const * argument)
{
	taskENTER_CRITICAL();//�����ٽ������ٽ����ڲ��ɱ����
	IndicatorDevice_Init(&Buzzer, GPIOC, GPIO_PIN_13);//��ʾ�豸��ʼ�����ú���
	IndicatorDevice_Init(&LED_Gre, GPIOC, GPIO_PIN_14);//��ʾ�豸��ʼ�����ú���
	IndicatorDevice_Init(&LED_Red, GPIOC, GPIO_PIN_15);//��ʾ�豸��ʼ�����ú���
	taskEXIT_CRITICAL();//�˳��ٽ���
	
	IndicatorDeviceControl(&Buzzer, 2, 50);//���������ƺ���
	IndicatorDeviceControl(&LED_Gre, 2, 50);//��ɫLED���ƺ���
	IndicatorDeviceControl(&LED_Red, 2, 50);//��ɫLED���ƺ���
	
  for(;;)
  {
		osDelay(1);
		IndicatorDeviceLoop(&Buzzer);//ָʾ�豸ѭ������
		IndicatorDeviceLoop(&LED_Gre);//ָʾ�豸ѭ������
		IndicatorDeviceLoop(&LED_Red);//ָʾ�豸ѭ������   
  }
}


void IMUTask(void const * argument)
{
	taskENTER_CRITICAL();//�����ٽ���
	MX_SPI1_Init();//SPI1��ʼ������
	SPI1_CSX_GPIO_Init();//SPI1ƬѡIO��ʼ��
	taskEXIT_CRITICAL();//�˳��ٽ���
	
	while(!ICM42605_Init())//�ȴ���������ʼ���ɹ�
	{
		osDelay(100); 		
		IndicatorDeviceControl(&LED_Red, 1, 100);//��ɫLED���ƺ���
		IndicatorDeviceControl(&Buzzer, 1, 100);//���������ƺ���
	}
	
	for(;;)
	{
		osDelay(1);
		ICM42605_Read_FIFO(&ICM42605);//��ICM42605 FIFOֵ		
	}
}

void KinematicsTask(void const * argument)
{
	taskENTER_CRITICAL();//�����ٽ���
	MX_TIM8_Init();//TIM8-PWM��ʼ������
	MX_TIM2_Init();//TIM2-������ģʽ��ʼ��
	MX_TIM3_Init();//TIM3-������ģʽ��ʼ��
	MX_TIM4_Init();//TIM4-������ģʽ��ʼ��
	MX_TIM5_Init();//TIM5-������ģʽ��ʼ��
	Motor1_GPIO_Init();//�������1 IO��ʼ������
  Motor2_GPIO_Init();//�������2 IO��ʼ������
  Motor3_GPIO_Init();//�������3 IO��ʼ������
  Motor4_GPIO_Init();//�������4 IO��ʼ������
	Encoder_Struct_Init(&Encoder_M1);//��ʼ���������ṹ�����
	Encoder_Struct_Init(&Encoder_M2);//��ʼ���������ṹ�����
	Encoder_Struct_Init(&Encoder_M3);//��ʼ���������ṹ�����
	Encoder_Struct_Init(&Encoder_M4);//��ʼ���������ṹ�����
	LPF_Struct_Init(&LPF_RPM1, 0.05);//��ͨ�˲��ṹ�������ʼ��
	LPF_Struct_Init(&LPF_RPM2, 0.05);//��ͨ�˲��ṹ�������ʼ��
	LPF_Struct_Init(&LPF_RPM3, 0.05);//��ͨ�˲��ṹ�������ʼ��
	LPF_Struct_Init(&LPF_RPM4, 0.05);//��ͨ�˲��ṹ�������ʼ��
//	IncPID_Init(&PID_M1, -3.95, -0.811, -0.62);//����ʽPID������ʼ��
//	IncPID_Init(&PID_M2, -3.95, -0.811, -0.62);//����ʽPID������ʼ��
//	IncPID_Init(&PID_M3, -3.95, -0.811, -0.62);//����ʽPID������ʼ��
//	IncPID_Init(&PID_M4, -3.95, -0.811, -0.62);//����ʽPID������ʼ��

	IncPID_Init(&PID_M1, -2.5f, -0.8f, -0.3f);//����ʽPID������ʼ��
	IncPID_Init(&PID_M2, -2.5f, -0.8f, -0.3f);//����ʽPID������ʼ��
	IncPID_Init(&PID_M3, -2.5f, -0.8f, -0.3f);//����ʽPID������ʼ��
	IncPID_Init(&PID_M4, -2.5f, -0.8f, -0.3f);//����ʽPID������ʼ��
	taskEXIT_CRITICAL();//�˳��ٽ���
	
	uint16_t tim1=0,tim2=0,tim3=0,tim4=0;
	
	for(;;)
	{
		osDelay(5);
		Kinematics_4WD_CalculateRPM(&Kinematics_Inverse);//4WD�˶�ѧ��⺯��
		/*-------�ԡ�Ŀ��ת�١�������ֵ-------*/	
		Kinematics_Inverse.M1_RPM = Constrain(Kinematics_Inverse.M1_RPM, -RPM_MAX, RPM_MAX);
		Kinematics_Inverse.M2_RPM = Constrain(Kinematics_Inverse.M2_RPM, -RPM_MAX, RPM_MAX);
		Kinematics_Inverse.M3_RPM = Constrain(Kinematics_Inverse.M3_RPM, -RPM_MAX, RPM_MAX);
		Kinematics_Inverse.M4_RPM = Constrain(Kinematics_Inverse.M4_RPM, -RPM_MAX, RPM_MAX);
				
		LPF_RPM1.SampleValue = Kinematics_Inverse.M1_RPM;//ͨ������ת��ֵ����ͨ�˲����ṹ���еĲɼ�ֵ
		VelControl.M1_SetRPM = LowPassFilter(&LPF_RPM1); //ת��ֵ������ͨ�˲�������ֵ�����ƽ��			
		LPF_RPM2.SampleValue = Kinematics_Inverse.M2_RPM;//ͨ������ת��ֵ����ͨ�˲����ṹ���еĲɼ�ֵ
		VelControl.M2_SetRPM = LowPassFilter(&LPF_RPM2); //ת��ֵ������ͨ�˲�������ֵ�����ƽ��	
		LPF_RPM3.SampleValue = Kinematics_Inverse.M3_RPM;//ͨ������ת��ֵ����ͨ�˲����ṹ���еĲɼ�ֵ
		VelControl.M3_SetRPM = LowPassFilter(&LPF_RPM3); //ת��ֵ������ͨ�˲�������ֵ�����ƽ��	
		LPF_RPM4.SampleValue = Kinematics_Inverse.M4_RPM;//ͨ������ת��ֵ����ͨ�˲����ṹ���еĲɼ�ֵ
		VelControl.M4_SetRPM = LowPassFilter(&LPF_RPM4); //ת��ֵ������ͨ�˲�������ֵ�����ƽ��		
		
		/*---------------PX4 PWM Control---------------*/
		if(CapturePulse_1.UPdatePulseFlag) 
		{ 
			if(CapturePulse_1.uwIC_DiffCapture > 900 && CapturePulse_1.uwIC_DiffCapture < 2100) tim1 = 10;
			CapturePulse_1.uwIC_DiffCapture = Constrain(CapturePulse_1.uwIC_DiffCapture, 1000, 2000);//Լ����С
			CapturePulse_1.UPdatePulseFlag = 0; 
		}else if(tim1) tim1--;
		
		if(CapturePulse_2.UPdatePulseFlag) 
		{ 
			if(CapturePulse_2.uwIC_DiffCapture > 900 && CapturePulse_2.uwIC_DiffCapture < 2100) tim2 = 10;
			CapturePulse_2.uwIC_DiffCapture = Constrain(CapturePulse_2.uwIC_DiffCapture, 1000, 2000);//Լ����С
			CapturePulse_2.UPdatePulseFlag = 0; 
		}else if(tim2) tim2--;
		
		if(CapturePulse_3.UPdatePulseFlag) 
		{ 
			if(CapturePulse_3.uwIC_DiffCapture > 900 && CapturePulse_3.uwIC_DiffCapture < 2100) tim3 = 10;
			CapturePulse_3.uwIC_DiffCapture = Constrain(CapturePulse_3.uwIC_DiffCapture, 1000, 2000);//Լ����С
			CapturePulse_3.UPdatePulseFlag = 0; 
		}else if(tim3) tim3--;
		
		if(CapturePulse_4.UPdatePulseFlag) 
		{ 
			if(CapturePulse_4.uwIC_DiffCapture > 900 && CapturePulse_4.uwIC_DiffCapture < 2100) tim4 = 10;
			CapturePulse_4.uwIC_DiffCapture = Constrain(CapturePulse_4.uwIC_DiffCapture, 1000, 2000);//Լ����С
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
					Motor1_Reverse();//���1��ת
					Motor3_Reverse();//���3��ת
				}
				else
				{
					Motor1_PWM = Motor3_PWM = (PWM * MotorPWM_Max / 500);
					Motor1_Forward();//���1����
					Motor3_Forward();//���3����
				}
			}else Motor1_PWM = Motor3_PWM = MotorPWM_Min;
			
			if(tim2)
			{
				int32_t PWM = CapturePulse_2.uwIC_DiffCapture - 1500;			
				if(PWM > 0)
				{			
					Motor2_PWM = Motor4_PWM = -(PWM * MotorPWM_Max / 500);
					Motor2_Reverse();//���2��ת
					Motor4_Reverse();//���4��ת					
				}
				else
				{
					Motor2_PWM = Motor4_PWM = (PWM * MotorPWM_Max / 500);
					Motor2_Forward();//���2����
					Motor4_Forward();//���4����			
				}
			}else Motor2_PWM = Motor4_PWM = MotorPWM_Min;
		}
		else 
		{
			/*----------------------��ת����----------------------*/
			static uint16_t M1_StallTime = 0;//M1�����תʱ��
			static uint16_t M2_StallTime = 0;//M1�����תʱ��
			static uint16_t M3_StallTime = 0;//M1�����תʱ��
			static uint16_t M4_StallTime = 0;//M1�����תʱ��
			static uint8_t Mx_StallFlag = 0; //�����ת��־λ

			if(fabs(VelControl.M1_SetRPM) > 10)//�������ת�ٲ����ñ�������ת���±�������ֵ���ȶ�
			{
				if(fabs(VelControl.M1_SetRPM - VelControl.M1_RPM) > fabs(VelControl.M1_SetRPM / 2.0)) M1_StallTime++;
				else if(M1_StallTime) M1_StallTime--;
			}else if(M1_StallTime) M1_StallTime--;
			
			if(fabs(VelControl.M2_SetRPM) > 10)//�������ת�ٲ����ñ�������ת���±�������ֵ���ȶ�
			{
				if(fabs(VelControl.M2_SetRPM - VelControl.M2_RPM) > fabs(VelControl.M2_SetRPM / 2.0)) M2_StallTime++;
				else if(M2_StallTime) M2_StallTime--;
			}else if(M2_StallTime) M2_StallTime--;
			
			if(fabs(VelControl.M3_SetRPM) > 10)//�������ת�ٲ����ñ�������ת���±�������ֵ���ȶ�
			{
				if(fabs(VelControl.M3_SetRPM - VelControl.M3_RPM) > fabs(VelControl.M3_SetRPM / 2.0)) M3_StallTime++;
				else if(M3_StallTime) M3_StallTime--;
			}else if(M3_StallTime) M3_StallTime--;
			
			if(fabs(VelControl.M4_SetRPM) > 10)//�������ת�ٲ����ñ�������ת���±�������ֵ���ȶ�
			{
				if(fabs(VelControl.M4_SetRPM - VelControl.M4_RPM) > fabs(VelControl.M4_SetRPM / 2.0)) M4_StallTime++;
				else if(M4_StallTime) M4_StallTime--;
			}else if(M4_StallTime) M4_StallTime--;
			
			if((M1_StallTime > 600) || (M2_StallTime > 600) || (M3_StallTime > 600) || (M4_StallTime > 600) || Mx_StallFlag)//������ת����
			{
				VelControl.M1_SetRPM = VelControl.M2_SetRPM = VelControl.M3_SetRPM = VelControl.M4_SetRPM = 0;
				Mx_StallFlag = 1;
				IndicatorDeviceControl(&Buzzer, 1, 500);//���������ƺ���
			}
			
			VelocityControl_4WD(&VelControl);//4WD�ٶȱջ����ƺ���
		}
		/*---------------------------------------------*/		
			
		/*----------------------���嵱ǰ���ٶȽ��ٶȲ����˲�----------------------*/
		static uint8_t i = 0;
		i++;
		Kinematics_Forward.M1_RPM += VelControl.M1_RPM;
		Kinematics_Forward.M2_RPM += VelControl.M2_RPM;
		Kinematics_Forward.M3_RPM += VelControl.M3_RPM;
		Kinematics_Forward.M4_RPM += VelControl.M4_RPM;
		
		if(i > 1) 
		{
			Kinematics_Forward.M1_RPM = (float)Kinematics_Forward.M1_RPM / i;//����ȡƽ��ֵ
			Kinematics_Forward.M2_RPM = (float)Kinematics_Forward.M2_RPM / i;
			Kinematics_Forward.M3_RPM = (float)Kinematics_Forward.M3_RPM / i;
			Kinematics_Forward.M4_RPM = (float)Kinematics_Forward.M4_RPM / i;
			Kinematics_4WD_GetVelocities(&Kinematics_Forward);//4WD�˶�ѧ���⺯��
			i = Kinematics_Forward.M1_RPM = Kinematics_Forward.M2_RPM = Kinematics_Forward.M3_RPM = Kinematics_Forward.M4_RPM = 0;//��0
		}	
	}
}

void MotionControlTask(void const * argument)
{
	taskENTER_CRITICAL();//�����ٽ���
	MX_UART4_Init();//UART4��ʼ������
	RC_Init(&RC);
	taskEXIT_CRITICAL();//�˳��ٽ���
	
	for(;;)
	{
		osDelay(10);
		if(RC.LifeTime) RC.LifeTime--;

		if(RC.LifeTime && RC.Switch==RC_DOWN)//�ж�ң��������ջ��Ƿ�������ͨ��״̬
		{		
			SystemState = 0x05;//ң��״̬
			IndicatorDeviceControl(&LED_Gre, 1, 50);//��ɫLED���ƺ���	
			LPF_RPM1.a = LPF_RPM2.a = LPF_RPM3.a = LPF_RPM4.a = 0.05f;
			Kinematics_Inverse.Linear_X = RC.Left_Y * 1.5f;//���Yҡ�˿��Ƴ���X�����ٶ�
			//Kinematics_Inverse.Linear_Y = RC.Left_X * 1.5f;//���Xҡ�˿��Ƴ���Y�����ٶ�
			Kinematics_Inverse.Angular_Z = RC.Right_X * 2.0f;//�Ҳ�Xҡ�˿��Ƴ���Z����ٶ�	
			DataCom_RX.Linear_X = DataCom_RX.Linear_Y = DataCom_RX.Angular_Z = 0;	
			DataCom_RX.HeartbeatTime = 0;//������ʱ��
		}
		else if(DataCom_RX.HeartbeatTime)//�����������ά������
		{
			SystemState = 0x06;//�߿�״̬
			LPF_RPM1.a = LPF_RPM2.a = LPF_RPM3.a = LPF_RPM4.a = 1.0f;
			Kinematics_Inverse.Linear_X = DataCom_RX.Linear_X;
			//Kinematics_Inverse.Linear_Y = DataCom_RX.Linear_Y;
			Kinematics_Inverse.Angular_Z = DataCom_RX.Angular_Z;
			DataCom_RX.HeartbeatTime--;
		}
		else//������ʱɲ��
		{
			SystemState = 0x00;//����״̬
			LPF_RPM1.a = LPF_RPM2.a = LPF_RPM3.a = LPF_RPM4.a = 0.05f;
			DataCom_RX.Linear_X = DataCom_RX.Linear_Y = DataCom_RX.Angular_Z = 0;									
			Kinematics_Inverse.Linear_X = 0;//���Ƴ������ٶ� m/s
			Kinematics_Inverse.Linear_Y = 0;//���Ƴ������ٶ� m/s
			Kinematics_Inverse.Angular_Z = 0;//���Ƴ�����ٶ� rad/s			
		}		
	}
}

void SerialInterfaceTask(void const * argument)
{
	taskENTER_CRITICAL();//�����ٽ���	
	MX_USART2_Init();//USART2��ʼ������
	MastMessage_Queue = xQueueCreate(100, sizeof(uint8_t));//����������Ϣ���У����������100��uint8_t��С
	ParseData_Init(&ParseData_Mast, 30);//�������ݲ�����ʼ��
	taskEXIT_CRITICAL();//�˳��ٽ���
	
	for(;;)
	{		
		osDelay(1);	
		if(MastMessage_Queue != NULL)//��Ϣ����MastMessage_Queue�����ɹ�
		{
			while(!ParseData_Mast.Flag)//�жϽ������������Ƿ��Ѵ���
			{
				uint8_t Data;
				if(xQueueReceive(MastMessage_Queue, &Data, 0) == pdTRUE)//����������Ϣ�����гɹ���ȡ��������ִ��,0������ʾ���ȴ�
				{
					ParseDataFunction(&ParseData_Mast, Data);//��������
				}
			}
		}
		
		if(HAL_UART_GetState(&huart2) != HAL_UART_STATE_BUSY_TX)//���⴮�ڷ���δ�����ִ�д��ڷ���
		{				
			if(ParseData_Mast.Flag)//��������һ֡���ݣ������
			{
				IndicatorDeviceControl(&LED_Gre, 1, 10);//��ɫLED���ƺ���				
				DataCom_RX_Function(&DataCom_RX, (uint8_t*)ParseData_Mast.Buff);//���ݽ��սӿ�
				ParseData_Mast.Flag = 0;//���ݽ���״̬��־λ��0
			}			
		}
	}
}

void BMSTask(void const * argument)
{
	taskENTER_CRITICAL();//�����ٽ���	
	MX_ADC1_Init();//ADC1��ʼ������
	Battery_Init(&Battery, 0.3f);//���״̬��ʼ������
	taskEXIT_CRITICAL();//�˳��ٽ���
	
	for(;;)
	{
		osDelay(500);//��ʱ��������ŷ��棬����Battery.Voltage����nan
		UpdateBatteryInfor(&Battery, (uint16_t*)ADC1_DMA_Buff);//���µ��״̬��Ϣ		
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
