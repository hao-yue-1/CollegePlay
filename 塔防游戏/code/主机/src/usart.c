#include <reg51.h>
#include "usart.h"

void init_USART_send()
{
	TMOD=0x20;                         //定时器T1工作于方式2（可自动重装的8位定时器）
	SCON=0x40;                         //串口工作方式1，不允许接收
	PCON=0x00;                         //波特率不倍增
	TH1=0xf4;                          //波特率为2400b/s
	TL1=0xf4;
	TR1=1;                             //启动定时器T1（T1作波特率的发生器）
}

void send(unsigned char dat)            //数据发送程序
{
   	SBUF=dat;                           //将要发送的数据存入发送缓冲器中
	while(TI==0);                       //若发送中断标志位没有置1(正在发送数据)，就等待
	TI=0;                               //若发送完成，TI自动置1，这里把它清零
}
