#ifndef __TIM2_H
#define __TIM2_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
	 
extern TIM_HandleTypeDef htim2;
void MX_TIM2_Init(void);//TIM2-编码器模式初始化

#ifdef __cplusplus
}
#endif

#endif
