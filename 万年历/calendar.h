#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "LinkedList.h"

//typedef enum Status
//{
//	ERROR,
//	SUCCESS
//}Status;

//����
Status IsYear(int year);								//�жϸ���������껹��ƽ��
long long SumDayLater(int year, int month, int day);	//������ھ�1900.01.01��������������λ��1900.01.01��
long long SumDayBefore(int year, int month, int day);	//������ھ�1900.01.01��������������λ��1900.01.01ǰ��
int WeekDay(int year, int month, int day);				//������������ڼ�
Status PrintYear(int year);								//��ӡ����
Status PrintMonth(int year, int month);					//��ӡ����

//�ճ�
Status AddEvent(LinkedList L);				//����µ��ճ�
Status DeleteEvent(LinkedList L, int num);	//ɾ�������ճ�


#endif