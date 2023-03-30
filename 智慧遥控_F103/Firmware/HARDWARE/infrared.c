#include "infrared.h"
//#include "sys.h"
#include "timer.h"
#include "delay.h"

//发送引导码
void Lead_Code()
{
	TIM_SetTIM3Compare2(189); //接收器拉低
	delay_us(4400);
	TIM_SetTIM3Compare2(0); //接收器拉高
	delay_us(4400);
}

//发送分隔码
void Stop_Code()
{
	TIM_SetTIM3Compare2(189); //接收器拉低
	delay_us(540);
	TIM_SetTIM3Compare2(0); //接收器拉高
	delay_us(5220);
}

//发送0数据
void Send_0_Code()
{
	TIM_SetTIM3Compare2(189); //接收器拉低
	delay_us(540);
	TIM_SetTIM3Compare2(0); //接收器拉高
	delay_us(540);
}

//发送1数据
void Send_1_Code()
{
	TIM_SetTIM3Compare2(189); //接收器拉低
	delay_us(540);
	TIM_SetTIM3Compare2(0); //接收器拉高
	delay_us(1620);
}

//发送一个空调编码
void Send_Byte(u8 data)
{
	int i;
	for(i=7;i>=0;i--)
	{
		if(data & (1<<i))
		{
			Send_1_Code();
		}
		else
		{
			Send_0_Code();
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: Normal_Code
*	功能说明: 发送空调编码，控制空调的模式
*	形    参: 1. A：码A（识别码A=10110010=0xB2，预留方案时A=10110111=0xB7）
			  2. B：码B（前三位控制风速，后五位固定为11111）
			  3. C：码C（前四位控制温度，接着两位控制模式，后两位固定为00）
*	返 回 值: 无
*********************************************************************************************************
*/
void Normal_Code(u8 A, u8 B, u8 C)
{
	Lead_Code();
	Send_Byte(A);
	Send_Byte(~A);
	Send_Byte(B);
	Send_Byte(~B);
	Send_Byte(C);
	Send_Byte(~C);
	Stop_Code();
	Lead_Code();
	Send_Byte(A);
	Send_Byte(~A);
	Send_Byte(B);
	Send_Byte(~B);
	Send_Byte(C);
	Send_Byte(~C);
	Stop_Code();
}


