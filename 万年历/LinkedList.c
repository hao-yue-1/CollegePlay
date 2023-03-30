#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//初始化单链表
Status InitList(LinkedList* L)
{
	*L = (LinkedList)malloc(sizeof(LNode));
	if (!*L)
		exit(1);
	(*L)->next = NULL;
	return SUCCESS;
}

//插入结点
Status InsertList(LinkedList L, int year, int month, int day, char event[51])
{
	if (!L)
	{
		//printf("链表未初始化，请初始化\n");
		return ERROR;
	}
	//为结点q赋值
	LNode *q = (LNode*)malloc(sizeof(LNode));
	if (!q)
		exit(1);
	q->year = year;
	q->month = month;
	q->day = day;
	strcpy_s(q->event, 51, event);
	q->next = NULL;
	//插入结点q
	while (L->next)
	{
		L = L->next;
	}
	L->next = q;

	return SUCCESS;
}

//删除结点
Status DeleteList(LinkedList L, int year, int month, int day)
{
	if (!L)
	{
		//printf("链表未初始化，请初始化\n");
		return ERROR;
	}
	if (!(L->next))
	{
		//printf("链表为空\n");
		return ERROR;
	}

	char flag = 0;
	LNode* p = L->next, *ph = L;	//p：遍历匹配待删除结点 ph：p的前驱
	while (p)
	{
		if (p->year == year && p->month == month && p->day == day)	//p是待删除结点，进行删除并跳出循环
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
		//printf("找不到该日程信息\n");
		return ERROR;
	}
	return SUCCESS;
}

void visit(LNode* p)
{
	printf("%d年%d月%d日的日程安排：%s\n", p->year, p->month, p->day, p->event);
}

//遍历结点
void TraverseList(LinkedList L, void (*visit)(LNode *p))
{
	if (!L)
	{
		printf("链表未初始化，请初始化\n");
		return;
	}

	L = L->next;
	int i = 1;		//为遍历出来的结点加编号
	while (L)
	{
		printf("%d. ", i);	
		i++;
		visit(L);
		L = L->next;
	}
	printf("\n");
}
