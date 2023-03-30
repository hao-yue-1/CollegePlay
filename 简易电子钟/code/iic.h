#ifndef _IIC_H
#define _IIC_H

#include <reg51.h>

sbit SCL=P2^6;
sbit SDA=P2^7;

void IIC_Delay();		//����4us��4.7us����ʱ
void IIC_Init();		//�����������ߣ������ʼ״̬���ȴ���ʼ�ź�
void IIC_Start();		//��ʼ�źţ�SCLΪ��ʱ��SDA�ɸ߱��
void IIC_Stop();			//�����źţ�SCLΪ��ʱ��SDA�ɵͱ��
void IIC_ACK();			//Ӧ���ź�
void IIC_Write_Byte(unsigned char j);		//дһ���ֽڣ��Ӹ�λ����λ����
unsigned char IIC_Read_Byte();		//��һ���ֽڣ��ӵ�λ����λ��

#endif