#include <reg51.h>
#include <intrins.h>
#include "iic.h"

void IIC_Delay()		//大于4us或4.7us的延时
{
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
}

void IIC_Init()			//将两根线拉高，进入初始状态，等待开始信号
{
	SDA = 1;
	SCL = 1;
}

void IIC_Start()		//起始信号，SCL为高时，SDA由高变低
{
	SCL = 1;
	SDA = 1;
	IIC_Delay();
	SDA = 0;
	IIC_Delay();
	SCL = 0;
	IIC_Delay();
}

void IIC_Stop()			//结束信号，SCL为高时，SDA由低变高
{
	SCL = 0;
	SDA = 0;
	IIC_Delay();
	SCL = 1;
	IIC_Delay();
	SDA = 1;
	IIC_Delay();
}

void IIC_ACK()			//应答信号
{
   unsigned char i=0;
   SCL = 1;			//SCL为高时，接收应答
   IIC_Delay();
   while((SDA==1)&&(i<255))		//过了一段时间SDA仍为高，就是非应答信号；否则，SDA=0,返回应答信号
   {  
       i++;
   }
   SCL = 0;			//应答后拉低时钟线，等待结束信号
   IIC_Delay();
}

void IIC_Write_Byte(unsigned char j)		//写一个字节，从高位到低位发送
{
	unsigned char i,temp;
	temp = j;
	SCL = 0;		//数据只在SCL为低时允许改变
	IIC_Delay();
	for(i=0;i<8;i++)
	{
		temp <<= 1;		//temp = temp << 1,从高到低依次移出
		SDA = CY;		//CY为PSW状态寄存器中的最高位，1000 0000<<1,CY=1;0000 0001>>1,CY=1.CY就是移出来的那一位
		IIC_Delay();
		SCL = 1;		//等待SDA数据稳定
		IIC_Delay();
		SCL = 0;		//等待下一次发送
		IIC_Delay();
	}
	SDA = 1;	
	IIC_Delay();
}

unsigned char IIC_Read_Byte()		//读一个字节，从低位到高位读
{
	unsigned char i,j,k=0;
	SCL = 0;		//拉低时钟线，等待数据变化
	IIC_Delay();
	for(i=0;i<8;i++)
	{
		SCL = 1;		//在这个时刻已经获取到了SDA的一位数据
		IIC_Delay();
		if(SDA == 1)
			j = 1;
		else 
			j = 0;
		k = (k<<1) | j;			//k依次往右移，最低位每次接受新的SDA 变化
		SCL = 0;		//等待下一次数据变化
	}
	return(k);		//返回读到的字节数据
}