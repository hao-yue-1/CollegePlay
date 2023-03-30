#include "LCD12864.h"
#include "usart.h"
#include <reg51.h>
#include <intrins.h>

#define LCD_databus P0 //���߽ӿڣ�ע��51��P0����Ҫ�����������
sbit DI = P2^2;	//DIΪ 0��дָ����״̬��1������
sbit RW = P2^3;	//1:д 0����
sbit EN = P2^4;	//ʹ��
sbit CS1 = P2^0;	//Ƭѡ1.�͵�ƽ��Ч�����������
sbit CS2 = P2^1;  //Ƭѡ2���͵�ƽ��Ч�������Ұ���
 
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

void write_LCD_command(uint8 value)	//д�����
{
	Read_busy();
	DI = 0; //0:дָ��
	RW = 0;	//0:д����
	LCD_databus = value;
	EN = 1;	//EN�½���������Ч����
	_nop_();
	_nop_();
	_nop_();//��ָ�������ʱ
	EN = 0;
}

void write_LCD_data(uint8 value)	//д���ݺ���
{
	Read_busy();
	DI = 1; //1:д����
	RW = 0;	//д����
	LCD_databus = value;
	EN = 1; //EN�½���������Ч����
	_nop_();
	_nop_();
	_nop_();//��ָ�������ʱ
	EN = 0;
}

void Set_page(uint8 page)	//������ʾ��ʼҳ��һҳ��8*8�ķ���
{
	page = 0xB8 | page;	//ҳ���׵�ַΪ0xB8
	write_LCD_command(page);
}

void Set_line(uint8 startline)	//������ʾ��ʼ��
{
	startline = 0xC0 | startline;
	write_LCD_command(startline);
}

void Set_column(uint8 column)	//������ʾ����
{
	column = column & 0x3F;	//�е����ֵΪ64
	column = column | 0x40;	//�е��׵�ַΪ0x40
	write_LCD_command(column);
}	

void SetOnOff(uint8 onoff)	//��ʾ���غ�����0x3E:�� 0x3F:��
{
	onoff = 0x3E | onoff;	//onoffΪ0ʱ����ʾ��Ϊ1ʱ����ʾ
	write_LCD_command(onoff);
}

void SelectScreen(uint8 screen)	//ѡ����Ļ
{
	switch(screen)
	{
		case 0:CS1 = 0;CS2 = 0;break;//ȫ��
		case 1:CS1 = 0;CS2 = 1;break;//�����
		case 2:CS1 = 1;CS2 = 0;break;//�Ұ���
		default:break;
	}
}

void ClearScreen(uint8 screen)	//����
{
	uint8 i,j;
	SelectScreen(screen);
	for(i=0;i<8;i++)
	{
		Set_page(i);
		Set_column(0);
		for(j=0;j<64;j++)
		{
			write_LCD_data(0x00);	//д��0����ַָ���Զ���1
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

//д16*16�ĺ���
void show_ch(uint8 screen,uint8 page,uint8 column,uint8 *p)
{
	uint8 i;
	SelectScreen(screen);	//ѡ����Ļ
	Set_page(page);			//ѡ����ʼҳ
	Set_column(column);		//ѡ����ʼ��
	
	for(i=0;i<16;i++)	//����16*16����ģ
	{
		write_LCD_data(p[i]);
	}
	
	Set_page(page+1);
	Set_column(column);
	for(i=0;i<16;i++)	//����16*16����ģ,"С�ĺ���"
	{
		write_LCD_data(p[i+16]);
	}
}

////д12*12�ĺ���
//void show_ch12(uint8 screen,uint8 page,uint8 column,uint8 *p)
//{
//	uint8 i;
//	SelectScreen(screen);	//ѡ����Ļ
//	Set_page(page);			//ѡ����ʼҳ
//	Set_column(column);		//ѡ����ʼ��
//	
//	for(i=0;i<12;i++)	//����16*16����ģ
//	{
//		write_LCD_data(p[i]);
//	}
//	
//	Set_page(page+1);
//	Set_column(column);
//	for(i=0;i<12;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+12]);
//	}
//}

////д32*32��ͼƬ
//void show_im(uint8 screen,uint8 page,uint8 column,uint8 *p)
//{
//	uint8 i;
//	SelectScreen(screen);
//	Set_page(page);
//	Set_column(column);

//	for(i=0;i<32;i++)	//����16*16����ģ
//	{
//		write_LCD_data(p[i]);
//	}
//	
//	Set_page(page+1);
//	Set_column(column);
//	for(i=0;i<32;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+32]);
//	}
//	
//	Set_page(page+2);
//	Set_column(column);
//	for(i=0;i<32;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+64]);
//	}
//	
//	Set_page(page+3);
//	Set_column(column);
//	for(i=0;i<32;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+96]);
//	}
//}

////д64*64��ͼƬ
//void show_im64(uint8 screen,uint8 page,uint8 column,uint8 *p)
//{
//	uint8 i;
//	SelectScreen(screen);
//	Set_page(page);
//	Set_column(column);

//	for(i=0;i<64;i++)	//����16*16����ģ
//	{
//		write_LCD_data(p[i]);
//	}
//	
//	Set_page(page+1);
//	Set_column(column);
//	for(i=0;i<64;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+64]);
//	}
//	
//	Set_page(page+2);
//	Set_column(column);
//	for(i=0;i<64;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+128]);
//	}
//	
//	Set_page(page+3);
//	Set_column(column);
//	for(i=0;i<64;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+192]);
//	}
//	
//	Set_page(page+4);
//	Set_column(column);
//	for(i=0;i<64;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+256]);
//	}
//	
//	Set_page(page+5);
//	Set_column(column);
//	for(i=0;i<64;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+320]);
//	}
//	
//	Set_page(page+6);
//	Set_column(column);
//	for(i=0;i<64;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+384]);
//	}
//	
//	Set_page(page+7);
//	Set_column(column);
//	for(i=0;i<64;i++)	//����16*16����ģ,"С�ĺ���"
//	{
//		write_LCD_data(p[i+448]);
//	}
//}