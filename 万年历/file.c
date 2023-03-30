#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "LinkedList.h"

//从文件中加载数据
/*
*********************************************************************************************************
*	函 数 名: load
*	功能说明: 1. 从文件中加载数据，若目录下不存在该文件，则新建一个文件用于保存数据
*             2. 对单链表的结点数据进行保存，保存到一个二进制文件中（.bin）
*	形    参: L：单链表，用于保存从文件中加载出来的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void load(LinkedList L)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "calendar.bin", "rb");		//rb；只读
	if (err)		//没有文件则创建一个文件
	{
		err = fopen_s(&fp, "calendar.bin", "wb");	//wb：只写，新建
		if (!fp)
			exit(1);
		fclose(fp);
	}
	else
	{
		if (!fp)
			exit(1);

		LNode* p = L;
		LNode* q = (LNode*)malloc(sizeof(LNode));
		if (!q)
			exit(1);
		//fread()：读取fp数据到q
		while (fread(q, sizeof(LNode), 1, fp))
		{
			p->next = q;	//把q插入在p后面
			p = p->next;
			q = q->next;
			q = (LNode*)malloc(sizeof(LNode));
			if (!q)
				exit(1);
		}
		fclose(fp);
	}
}

//保存数据到文件
/*
*********************************************************************************************************
*	函 数 名: load
*	功能说明: 1. 把单链表的数据保存到文件中，该函数必须在load调用后才能调用，以保证目标文件的存在
*             2. 从头到尾重新保存一遍单链表节点的数据，将旧数据覆盖
*	形    参: L：单链表，将该单链表的数据保存到文件中
*	返 回 值: 无
*********************************************************************************************************
*/
void save(LinkedList L)
{
	FILE* fp;
	errno_t err;

	err = fopen_s(&fp, "calendar.bin", "wb");
	if (!fp)
		exit(1);
	LNode* p = L->next;
	while (p)
	{
		fwrite(p, sizeof(LNode), 1, fp);
		p = p->next;
	}
	fclose(fp);
}

