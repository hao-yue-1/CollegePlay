#ifndef _INFORMATION_H
#define _INFORMATION_H

#include "LCD12864.h"

void money_num(uint8 money);	//���½�Ǯ
void hp_num(uint8 hp);			//���»�������ֵ
void round_num(uint8 round);	//���µ���ʣ���������
void init_information();		//��ʼ����Ϸ��Ϣ

void init_recover();			//�����Ѫ����
void init_bigtower();			//�����������
void init_smalltower();			//���С������
void init_remotetower();		//���Զ��������
void init_remove();				//����Ƴ�����
void init_magic();				//���������

#endif