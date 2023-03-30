#include "infrared.h"
//#include "sys.h"
#include "timer.h"
#include "delay.h"

//����������
void Lead_Code()
{
	TIM_SetTIM3Compare2(189); //����������
	delay_us(4400);
	TIM_SetTIM3Compare2(0); //����������
	delay_us(4400);
}

//���ͷָ���
void Stop_Code()
{
	TIM_SetTIM3Compare2(189); //����������
	delay_us(540);
	TIM_SetTIM3Compare2(0); //����������
	delay_us(5220);
}

//����0����
void Send_0_Code()
{
	TIM_SetTIM3Compare2(189); //����������
	delay_us(540);
	TIM_SetTIM3Compare2(0); //����������
	delay_us(540);
}

//����1����
void Send_1_Code()
{
	TIM_SetTIM3Compare2(189); //����������
	delay_us(540);
	TIM_SetTIM3Compare2(0); //����������
	delay_us(1620);
}

//����һ���յ�����
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
*	�� �� ��: Normal_Code
*	����˵��: ���Ϳյ����룬���ƿյ���ģʽ
*	��    ��: 1. A����A��ʶ����A=10110010=0xB2��Ԥ������ʱA=10110111=0xB7��
			  2. B����B��ǰ��λ���Ʒ��٣�����λ�̶�Ϊ11111��
			  3. C����C��ǰ��λ�����¶ȣ�������λ����ģʽ������λ�̶�Ϊ00��
*	�� �� ֵ: ��
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


