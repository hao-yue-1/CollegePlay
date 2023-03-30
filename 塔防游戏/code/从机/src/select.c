#include "select.h"
#include "menu.h"
#include "checkpoint.h"
#include <reg52.h>
#include "usart.h"
#include "score.h"
#include "arrow.h"

#define index_1_max 1	//��ҳ��ҳ����1����ҳ��
#define index2_max 3
uchar wel=1;		//��ӭҳ��ʶ
uchar index_1=0;	//һ���˵�����
uchar index=0;		//��Ļ����
uchar inx2=1;		//�����˵�ѡ�񻺴�
uchar inx3=1;		//�����˵�ѡ�񻺴棨����ӣ���1~6��
uchar m_p=1;		//�˵���꣨һ����ͷ��
uchar y=0;			//��0,3,6��

uchar key;

void show()
{
	switch(index+index_1*10)
	{		
		case 0:menu0();		break;	//��ʼ�˵�
		case 1:{//�ؿ�ѡ��˵�
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
					delay(1000);	//��ֹ������Ϸ���������÷�����
					key=receive();
					show1();
					break;
				}
				key=receive();
				if(key==0xf7)	//up
				{
					if(inx3>1)
					{
						if(inx3==4)	//�պ�Ҫ��ת��������һ��
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
						if(inx3==3)	//�պ�Ҫ��ת���ұߵ�һ��
							y=0;
						else		//�������
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
					shift_game_draw_left(y);	//�����¹��
				else
					shift_game_draw_right(y);	//�����¹��
				key=receive();
			}
			break;
		}
		case 2:menu0_1();	break;	//��ʼ�˵��������а�
		case 3:menu0_2();	break;	//��ʼ�˵���������˵��
		default:			break;
	}
}

//ѡ�����ؿ�
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
		//while((key&0x1f)!=0x1f);	//�ȴ������ɿ�  
		while((receive()&0x1f)!=0x1f);	//�ȴ������ɿ�
		if(wel==1)//�״ΰ��������������ӭҳ��
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
							index=inx2;	//���Ӳ˵���
						show();
						break; //ȷ��
				}   
				case 0x1d: {
						index=0;
						show();
						break; //����
				}   			
				case 0x1b: {
						if(index==0 && m_p<3)
						{
							if(inx2<index2_max)
								inx2++;
							shift(m_p,m_p+1);
							m_p++;
						}
						break;  	//����
				}
				case 0x17: {
						if(index==0 && m_p>1)
						{ 
							if(inx2>0)
								inx2--;
							shift(m_p,m_p-1);
							m_p--;
						}
						break;	//����
				}
//				case 0x0f: {
//						if(!index)
//						{
//							index_1++;
//							if(index_1>index_1_max)
//								index_1=0;
//							show();	
//						}
//						break;	//��ҳ
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