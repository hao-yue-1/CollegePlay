#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//��ʼ��������
Status InitList(LinkedList* L)
{
	*L = (LinkedList)malloc(sizeof(LNode));
	if (!*L)
		exit(1);
	(*L)->next = NULL;
	return SUCCESS;
}

//������
Status InsertList(LinkedList L, int year, int month, int day, char event[51])
{
	if (!L)
	{
		//printf("����δ��ʼ�������ʼ��\n");
		return ERROR;
	}
	//Ϊ���q��ֵ
	LNode *q = (LNode*)malloc(sizeof(LNode));
	if (!q)
		exit(1);
	q->year = year;
	q->month = month;
	q->day = day;
	strcpy_s(q->event, 51, event);
	q->next = NULL;
	//������q
	while (L->next)
	{
		L = L->next;
	}
	L->next = q;

	return SUCCESS;
}

//ɾ�����
Status DeleteList(LinkedList L, int year, int month, int day)
{
	if (!L)
	{
		//printf("����δ��ʼ�������ʼ��\n");
		return ERROR;
	}
	if (!(L->next))
	{
		//printf("����Ϊ��\n");
		return ERROR;
	}

	char flag = 0;
	LNode* p = L->next, *ph = L;	//p������ƥ���ɾ����� ph��p��ǰ��
	while (p)
	{
		if (p->year == year && p->month == month && p->day == day)	//p�Ǵ�ɾ����㣬����ɾ��������ѭ��
		{
			ph->next = p->next;
			p->next = NULL;
			free(p);
			flag = 1;
			break;
		}
		ph = p;
		p = p->next;
	}
	if (flag == 0)
	{
		//printf("�Ҳ������ճ���Ϣ\n");
		return ERROR;
	}
	return SUCCESS;
}

void visit(LNode* p)
{
	printf("%d��%d��%d�յ��ճ̰��ţ�%s\n", p->year, p->month, p->day, p->event);
}

//�������
void TraverseList(LinkedList L, void (*visit)(LNode *p))
{
	if (!L)
	{
		printf("����δ��ʼ�������ʼ��\n");
		return;
	}

	L = L->next;
	int i = 1;		//Ϊ���������Ľ��ӱ��
	while (L)
	{
		printf("%d. ", i);	
		i++;
		visit(L);
		L = L->next;
	}
	printf("\n");
}
