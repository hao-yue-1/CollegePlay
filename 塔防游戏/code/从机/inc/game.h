#ifndef _GAME_H
#define _GAME_H

#define uchar unsigned char
#define uint unsigned int
	
extern uchar score_temp;

//������������ṹ��
typedef struct node_tower
{
	uchar s;
	uchar x;
	uchar y;
	uchar flag;
}towers_xy;

void initex();		//��ʼ���ⲿ�ж�0

void game_monster1_go(uchar s,uchar x);	//Ұ��1ֱ���н�����->�ң��������У�
void game_monster1_down(uchar x);		//Ұ��1ֱ���н�����->�£����Ұ������ң�
void game_monster2_go(uchar s,uchar x);	//Ұ��2ֱ���н�����->�ң��������У�
void game_monster2_down(uchar x);		//Ұ��2ֱ���н�����->�£����Ұ������ң�
void game_monster3_go(uchar s,uchar x);	//Ұ��3ֱ���н�����->�ң��������У�
void game_monster3_down(uchar x);		//Ұ��3ֱ���н�����->�£����Ұ������ң�
void game_monster_die(uchar s,uchar x);	//ĨȥҰ�֣�Ұ��������
void game_monster_get(uchar s,uchar x);	//ĨȥҰ�֣�Ұ�ֵ�����أ�

void game_tower();			//��һ��������
void game_tower2();			//��һ����������������
void game_tower3();			//��һ����������Զ������
void game_cursor_draw();	//�������
void game_cursor_clear();	//Ĩȥ���

void game1_1();	//��һ����Ϸ����
void game1_2();	//�ڶ�����Ϸ����
void game1_3();	//��������Ϸ����
void game1_4();	//���Ĺ���Ϸ����
void game1_5();	//�������Ϸ����
void game1_6();	//��������Ϸ����

void init_game();		//��ʼ����Ϸ����
void init_tower();		//�ÿշ���������
void remove_tower();	//�ÿ�ָ������������
void init_monster();	//�ÿ�Ұ������
void init_flag();		//�ÿ����������

void write_tower();		//��¼����������
int is_tower();			//�жϸ�λ�����Ƿ��ѷ��÷�����
int is_tower_ud(uchar x);	//�ж�Ұ�� ��/�� �Ƿ��з�����
int is_tower_l(uchar x,uchar y);	//�ж�Ұ�� �� �Ƿ��з�����

#endif