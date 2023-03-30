#ifndef _IIC_H
#define _IIC_H

#include <reg51.h>

sbit SCL=P2^6;
sbit SDA=P2^7;

void IIC_Delay();		//大于4us或4.7us的延时
void IIC_Init();		//将两根线拉高，进入初始状态，等待开始信号
void IIC_Start();		//起始信号，SCL为高时，SDA由高变低
void IIC_Stop();			//结束信号，SCL为高时，SDA由低变高
void IIC_ACK();			//应答信号
void IIC_Write_Byte(unsigned char j);		//写一个字节，从高位到低位发送
unsigned char IIC_Read_Byte();		//读一个字节，从低位到高位读

#endif