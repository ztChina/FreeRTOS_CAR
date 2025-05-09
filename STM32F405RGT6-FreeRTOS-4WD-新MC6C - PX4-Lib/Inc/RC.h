#ifndef __RC_H
#define __RC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

enum RC_Enum
{
	RC_UP=0,		//拨钮开关拨上
	RC_Middle,	//拨钮开关拨中间
	RC_DOWN			//拨钮开关拨下
};

typedef struct
{		
	__IO float Left_X; //左侧摇杆X值，数值范围 -1.0f ~ 1.0f
	__IO float Left_Y; //左侧摇杆Y值，数值范围 -1.0f ~ 1.0f
	__IO float Right_X;//右侧摇杆X值，数值范围 -1.0f ~ 1.0f
	__IO float Right_Y;//右侧摇杆Y值，数值范围 -1.0f ~ 1.0f
	__IO uint8_t Switch;	 //三档拨钮开关，[RC_UP：拨上]、[RC_Middle：拨中间]、[RC_DOWN：拨下]
	__IO float Knob;	 	 //旋钮0.0f~1.0f
	__IO uint16_t CH[11];//缓存各通道原始数据
	__IO int8_t CH_int8_t[11];//缓存各通道转换成int8_t的数值
	__IO uint16_t LifeTime;
}RC_Struct;//RC遥控器结构体参数


void RC_Init(RC_Struct *RC);
void RC_Update(RC_Struct *RC, uint8_t *Data);//遥控器更新函数

#ifdef __cplusplus
}
#endif

#endif
