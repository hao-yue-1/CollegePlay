#include "LCD12864.h"
#include "menu.h"
#include "select.h"
#include "eeprom.h"

uint8 score[6]={0,0,0,0,0,0};

//欢迎界面
void welcome()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xF0,0xF0,0x80,0x80,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x0F,0x0F,0x01,0x01,0x01,0x00,0x00,0x00,0x00,/*"游戏光标",0*/
		
		0x80,0x80,0xC0,0xC0,0xE0,0xA0,0x90,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
		0x00,0x00,0x01,0x01,0x03,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"←",1*/
		
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x90,0xA0,0xE0,0xC0,0xC0,0x80,0x80,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x02,0x03,0x01,0x01,0x00,0x00,0x00,/*"→",2*/
		
		0x00,0x00,0x00,0x00,0x3C,0xFC,0xF4,0x10,0x10,0xF4,0xFC,0x3C,0x00,0x00,0x00,0x00,
		0x00,0x00,0x30,0x30,0x3C,0x3D,0x05,0x07,0x07,0x05,0x3D,0x3C,0x30,0x30,0x00,0x00,/*"防御塔1",3*/
		
		0x00,0x0E,0x0C,0x18,0x38,0xF0,0x30,0xF6,0xF6,0x30,0xF0,0x38,0x18,0x0C,0x0E,0x00,
		0x20,0x20,0x38,0x2E,0x3E,0x27,0x39,0x3F,0x3F,0x39,0x27,0x3E,0x2E,0x38,0x20,0x20,/*"防御塔2",4*/
		
		0x00,0x00,0x0C,0x0C,0xE0,0x30,0x18,0x8B,0x8B,0x18,0x30,0xE0,0x0C,0x0C,0x00,0x00,
		0x00,0x00,0x20,0x3C,0x27,0x38,0x20,0x21,0x21,0x20,0x38,0x27,0x3C,0x20,0x00,0x00,/*"防御塔3",5*/
		
		0x00,0x00,0xE0,0x10,0x10,0x3C,0x5C,0x5C,0x3C,0x14,0x14,0x74,0xC4,0x80,0x00,0x00,
		0x00,0x00,0x07,0x1C,0x3C,0x1C,0x04,0x04,0x04,0x04,0x1C,0x3C,0x1D,0x07,0x00,0x00,/*"野怪1",6*/
		
		0x00,0x08,0x88,0xB8,0xEC,0x4C,0x7C,0xCC,0xCC,0x7C,0x4C,0xEC,0xB8,0x88,0x08,0x00,
		0x00,0x02,0x0B,0x0F,0x3F,0x3F,0x39,0x09,0x09,0x39,0x3F,0x3F,0x0F,0x0B,0x02,0x00,/*"野怪2",7*/
		
		0x80,0x80,0x1C,0x7C,0x6C,0x48,0xF8,0xCF,0xCF,0xF9,0x49,0x6C,0x7C,0x1C,0x80,0x80,
		0x00,0x03,0x02,0xCE,0xCE,0x6E,0x72,0x03,0x03,0x72,0x6E,0xCE,0xCE,0x02,0x03,0x00,/*"野怪3",8*/
		
		0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFE,0xF8,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x20,0x38,0x3E,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3E,0x38,0x20,0x00,0x00,/*"移除",9*/
		
		0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xFE,0xFE,0xFE,0xFE,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,
		0x00,0x03,0x03,0x03,0x03,0x03,0x7F,0x7F,0x7F,0x7F,0x03,0x03,0x03,0x03,0x03,0x00,/*"回血",10*/
	};
	
	ClearScreen(0);
	Set_line(0);
	
	//第一行
	show_ch(1,0,0*16,ch0+32*2);
	show_ch(1,0,1*16,ch0+32*9);
	show_ch(1,0,2*16,ch0+32*9);
	show_ch(1,0,3*16,ch0+32*9);
	show_ch(2,0,4*16,ch0+32*10);
	show_ch(2,0,5*16,ch0+32*10);
	show_ch(2,0,6*16,ch0+32*10);
	show_ch(2,0,7*16,ch0+32*1);
	//第二行
	show_ch(1,2,0*16,ch0+32*0);
	show_ch(1,2,1*16,ch0+32*3);
	show_ch(1,2,2*16,ch0+32*3);
	show_ch(1,2,3*16,ch0+32*3);
	show_ch(2,2,4*16,ch0+32*6);
	show_ch(2,2,5*16,ch0+32*6);
	show_ch(2,2,6*16,ch0+32*6);
	show_ch(2,2,7*16,ch0+32*0);
	//第三行
	show_ch(1,4,0*16,ch0+32*0);
	show_ch(1,4,1*16,ch0+32*5);
	show_ch(1,4,2*16,ch0+32*5);
	show_ch(1,4,3*16,ch0+32*5);
	show_ch(2,4,4*16,ch0+32*7);
	show_ch(2,4,5*16,ch0+32*7);
	show_ch(2,4,6*16,ch0+32*7);
	show_ch(2,4,7*16,ch0+32*0);
	//第四行
	show_ch(1,6,0*16,ch0+32*2);
	show_ch(1,6,1*16,ch0+32*4);
	show_ch(1,6,2*16,ch0+32*4);
	show_ch(1,6,3*16,ch0+32*4);
	show_ch(2,6,4*16,ch0+32*8);
	show_ch(2,6,5*16,ch0+32*8);
	show_ch(2,6,6*16,ch0+32*8);
	show_ch(2,6,7*16,ch0+32*1);
}

//开始菜单
void menu0()
{
	//开始游戏
	uint8 code ch0[]=
	{
	0x80,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x80,0x00,
	0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,/*"开",0*/

	0x10,0x10,0xF0,0x1F,0x10,0xF0,0x00,0x40,0xE0,0x58,0x47,0x40,0x50,0x60,0xC0,0x00,
	0x40,0x22,0x15,0x08,0x16,0x21,0x00,0x00,0xFE,0x42,0x42,0x42,0x42,0xFE,0x00,0x00,/*"始",1*/

	0x10,0x60,0x02,0x8C,0x00,0x08,0xF9,0x4E,0xC8,0x20,0x58,0x4F,0x48,0xC8,0x08,0x00,
	0x04,0x04,0x7E,0x81,0x40,0x30,0x0F,0x40,0x7F,0x00,0x44,0x84,0x7F,0x04,0x04,0x00,/*"游",2*/

	0x00,0x08,0x48,0x88,0x08,0xC8,0x38,0x40,0x40,0x40,0xFF,0x20,0x22,0xAC,0x20,0x00,
	0x00,0x20,0x10,0x0C,0x03,0x04,0x18,0x80,0x40,0x20,0x17,0x18,0x26,0x41,0xF0,0x00,/*"戏",3*/
	};
	//排行榜
	uint8 code ch1[]=
	{
	0x10,0x10,0x10,0xFF,0x90,0x08,0x88,0x88,0xFF,0x00,0x00,0xFF,0x88,0x88,0x08,0x00,
	0x02,0x42,0x81,0x7F,0x00,0x08,0x08,0x08,0xFF,0x00,0x00,0xFF,0x08,0x08,0x08,0x00,/*"排",0*/

	0x00,0x10,0x88,0xC4,0x33,0x00,0x40,0x42,0x42,0x42,0xC2,0x42,0x42,0x42,0x40,0x00,
	0x02,0x01,0x00,0xFF,0x00,0x00,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,/*"行",1*/

	0x10,0x10,0xD0,0xFF,0x90,0x90,0x64,0x2C,0x34,0xA5,0x26,0x34,0x2C,0xA4,0x60,0x00,
	0x04,0x03,0x00,0xFF,0x00,0x01,0x82,0x62,0x1E,0x0A,0x4B,0x8A,0x7A,0x02,0x00,0x00,/*"榜",2*/
	};
	//操作说明
	uint8 code ch2[]=
	{
	0x10,0x10,0xFF,0x10,0x90,0xE0,0x2F,0x29,0xE9,0x09,0xE9,0x29,0x2F,0xE0,0x00,0x00,
	0x42,0x82,0x7F,0x41,0x44,0x25,0x15,0x0D,0x05,0xFE,0x0D,0x15,0x25,0x45,0x44,0x00,/*"操",0*/

	0x00,0x80,0x60,0xF8,0x07,0x40,0x30,0x0F,0xF8,0x88,0x88,0x88,0x88,0x08,0x08,0x00,
	0x01,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0xFF,0x08,0x08,0x08,0x08,0x08,0x00,0x00,/*"作",1*/

	0x40,0x40,0x42,0xCC,0x00,0x00,0xF1,0x16,0x10,0x10,0x18,0x14,0xF3,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3F,0x90,0x48,0x21,0x19,0x07,0x01,0x3F,0x41,0x41,0x40,0x70,0x00,/*"说",2*/

	0x00,0xFC,0x44,0x44,0x44,0xFC,0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,
	0x00,0x0F,0x04,0x04,0x04,0x8F,0x40,0x30,0x0F,0x02,0x02,0x42,0x82,0x7F,0x00,0x00,/*"明",3*/
	};
	
	ClearScreen(0);
	Set_line(0);
	
	show_ch(1,0,2*16,ch0+32*0);
	show_ch(1,0,3*16,ch0+32*1);
	show_ch(2,0,0*16,ch0+32*2);
	show_ch(2,0,1*16,ch0+32*3);
	
	show_ch(1,3,2*16,ch1+32*0);
	show_ch(1,3,3*16,ch1+32*1);
	show_ch(2,3,0*16,ch1+32*2);
	
	show_ch(1,6,2*16,ch2+32*0);
	show_ch(1,6,3*16,ch2+32*1);
	show_ch(2,6,0*16,ch2+32*2);
	show_ch(2,6,1*16,ch2+32*3);
}

//开始菜单——排行榜
void menu0_1()
{
	//开始游戏
	uint8 code ch0[]=
	{
	0x00,0x00,0x90,0x88,0x4C,0x57,0x24,0x24,0x54,0x54,0x8C,0x84,0x00,0x00,0x00,0x00,
	0x01,0x01,0x00,0xFE,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0xFE,0x01,0x01,0x01,0x00,/*"各",0*/

	0x00,0x00,0x10,0x11,0x16,0x10,0x10,0xF0,0x10,0x10,0x14,0x13,0x10,0x00,0x00,0x00,
	0x81,0x81,0x41,0x41,0x21,0x11,0x0D,0x03,0x0D,0x11,0x21,0x41,0x41,0x81,0x81,0x00,/*"关",1*/

	0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x44,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x02,0x04,0x08,0x10,0x00,0x00,0x00,/*"卡",2*/

	0x40,0x40,0xC0,0x5F,0x55,0x55,0xD5,0x55,0x55,0x55,0x55,0x5F,0x40,0x40,0x40,0x00,
	0x20,0x60,0x3F,0x25,0x15,0x15,0xFF,0x90,0x47,0x29,0x11,0x2D,0x43,0x80,0x80,0x00,/*"最",3*/

	0x04,0x04,0x04,0x04,0xF4,0x94,0x95,0x96,0x94,0x94,0xF4,0x04,0x04,0x04,0x04,0x00,
	0x00,0xFE,0x02,0x02,0x7A,0x4A,0x4A,0x4A,0x4A,0x4A,0x7A,0x02,0x82,0xFE,0x00,0x00,/*"高",4*/

	0x80,0x40,0x20,0x90,0x88,0x86,0x80,0x80,0x80,0x83,0x8C,0x10,0x20,0x40,0x80,0x00,
	0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,0x00,0x00,0x00,/*"分",5*/

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"：",6*/
	};
	
	uint8 code ch1[]=
	{
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"一",0*/

	0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
	0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,/*"二",1*/

	0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x04,0x00,0x00,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,/*"三",2*/

	0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
	0x00,0x7F,0x28,0x24,0x23,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00,/*"四",3*/

	0x00,0x02,0x42,0x42,0x42,0xC2,0x7E,0x42,0x42,0x42,0x42,0xC2,0x02,0x02,0x00,0x00,
	0x40,0x40,0x40,0x40,0x78,0x47,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x00,/*"五",4*/

	0x20,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x2C,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
	0x00,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x01,0x02,0x04,0x18,0x60,0x00,0x00,/*"六",5*/		
	};
	
	uint8 code ch2[]=
	{
	0x00,0x00,0xE0,0xF0,0x30,0x08,0x08,0x08,0x08,0x08,0x08,0x30,0xF0,0xE0,0x00,0x00,
	0x00,0x00,0x0F,0x1F,0x18,0x30,0x20,0x20,0x20,0x20,0x20,0x18,0x0F,0x07,0x00,0x00,/*"0",0*/

	0x00,0x00,0x00,0x00,0x00,0x10,0x10,0xF0,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x20,0x20,0x20,0x3F,0x3F,0x20,0x20,0x20,0x20,0x00,0x00,0x00,/*"1",1*/

	0x00,0x00,0x30,0x70,0x28,0x08,0x08,0x08,0x08,0x08,0x08,0xD8,0xF0,0x20,0x00,0x00,
	0x00,0x00,0x30,0x30,0x28,0x24,0x24,0x22,0x22,0x21,0x21,0x20,0x30,0x18,0x00,0x00,/*"2",2*/

	0x00,0x00,0x30,0x30,0x28,0x08,0x08,0x08,0x08,0x88,0x88,0x70,0x70,0x00,0x00,0x00,
	0x00,0x00,0x18,0x18,0x28,0x20,0x20,0x21,0x21,0x21,0x21,0x13,0x1E,0x0C,0x00,0x00,/*"3",3*/

	0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x20,0x10,0xF0,0xF8,0xF8,0x00,0x00,0x00,0x00,
	0x00,0x04,0x06,0x05,0x04,0x04,0x24,0x24,0x24,0x3F,0x3F,0x3F,0x24,0x24,0x24,0x00,/*"4",4*/

	0x00,0x00,0x00,0xF8,0x08,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x08,0x00,0x00,0x00,
	0x00,0x00,0x18,0x19,0x29,0x20,0x20,0x20,0x20,0x20,0x20,0x11,0x1F,0x0E,0x00,0x00,/*"5",5*/

	0x00,0x00,0xC0,0xE0,0x10,0x88,0x88,0x88,0x88,0x88,0x88,0x98,0x10,0x00,0x00,0x00,
	0x00,0x00,0x0F,0x1F,0x11,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x1F,0x0F,0x00,0x00,/*"6",6*/

	0x00,0x00,0x30,0x18,0x08,0x08,0x08,0x08,0x08,0x88,0x48,0x28,0x18,0x08,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x3E,0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"7",7*/

	0x00,0x00,0x70,0x70,0xC8,0x88,0x88,0x88,0x08,0x08,0x88,0x88,0x70,0x20,0x00,0x00,
	0x00,0x0C,0x1E,0x12,0x21,0x21,0x20,0x21,0x21,0x21,0x23,0x22,0x1E,0x0C,0x00,0x00,/*"8",8*/

	0x00,0x40,0xF0,0xF0,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x90,0xF0,0xE0,0x00,0x00,
	0x00,0x00,0x11,0x11,0x33,0x22,0x22,0x22,0x22,0x22,0x11,0x1C,0x0F,0x07,0x00,0x00,/*"9",9*/

	};
	
	ClearScreen(0);
	Set_line(0);
	
	read_eeprom(score);	//读取数据
	
	//第一行
	show_ch(1,0,0*16,ch0+32*0);
	show_ch(1,0,1*16,ch0+32*1);
	show_ch(1,0,2*16,ch0+32*2);
	show_ch(1,0,3*16,ch0+32*3);
	show_ch(2,0,4*16,ch0+32*4);
	show_ch(2,0,5*16,ch0+32*5);
	show_ch(2,0,6*16,ch0+32*6);
	//第二行
	show_ch(1,2,0*16,ch1+32*0);
	show_ch(1,2,1*16,ch0+32*6);
	show_ch(1,2,2*16,ch2+32*score[0]);
	
	show_ch(2,2,4*16,ch1+32*3);
	show_ch(2,2,5*16,ch0+32*6);
	show_ch(2,2,6*16,ch2+32*score[3]);
	//第三行
	show_ch(1,4,0*16,ch1+32*1);
	show_ch(1,4,1*16,ch0+32*6);
	show_ch(1,4,2*16,ch2+32*score[1]);
	
	show_ch(2,4,4*16,ch1+32*4);
	show_ch(2,4,5*16,ch0+32*6);
	show_ch(2,4,6*16,ch2+32*score[4]);
	//第四行
	show_ch(1,6,0*16,ch1+32*2);
	show_ch(1,6,1*16,ch0+32*6);
	show_ch(1,6,2*16,ch2+32*score[2]);
	
	show_ch(2,6,4*16,ch1+32*5);
	show_ch(2,6,5*16,ch0+32*6);
	show_ch(2,6,6*16,ch2+32*score[5]);
}

//开始菜单——关卡选择
void menu1()
{
	uint8 code ch0[]=
	{	
	0x00,0x00,0x10,0x11,0x16,0x10,0x10,0xF0,0x10,0x10,0x14,0x13,0x10,0x00,0x00,0x00,
	0x81,0x81,0x41,0x41,0x21,0x11,0x0D,0x03,0x0D,0x11,0x21,0x41,0x41,0x81,0x81,0x00,/*"关",0*/

	0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x44,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x02,0x04,0x08,0x10,0x00,0x00,0x00,/*"卡",1*/
		
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"一",2*/

	0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
	0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,/*"二",3*/

	0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x04,0x00,0x00,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,/*"三",4*/

	0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
	0x00,0x7F,0x28,0x24,0x23,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00,/*"四",5*/

	0x00,0x02,0x42,0x42,0x42,0xC2,0x7E,0x42,0x42,0x42,0x42,0xC2,0x02,0x02,0x00,0x00,
	0x40,0x40,0x40,0x40,0x78,0x47,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x00,/*"五",6*/
		
	0x20,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x2C,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
	0x00,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x01,0x02,0x04,0x18,0x60,0x00,0x00,/*"六",7*/
	};
	
	ClearScreen(0);
	Set_line(0);
	//第一行
	show_ch(1,0,0*16,ch0+32*0);
	show_ch(1,0,1*16,ch0+32*1);
	show_ch(1,0,2*16,ch0+32*2);
	show_ch(2,0,5*16,ch0+32*0);
	show_ch(2,0,6*16,ch0+32*1);
	show_ch(2,0,7*16,ch0+32*5);
	//第二行
	show_ch(1,3,0*16,ch0+32*0);
	show_ch(1,3,1*16,ch0+32*1);
	show_ch(1,3,2*16,ch0+32*3);
	show_ch(2,3,5*16,ch0+32*0);
	show_ch(2,3,6*16,ch0+32*1);
	show_ch(2,3,7*16,ch0+32*6);
	//第三行
	show_ch(1,6,0*16,ch0+32*0);
	show_ch(1,6,1*16,ch0+32*1);
	show_ch(1,6,2*16,ch0+32*4);
	show_ch(2,6,5*16,ch0+32*0);
	show_ch(2,6,6*16,ch0+32*1);
	show_ch(2,6,7*16,ch0+32*7);
}

//开始菜单——操作说明
void menu0_2()
{
	uint8 code ch0[]=
	{
	0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
	0x00,0x7F,0x28,0x24,0x23,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00,/*"四",0*/

	0x80,0x80,0x40,0x20,0x10,0x08,0x04,0xC3,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"个",1*/

	0x08,0x08,0x08,0x08,0x08,0xF8,0x89,0x8E,0x88,0x88,0x88,0x88,0x08,0x08,0x08,0x00,
	0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,0x00,0x00,/*"方",2*/

	0x00,0xF8,0x08,0x08,0x0C,0xCA,0x49,0x48,0x48,0xC8,0x08,0x08,0x08,0xF8,0x00,0x00,
	0x00,0xFF,0x00,0x00,0x00,0x1F,0x08,0x08,0x08,0x1F,0x00,0x40,0x80,0x7F,0x00,0x00,/*"向",3*/

	0x40,0x30,0xEF,0x24,0x24,0x80,0xE4,0x9C,0x10,0x54,0x54,0xFF,0x54,0x7C,0x10,0x00,
	0x01,0x01,0x7F,0x21,0x51,0x26,0x18,0x27,0x44,0x45,0x45,0x5F,0x45,0x45,0x44,0x00,/*"键",4*/
		
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"：",5*/
	};
	
	uint8 code ch1[]=
	{
	0x10,0x10,0x10,0xFF,0x90,0x20,0x98,0x48,0x28,0x09,0x0E,0x28,0x48,0xA8,0x18,0x00,
	0x02,0x42,0x81,0x7F,0x00,0x40,0x40,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x40,0x00,/*"控",0*/

	0x40,0x50,0x4E,0x48,0x48,0xFF,0x48,0x48,0x48,0x40,0xF8,0x00,0x00,0xFF,0x00,0x00,
	0x00,0x00,0x3E,0x02,0x02,0xFF,0x12,0x22,0x1E,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,/*"制",1*/

	0x40,0x40,0x42,0x44,0x58,0xC0,0x40,0x7F,0x40,0xC0,0x50,0x48,0x46,0x40,0x40,0x00,
	0x80,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00,/*"光",2*/

	0x10,0x10,0xD0,0xFF,0x90,0x10,0x20,0x22,0x22,0x22,0xE2,0x22,0x22,0x22,0x20,0x00,
	0x04,0x03,0x00,0xFF,0x00,0x13,0x0C,0x03,0x40,0x80,0x7F,0x00,0x01,0x06,0x18,0x00,/*"标",3*/

	0x00,0xF8,0x0C,0x0B,0x08,0x08,0xF8,0x40,0x30,0x8F,0x08,0x08,0x08,0xF8,0x00,0x00,
	0x00,0x7F,0x21,0x21,0x21,0x21,0x7F,0x00,0x00,0x00,0x43,0x80,0x40,0x3F,0x00,0x00,/*"的",4*/

	0x24,0x24,0xA4,0xFE,0x23,0x22,0x00,0x10,0x08,0x94,0x67,0x24,0x94,0x0C,0x00,0x00,
	0x08,0x06,0x01,0xFF,0x01,0x06,0x00,0x89,0x89,0x44,0x4A,0x33,0x12,0x0A,0x06,0x00,/*"移",5*/

	0x40,0x44,0xC4,0x44,0x44,0x44,0x40,0x10,0x10,0xFF,0x10,0x10,0x10,0xF0,0x00,0x00,
	0x10,0x3C,0x13,0x10,0x14,0xB8,0x40,0x30,0x0E,0x01,0x40,0x80,0x40,0x3F,0x00,0x00,/*"动",6*/
	};
	
	uint8 code ch2[]=
	{
	0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x04,0x00,0x00,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,/*"三",0*/

	0x80,0x80,0x40,0x20,0x10,0x08,0x04,0xC3,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"个",1*/

	0x08,0x08,0x08,0xF8,0x08,0x08,0x08,0x10,0x10,0xFF,0x10,0x10,0x10,0xF0,0x00,0x00,
	0x10,0x30,0x10,0x1F,0x08,0x88,0x48,0x30,0x0E,0x01,0x40,0x80,0x40,0x3F,0x00,0x00,/*"功",2*/

	0x08,0xCC,0x4A,0x49,0x48,0x4A,0xCC,0x18,0x00,0x7F,0x88,0x88,0x84,0x82,0xE0,0x00,
	0x00,0xFF,0x12,0x12,0x52,0x92,0x7F,0x00,0x00,0x7E,0x88,0x88,0x84,0x82,0xE0,0x00,/*"能",3*/

	0x40,0x30,0xEF,0x24,0x24,0x80,0xE4,0x9C,0x10,0x54,0x54,0xFF,0x54,0x7C,0x10,0x00,
	0x01,0x01,0x7F,0x21,0x51,0x26,0x18,0x27,0x44,0x45,0x45,0x5F,0x45,0x45,0x44,0x00,/*"键",4*/
	};
	
	uint8 code ch3[]=
	{
	0x00,0xFE,0x02,0x22,0xDA,0x06,0x08,0x08,0xF8,0x89,0x8E,0x88,0x88,0x88,0x08,0x00,
	0x00,0xFF,0x08,0x10,0x08,0x87,0x40,0x30,0x0F,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,/*"防",0*/

	0x10,0x88,0xC4,0x33,0x48,0x47,0x44,0xFC,0x44,0x44,0x00,0xFC,0x04,0x04,0xFC,0x00,
	0x01,0x00,0xFF,0x20,0x60,0x3E,0x20,0x1F,0x12,0x12,0x00,0xFF,0x08,0x10,0x0F,0x00,/*"御",1*/

	0x20,0x20,0xFF,0x20,0x20,0x04,0x84,0x4F,0x24,0x14,0x24,0x4F,0x84,0x04,0x00,0x00,
	0x10,0x30,0x1F,0x08,0x08,0x01,0xF8,0x49,0x49,0x49,0x49,0x49,0xF8,0x01,0x01,0x00,/*"塔",2*/

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x04,0x08,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"、",3*/

	0x40,0x40,0x42,0xCC,0x00,0x08,0xE9,0xAA,0xB8,0xA8,0xA8,0xAA,0xE9,0x08,0x00,0x00,
	0x00,0x40,0x20,0x1F,0x20,0x40,0x5F,0x4A,0x4A,0x4A,0x4A,0x4A,0x5F,0x40,0x40,0x00,/*"道",4*/

	0x00,0x00,0x00,0xFE,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0xFE,0x00,0x00,0x00,0x00,
	0x08,0x08,0x88,0x4F,0x29,0x19,0x09,0x09,0x09,0x19,0x29,0x4F,0x88,0x08,0x08,0x00,/*"具",5*/

	0x10,0x10,0xFF,0x10,0x90,0xE0,0x2F,0x29,0xE9,0x09,0xE9,0x29,0x2F,0xE0,0x00,0x00,
	0x42,0x82,0x7F,0x41,0x44,0x25,0x15,0x0D,0x05,0xFE,0x0D,0x15,0x25,0x45,0x44,0x00,/*"操",6*/

	0x00,0x80,0x60,0xF8,0x07,0x40,0x30,0x0F,0xF8,0x88,0x88,0x88,0x88,0x08,0x08,0x00,
	0x01,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0xFF,0x08,0x08,0x08,0x08,0x08,0x00,0x00,/*"作",7*/
	};
	
	ClearScreen(0);
	Set_line(0);
	//第一行
	show_ch(1,0,0*16,ch0+32*0);
	show_ch(1,0,1*16,ch0+32*1);
	show_ch(1,0,2*16,ch0+32*2);
	show_ch(1,0,3*16,ch0+32*3);
	show_ch(2,0,4*16,ch0+32*4);
	show_ch(2,0,5*16,ch0+32*5);
	//第二行
	show_ch(1,2,0*16,ch1+32*0);
	show_ch(1,2,1*16,ch1+32*1);
	show_ch(1,2,2*16,ch1+32*2);
	show_ch(1,2,3*16,ch1+32*3);
	show_ch(2,2,4*16,ch1+32*4);
	show_ch(2,2,5*16,ch1+32*5);
	show_ch(2,2,6*16,ch1+32*6);
	//第三行
	show_ch(1,4,0*16,ch2+32*0);
	show_ch(1,4,1*16,ch2+32*1);
	show_ch(1,4,2*16,ch2+32*2);
	show_ch(1,4,3*16,ch2+32*3);
	show_ch(2,4,4*16,ch2+32*4);
	show_ch(2,4,5*16,ch0+32*5);
	//第四行
	show_ch(1,6,0*16,ch3+32*0);
	show_ch(1,6,1*16,ch3+32*1);
	show_ch(1,6,2*16,ch3+32*2);
	show_ch(1,6,3*16,ch3+32*3);
	show_ch(2,6,4*16,ch3+32*4);
	show_ch(2,6,5*16,ch3+32*5);
	show_ch(2,6,6*16,ch3+32*6);
	show_ch(2,6,7*16,ch3+32*7);
}

//胜利界面
void gamevictory()
{
	uint8 code ch0[]=
	{
	0x00,0xFE,0x22,0x22,0xFE,0x00,0x40,0x3C,0x10,0x10,0xFF,0x10,0x10,0x10,0x00,0x00,
	0x80,0x7F,0x02,0x82,0xFF,0x00,0x40,0x42,0x42,0x42,0x7F,0x42,0x42,0x42,0x40,0x00,/*"胜",0*/

	0x40,0x44,0x44,0x44,0xC4,0xFE,0x42,0x43,0x42,0x00,0xF8,0x00,0x00,0xFF,0x00,0x00,
	0x20,0x10,0x0C,0x03,0x00,0xFF,0x01,0x02,0x04,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,/*"利",1*/
	};
	
	ClearScreen(0);
	Set_line(0);

	show_ch(1,3,3*16,ch0+32*0);
	show_ch(2,3,4*16,ch0+32*1);
}
//失败界面
void gamefailure()
{
	uint8 code ch0[]=
	{
	0x00,0x40,0x30,0x1E,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
	0x81,0x81,0x41,0x21,0x11,0x0D,0x03,0x01,0x03,0x0D,0x11,0x21,0x41,0x81,0x81,0x00,/*"失",0*/

	0x00,0xFE,0x02,0xFA,0x02,0xFE,0x40,0x20,0xD8,0x17,0x10,0x10,0xF0,0x10,0x10,0x00,
	0x80,0x47,0x30,0x0F,0x10,0x67,0x80,0x40,0x21,0x16,0x08,0x16,0x21,0x40,0x80,0x00,/*"败",1*/
	};
	
	ClearScreen(0);
	Set_line(0);
	
	show_ch(1,3,3*16,ch0+32*0);
	show_ch(2,3,4*16,ch0+32*1);
}

//游戏结束界面
void gameover()
{
	uint8 code ch0[]=
	{
	0x10,0x60,0x02,0x8C,0x00,0x08,0xF9,0x4E,0xC8,0x20,0x58,0x4F,0x48,0xC8,0x08,0x00,
	0x04,0x04,0x7E,0x81,0x40,0x30,0x0F,0x40,0x7F,0x00,0x44,0x84,0x7F,0x04,0x04,0x00,/*"游",0*/

	0x00,0x08,0x48,0x88,0x08,0xC8,0x38,0x40,0x40,0x40,0xFF,0x20,0x22,0xAC,0x20,0x00,
	0x00,0x20,0x10,0x0C,0x03,0x04,0x18,0x80,0x40,0x20,0x17,0x18,0x26,0x41,0xF0,0x00,/*"戏",1*/

	0x20,0x30,0xAC,0x63,0x20,0x18,0x08,0x48,0x48,0x48,0x7F,0x48,0x48,0x48,0x08,0x00,
	0x22,0x67,0x22,0x12,0x12,0x12,0x00,0xFE,0x42,0x42,0x42,0x42,0x42,0xFE,0x00,0x00,/*"结",2*/

	0x04,0x04,0xE4,0x24,0x24,0x24,0x24,0xFF,0x24,0x24,0x24,0x24,0xE4,0x04,0x04,0x00,
	0x40,0x40,0x27,0x22,0x12,0x0A,0x06,0xFF,0x06,0x0A,0x12,0x22,0x27,0x40,0x40,0x00,/*"束",3*/
	};
	
	uint8 code ch1[]=
	{
	0x10,0x10,0x10,0xFF,0x90,0x20,0x98,0x88,0x88,0xE9,0x8E,0x88,0x88,0xA8,0x98,0x00,
	0x02,0x42,0x81,0x7F,0x00,0x00,0x80,0x84,0x4B,0x28,0x10,0x28,0x47,0x80,0x00,0x00,/*"按",0*/

	0x04,0x84,0xE4,0x5C,0x44,0xC4,0x20,0x10,0xE8,0x27,0x24,0xE4,0x34,0x2C,0xE0,0x00,
	0x02,0x01,0x7F,0x10,0x10,0x3F,0x80,0x60,0x1F,0x09,0x09,0x3F,0x49,0x89,0x7F,0x00,/*"确",1*/

	0x40,0x40,0x42,0xCC,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3F,0x90,0x48,0x20,0x18,0x07,0x00,0x07,0x18,0x20,0x40,0x80,0x00,/*"认",2*/

	0x40,0x30,0xEF,0x24,0x24,0x80,0xE4,0x9C,0x10,0x54,0x54,0xFF,0x54,0x7C,0x10,0x00,
	0x01,0x01,0x7F,0x21,0x51,0x26,0x18,0x27,0x44,0x45,0x45,0x5F,0x45,0x45,0x44,0x00,/*"键",3*/

	0x20,0x30,0xAC,0x63,0x10,0x00,0xFE,0x48,0x50,0x40,0xFF,0x40,0x50,0x48,0x00,0x00,
	0x22,0x67,0x22,0x12,0x12,0x00,0x7F,0x44,0x42,0x41,0x7F,0x41,0x42,0x44,0x40,0x00,/*"继",4*/

	0x20,0x30,0xAC,0x63,0x20,0x18,0x20,0x24,0xA4,0x24,0x3F,0xA4,0x24,0xA4,0x60,0x00,
	0x22,0x67,0x22,0x12,0x12,0x12,0x88,0x8A,0x4C,0x29,0x18,0x0F,0x18,0x28,0xC8,0x00,/*"续",5*/

	0x10,0x60,0x02,0x8C,0x00,0x08,0xF9,0x4E,0xC8,0x20,0x58,0x4F,0x48,0xC8,0x08,0x00,
	0x04,0x04,0x7E,0x81,0x40,0x30,0x0F,0x40,0x7F,0x00,0x44,0x84,0x7F,0x04,0x04,0x00,/*"游",6*/

	0x00,0x08,0x48,0x88,0x08,0xC8,0x38,0x40,0x40,0x40,0xFF,0x20,0x22,0xAC,0x20,0x00,
	0x00,0x20,0x10,0x0C,0x03,0x04,0x18,0x80,0x40,0x20,0x17,0x18,0x26,0x41,0xF0,0x00,/*"戏",7*/
	};
	
	uint8 code ch2[]=
	{
	0x40,0x40,0x42,0xCC,0x00,0x00,0xFC,0x24,0xA4,0x24,0x22,0x22,0xA3,0x62,0x00,0x00,
	0x00,0x40,0x20,0x1F,0x20,0x58,0x47,0x50,0x48,0x45,0x42,0x45,0x48,0x50,0x40,0x00,/*"返",0*/

	0x00,0x00,0xFE,0x02,0x02,0xF2,0x12,0x12,0x12,0xF2,0x02,0x02,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x7F,0x20,0x20,0x27,0x24,0x24,0x24,0x27,0x20,0x20,0x7F,0x00,0x00,0x00,/*"回",1*/

	0x40,0x30,0xEF,0x24,0x24,0x80,0xE4,0x9C,0x10,0x54,0x54,0xFF,0x54,0x7C,0x10,0x00,
	0x01,0x01,0x7F,0x21,0x51,0x26,0x18,0x27,0x44,0x45,0x45,0x5F,0x45,0x45,0x44,0x00,/*"键",2*/

	0x00,0x00,0xFE,0x02,0x02,0xF2,0x12,0x12,0x12,0xF2,0x02,0x02,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x7F,0x20,0x20,0x27,0x24,0x24,0x24,0x27,0x20,0x20,0x7F,0x00,0x00,0x00,/*"回",3*/

	0x42,0x62,0x52,0x4A,0xC6,0x42,0x52,0x62,0xC2,0x00,0xF8,0x00,0x00,0xFF,0x00,0x00,
	0x40,0xC4,0x44,0x44,0x7F,0x24,0x24,0x24,0x20,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,/*"到",4*/

	0x00,0x08,0x08,0x08,0x08,0x08,0x09,0xFA,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,
	0x40,0x40,0x41,0x41,0x41,0x41,0x41,0x7F,0x41,0x41,0x41,0x41,0x41,0x40,0x40,0x00,/*"主",5*/

	0x04,0x04,0x44,0xC4,0x4F,0x44,0x44,0xC4,0x24,0x24,0x2F,0xB4,0x24,0x04,0x04,0x00,
	0x40,0x44,0x24,0x24,0x15,0x0C,0x04,0xFE,0x04,0x0C,0x15,0x24,0x24,0x44,0x40,0x00,/*"菜",6*/

	0x00,0x00,0xF8,0x49,0x4A,0x4C,0x48,0xF8,0x48,0x4C,0x4A,0x49,0xF8,0x00,0x00,0x00,
	0x10,0x10,0x13,0x12,0x12,0x12,0x12,0xFF,0x12,0x12,0x12,0x12,0x13,0x10,0x10,0x00,/*"单",7*/
	};
	
	ClearScreen(0);
	Set_line(0);
	
	show_ch(1,0,2*16,ch0+32*0);
	show_ch(1,0,3*16,ch0+32*1);
	show_ch(2,0,4*16,ch0+32*2);
	show_ch(2,0,5*16,ch0+32*3);
	
	show_ch(1,3,0*16,ch1+32*0);
	show_ch(1,3,1*16,ch1+32*1);
	show_ch(1,3,2*16,ch1+32*2);
	show_ch(1,3,3*16,ch1+32*3);
	show_ch(2,3,4*16,ch1+32*4);
	show_ch(2,3,5*16,ch1+32*5);
	show_ch(2,3,6*16,ch1+32*6);
	show_ch(2,3,7*16,ch1+32*7);
	
	show_ch(1,6,0*16,ch2+32*0);
	show_ch(1,6,1*16,ch2+32*1);
	show_ch(1,6,2*16,ch2+32*2);
	show_ch(1,6,3*16,ch2+32*3);
	show_ch(2,6,4*16,ch2+32*4);
	show_ch(2,6,5*16,ch2+32*5);
	show_ch(2,6,6*16,ch2+32*6);
	show_ch(2,6,7*16,ch2+32*7);
}
