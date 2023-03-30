#include <reg51.h>
#include "usart.h"
#include "select.h"

void init_USART_receive()
{
	TMOD=0x20;                     //设置定时器T1工作于方式2
	SCON=0x50;                     //串口工作方式1，允许接收数据
	PCON=0x00;                     //波特率不倍增
	TH1=0xf4;                      //波特率为2400b/s
	TL1=0xf4;
	TR1=1;                         //启动定时器T1
	REN=1;                         //允许接收数据
}

unsigned char receive()         //数据接收函数
{
	unsigned char dat;
	
	while(RI==0);                   //数据没有接收完毕时等待接收
	RI=0;                           //接收完毕时清零
	dat=SBUF;                       //将接收缓冲器中的数据存入dat
	return dat;                     //将接收到的数据返回
}