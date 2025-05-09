#include "delayus.h"
/*---------------------------------------------us��ʱ����---------------------------------------------*/
//delayus(x)ʵ���ӳ�(x+1)΢��
#define CPU_FREQUENCY_MHZ    168

static __IO uint32_t TimingDelay;

void delay_us(__IO uint32_t nTime)//us��ʱ����
{
    int old_val,new_val,val;

    if(nTime > 900)
    {
        for(old_val = 0; old_val < nTime/900; old_val++)
        {
            delay_us(900);
        }
        nTime = nTime%900;
    }

    old_val = SysTick->VAL;
    new_val = old_val - CPU_FREQUENCY_MHZ*nTime;
    if(new_val >= 0)
    {
        do
        {
            val = SysTick->VAL;
        }
        while((val < old_val)&&(val >= new_val));
    }
    else
    {
        new_val +=CPU_FREQUENCY_MHZ*1000;
        do
        {
            val = SysTick->VAL;
        }
        while((val <= old_val)||(val > new_val));

    }
}
