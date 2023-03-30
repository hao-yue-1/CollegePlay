#include "LCD12864.h"
#include "usart.h"
#include <reg51.h>
#include <intrins.h>

#define LCD_databus P0 //总线接口，注意51的P0口需要外接上拉电阻
sbit DI = P2^2;	//DI为 0：写指令或读状态，1：数据
sbit RW = P2^3;	//1:写 0：读
sbit EN = P2^4;	//使能
sbit CS1 = P2^0;	//片选1.低电平有效，控制左半屏
sbit CS2 = P2^1;  //片选2，低电平有效，控制右半屏
 
void Read_busy()
{
	P0 = 0X00;
	DI = 0;
	RW = 1;
	EN = 1;
	while(P0 & 0x80)
	{;}
	EN = 0;
}

void write_LCD_command(uint8 value)	//写命令函数
{
	Read_busy();
	DI = 0; //0:写指令
	RW = 0;	//0:写操作
	LCD_databus = value;
	EN = 1;	//EN下降沿锁存有效数据
	_nop_();
	_nop_();
	_nop_();//空指令，短暂延时
	EN = 0;
}

void write_LCD_data(uint8 value)	//写数据函数
{
	Read_busy();
	DI = 1; //1:写数据
	RW = 0;	//写操作
	LCD_databus = value;
	EN = 1; //EN下降沿锁存有效数据
	_nop_();
	_nop_();
	_nop_();//空指令，短暂延时
	EN = 0;
}

void Set_page(uint8 page)	//设置显示起始页（一页是8*8的方框）
{
	page = 0xB8 | page;	//页的首地址为0xB8
	write_LCD_command(page);
}

void Set_line(uint8 startline)	//设置显示起始行
{
	startline = 0xC0 | startline;
	write_LCD_command(startline);
}

void Set_column(uint8 column)	//设置显示的列
{
	column = column & 0x3F;	//列的最大值为64
	column = column | 0x40;	//列的首地址为0x40
	write_LCD_command(column);
}	

void SetOnOff(uint8 onoff)	//显示开关函数，0x3E:关 0x3F:开
{
	onoff = 0x3E | onoff;	//onoff为0时关显示，为1时开显示
	write_LCD_command(onoff);
}

void SelectScreen(uint8 screen)	//选择屏幕
{
	switch(screen)
	{
		case 0:CS1 = 0;CS2 = 0;break;//全屏
		case 1:CS1 = 0;CS2 = 1;break;//左半屏
		case 2:CS1 = 1;CS2 = 0;break;//右半屏
		default:break;
	}
}

void ClearScreen(uint8 screen)	//清屏
{
	uint8 i,j;
	SelectScreen(screen);
	for(i=0;i<8;i++)
	{
		Set_page(i);
		Set_column(0);
		for(j=0;j<64;j++)
		{
			write_LCD_data(0x00);	//写入0，地址指针自动加1
		}
	}
}

void InitLCD()
{
	Read_busy();
	SelectScreen(0);
	SetOnOff(0);
	SelectScreen(0);
	SetOnOff(1);
	SelectScreen(0);
	ClearScreen(0);
	Set_line(0);
}

//写16*16的汉字
void show_ch(uint8 screen,uint8 page,uint8 column,uint8 *p)
{
	uint8 i;
	SelectScreen(screen);	//选择屏幕
	Set_page(page);			//选择起始页
	Set_column(column);		//选择起始行
	
	for(i=0;i<16;i++)	//采用16*16的字模
	{
		write_LCD_data(p[i]);
	}
	
	Set_page(page+1);
	Set_column(column);
	for(i=0;i<16;i++)	//采用16*16的字模,"小四号字"
	{
		write_LCD_data(p[i+16]);
	}
}

////写12*12的汉字
//void show_ch12(uint8 screen,uint8 page,uint8 column,uint8 *p)
//{
//	uint8 i;
//	SelectScreen(screen);	//选择屏幕
//	Set_page(page);			//选择起始页
//	Set_column(column);		//选择起始行
//	
//	for(i=0;i<12;i++)	//采用16*16的字模
//	{
//		write_LCD_data(p[i]);
//	}
//	
//	Set_page(page+1);
//	Set_column(column);
//	for(i=0;i<12;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+12]);
//	}
//}

////写32*32的图片
//void show_im(uint8 screen,uint8 page,uint8 column,uint8 *p)
//{
//	uint8 i;
//	SelectScreen(screen);
//	Set_page(page);
//	Set_column(column);

//	for(i=0;i<32;i++)	//采用16*16的字模
//	{
//		write_LCD_data(p[i]);
//	}
//	
//	Set_page(page+1);
//	Set_column(column);
//	for(i=0;i<32;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+32]);
//	}
//	
//	Set_page(page+2);
//	Set_column(column);
//	for(i=0;i<32;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+64]);
//	}
//	
//	Set_page(page+3);
//	Set_column(column);
//	for(i=0;i<32;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+96]);
//	}
//}

////写64*64的图片
//void show_im64(uint8 screen,uint8 page,uint8 column,uint8 *p)
//{
//	uint8 i;
//	SelectScreen(screen);
//	Set_page(page);
//	Set_column(column);

//	for(i=0;i<64;i++)	//采用16*16的字模
//	{
//		write_LCD_data(p[i]);
//	}
//	
//	Set_page(page+1);
//	Set_column(column);
//	for(i=0;i<64;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+64]);
//	}
//	
//	Set_page(page+2);
//	Set_column(column);
//	for(i=0;i<64;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+128]);
//	}
//	
//	Set_page(page+3);
//	Set_column(column);
//	for(i=0;i<64;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+192]);
//	}
//	
//	Set_page(page+4);
//	Set_column(column);
//	for(i=0;i<64;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+256]);
//	}
//	
//	Set_page(page+5);
//	Set_column(column);
//	for(i=0;i<64;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+320]);
//	}
//	
//	Set_page(page+6);
//	Set_column(column);
//	for(i=0;i<64;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+384]);
//	}
//	
//	Set_page(page+7);
//	Set_column(column);
//	for(i=0;i<64;i++)	//采用16*16的字模,"小四号字"
//	{
//		write_LCD_data(p[i+448]);
//	}
//}