#include "select.h"
#include "menu.h"
#include "checkpoint.h"
#include <reg52.h>
#include "usart.h"
#include "score.h"
#include "arrow.h"

#define index_1_max 1	//翻页的页数（1：两页）
#define index2_max 3
uchar wel=1;		//欢迎页标识
uchar index_1=0;	//一级菜单索引
uchar index=0;		//屏幕索引
uchar inx2=1;		//二级菜单选择缓存
uchar inx3=1;		//三级菜单选择缓存（自添加）（1~6）
uchar m_p=1;		//菜单光标（一个箭头）
uchar y=0;			//（0,3,6）

uchar key;

void show()
{
	switch(index+index_1*10)
	{		
		case 0:menu0();		break;	//开始菜单
		case 1:{//关卡选择菜单
			menu1();
			key=receive();
			while(key!=0xfd)
			{
				key=receive();
				if(inx3<4)
					shift_game_clear_left(y);
				else
					shift_game_clear_right(y);
				key=receive();
				if(key==0xfe)	//enter
				{
					delay(1000);	//防止进入游戏后，立即放置防御塔
					key=receive();
					show1();
					break;
				}
				key=receive();
				if(key==0xf7)	//up
				{
					if(inx3>1)
					{
						if(inx3==4)	//刚好要跳转到左边最后一行
							y=6;
						else
						{
							if(y>0)
								y-=3;
						}
						inx3--;
					}
					delay(1000);
					key=receive();
				}
				key=receive();
				if(key==0xfb)	//down
				{
					if(inx3<6)
					{
						if(inx3==3)	//刚好要跳转到右边第一行
							y=0;
						else		//其他情况
						{
							if(y<6)
								y+=3;
						}
						inx3++;	//1->2->3
					}
					delay(1000);
					key=receive();
				}
				key=receive();
				if(inx3<4)
					shift_game_draw_left(y);	//画出新光标
				else
					shift_game_draw_right(y);	//画出新光标
				key=receive();
			}
			break;
		}
		case 2:menu0_1();	break;	//开始菜单——排行榜
		case 3:menu0_2();	break;	//开始菜单——操作说明
		default:			break;
	}
}

//选择进入关卡
void show1()
{
	switch(inx3)
	{
		case 0:menu1();		break;
		case 1:game1();		break;
		case 2:game2();		break;
		case 3:game3();		break;
		case 4:game4();		break;
		case 5:game5();		break;
		case 6:game6();		break;
		default: 			break;
	}
}

void keyscan()   
{    
	uchar keyword;
	init_USART_receive();
	key=receive();
	keyword=key&0x1f;   
	if(keyword!=0x1f)   
	{   
		//while((key&0x1f)!=0x1f);	//等待按键松开  
		while((receive()&0x1f)!=0x1f);	//等待按键松开
		if(wel==1)//首次按下任意键跳过欢迎页面
		{
			wel=0;
			show();
		}
		else
		{
			switch(keyword)   
			{   
				case 0x1e: {
						if(index==0)
							index=inx2;	//往子菜单跳
						show();
						break; //确认
				}   
				case 0x1d: {
						index=0;
						show();
						break; //回退
				}   			
				case 0x1b: {
						if(index==0 && m_p<3)
						{
							if(inx2<index2_max)
								inx2++;
							shift(m_p,m_p+1);
							m_p++;
						}
						break;  	//下移
				}
				case 0x17: {
						if(index==0 && m_p>1)
						{ 
							if(inx2>0)
								inx2--;
							shift(m_p,m_p-1);
							m_p--;
						}
						break;	//上移
				}
//				case 0x0f: {
//						if(!index)
//						{
//							index_1++;
//							if(index_1>index_1_max)
//								index_1=0;
//							show();	
//						}
//						break;	//翻页
//				}
				default:break;  
			}
		}
	}   
}

void delay(uint x)
{
	uint a,b;
	for(a=0;a<x;a++)
		for(b=0;b<120;b++);
	
}