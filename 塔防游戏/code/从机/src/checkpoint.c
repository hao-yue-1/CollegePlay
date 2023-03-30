#include "checkpoint.h"
#include "game.h"
#include "select.h"
#include "menu.h"
#include "LCD12864.h"
#include <reg52.h>
#include "score.h"
#include "usart.h"
#include "information.h"

//��һ��
void game1()
{
	init_beep();
	play_beep();	//��������
	ClearScreen(0);	//�����Ļ
	Set_line(0);
	init_USART_receive();
	init_game();	//��ʼ����Ϸ����
	initex();		//�����жϣ�������Ϸ��������Ӧ
	game1_1();		//������Ϸ��Ұ�ֿ�ʼˢ��
	play_beep();	//��������
	EX0=0;			//�ر��ⲿ�ж�0
	EA=0;
	gameover();		//��Ϸ��������
}

//�ڶ���
void game2()
{
	init_beep();
	play_beep();	//��������
	ClearScreen(0);	//�����Ļ
	Set_line(0);
	init_USART_receive();
	init_game();	//��ʼ����Ϸ����
	initex();		//�����жϣ�������Ϸ��������Ӧ
	game1_2();		//������Ϸ��Ұ�ֿ�ʼˢ��
	play_beep();	//��������
	EX0=0;			//�ر��ⲿ�ж�0
	EA=0;
	gameover();		//��Ϸ��������
}

//������
void game3()
{
	init_beep();
	play_beep();	//��������
	ClearScreen(0);	//�����Ļ
	Set_line(0);
	init_USART_receive();
	init_game();	//��ʼ����Ϸ����
	initex();		//�����жϣ�������Ϸ��������Ӧ
	game1_3();		//������Ϸ��Ұ�ֿ�ʼˢ��
	play_beep();	//��������
	EX0=0;			//�ر��ⲿ�ж�0
	EA=0;
	gameover();		//��Ϸ��������
}

//���Ĺ�
void game4()
{
	init_beep();
	play_beep();	//��������
	ClearScreen(0);	//�����Ļ
	Set_line(0);
	init_USART_receive();
	init_game();	//��ʼ����Ϸ����
	initex();		//�����жϣ�������Ϸ��������Ӧ
	game1_4();		//������Ϸ��Ұ�ֿ�ʼˢ��
	play_beep();	//��������
	EX0=0;			//�ر��ⲿ�ж�0
	EA=0;
	gameover();		//��Ϸ��������
}

//�����
void game5()
{
	init_beep();
	play_beep();	//��������
	ClearScreen(0);	//�����Ļ
	Set_line(0);
	init_USART_receive();
	init_game();	//��ʼ����Ϸ����
	initex();		//�����жϣ�������Ϸ��������Ӧ
	game1_5();		//������Ϸ��Ұ�ֿ�ʼˢ��
	play_beep();	//��������
	EX0=0;			//�ر��ⲿ�ж�0
	EA=0;
	gameover();		//��Ϸ��������
}

//������
void game6()
{
	init_beep();
	play_beep();	//��������
	ClearScreen(0);	//�����Ļ
	Set_line(0);
	init_USART_receive();
	init_game();	//��ʼ����Ϸ����
	initex();		//�����жϣ�������Ϸ��������Ӧ
	game1_6();		//������Ϸ��Ұ�ֿ�ʼˢ��
	play_beep();	//��������
	EX0=0;			//�ر��ⲿ�ж�0
	EA=0;
	gameover();		//��Ϸ��������
}