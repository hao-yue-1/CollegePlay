#include <reg51.h>
#include <intrins.h>
#include "iic.h"

void IIC_Delay()		//����4us��4.7us����ʱ
{
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
}

void IIC_Init()			//�����������ߣ������ʼ״̬���ȴ���ʼ�ź�
{
	SDA = 1;
	SCL = 1;
}

void IIC_Start()		//��ʼ�źţ�SCLΪ��ʱ��SDA�ɸ߱��
{
	SCL = 1;
	SDA = 1;
	IIC_Delay();
	SDA = 0;
	IIC_Delay();
	SCL = 0;
	IIC_Delay();
}

void IIC_Stop()			//�����źţ�SCLΪ��ʱ��SDA�ɵͱ��
{
	SCL = 0;
	SDA = 0;
	IIC_Delay();
	SCL = 1;
	IIC_Delay();
	SDA = 1;
	IIC_Delay();
}

void IIC_ACK()			//Ӧ���ź�
{
   unsigned char i=0;
   SCL = 1;			//SCLΪ��ʱ������Ӧ��
   IIC_Delay();
   while((SDA==1)&&(i<255))		//����һ��ʱ��SDA��Ϊ�ߣ����Ƿ�Ӧ���źţ�����SDA=0,����Ӧ���ź�
   {  
       i++;
   }
   SCL = 0;			//Ӧ�������ʱ���ߣ��ȴ������ź�
   IIC_Delay();
}

void IIC_Write_Byte(unsigned char j)		//дһ���ֽڣ��Ӹ�λ����λ����
{
	unsigned char i,temp;
	temp = j;
	SCL = 0;		//����ֻ��SCLΪ��ʱ����ı�
	IIC_Delay();
	for(i=0;i<8;i++)
	{
		temp <<= 1;		//temp = temp << 1,�Ӹߵ��������Ƴ�
		SDA = CY;		//CYΪPSW״̬�Ĵ����е����λ��1000 0000<<1,CY=1;0000 0001>>1,CY=1.CY�����Ƴ�������һλ
		IIC_Delay();
		SCL = 1;		//�ȴ�SDA�����ȶ�
		IIC_Delay();
		SCL = 0;		//�ȴ���һ�η���
		IIC_Delay();
	}
	SDA = 1;	
	IIC_Delay();
}

unsigned char IIC_Read_Byte()		//��һ���ֽڣ��ӵ�λ����λ��
{
	unsigned char i,j,k=0;
	SCL = 0;		//����ʱ���ߣ��ȴ����ݱ仯
	IIC_Delay();
	for(i=0;i<8;i++)
	{
		SCL = 1;		//�����ʱ���Ѿ���ȡ����SDA��һλ����
		IIC_Delay();
		if(SDA == 1)
			j = 1;
		else 
			j = 0;
		k = (k<<1) | j;			//k���������ƣ����λÿ�ν����µ�SDA �仯
		SCL = 0;		//�ȴ���һ�����ݱ仯
	}
	return(k);		//���ض������ֽ�����
}