#ifndef _MENU_H
#define _MENU_H

#define uint8 unsigned char

extern uint8 score[6];	//��¼6���ؿ��ķ���

void menu0();	//��ʼ�˵�
void menu0_1();	//��ʼ�˵��������а�
void menu0_2();	//��ʼ�˵���������˵��
void menu1();	//�ؿ�ѡ��˵�

void welcome();		//��ӭ����
void gameover();	//��������
void gamevictory();	//ʤ������
void gamefailure();	//ʧ�ܽ���

#endif