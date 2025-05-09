#ifndef __RC_H
#define __RC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

enum RC_Enum
{
	RC_UP=0,		//��ť���ز���
	RC_Middle,	//��ť���ز��м�
	RC_DOWN			//��ť���ز���
};

typedef struct
{		
	__IO float Left_X; //���ҡ��Xֵ����ֵ��Χ -1.0f ~ 1.0f
	__IO float Left_Y; //���ҡ��Yֵ����ֵ��Χ -1.0f ~ 1.0f
	__IO float Right_X;//�Ҳ�ҡ��Xֵ����ֵ��Χ -1.0f ~ 1.0f
	__IO float Right_Y;//�Ҳ�ҡ��Yֵ����ֵ��Χ -1.0f ~ 1.0f
	__IO uint8_t Switch;	 //������ť���أ�[RC_UP������]��[RC_Middle�����м�]��[RC_DOWN������]
	__IO float Knob;	 	 //��ť0.0f~1.0f
	__IO uint16_t CH[11];//�����ͨ��ԭʼ����
	__IO int8_t CH_int8_t[11];//�����ͨ��ת����int8_t����ֵ
	__IO uint16_t LifeTime;
}RC_Struct;//RCң�����ṹ�����


void RC_Init(RC_Struct *RC);
void RC_Update(RC_Struct *RC, uint8_t *Data);//ң�������º���

#ifdef __cplusplus
}
#endif

#endif
