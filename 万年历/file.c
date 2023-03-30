#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "LinkedList.h"

//���ļ��м�������
/*
*********************************************************************************************************
*	�� �� ��: load
*	����˵��: 1. ���ļ��м������ݣ���Ŀ¼�²����ڸ��ļ������½�һ���ļ����ڱ�������
*             2. �Ե�����Ľ�����ݽ��б��棬���浽һ���������ļ��У�.bin��
*	��    ��: L�����������ڱ�����ļ��м��س���������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void load(LinkedList L)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "calendar.bin", "rb");		//rb��ֻ��
	if (err)		//û���ļ��򴴽�һ���ļ�
	{
		err = fopen_s(&fp, "calendar.bin", "wb");	//wb��ֻд���½�
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
		//fread()����ȡfp���ݵ�q
		while (fread(q, sizeof(LNode), 1, fp))
		{
			p->next = q;	//��q������p����
			p = p->next;
			q = q->next;
			q = (LNode*)malloc(sizeof(LNode));
			if (!q)
				exit(1);
		}
		fclose(fp);
	}
}

//�������ݵ��ļ�
/*
*********************************************************************************************************
*	�� �� ��: load
*	����˵��: 1. �ѵ���������ݱ��浽�ļ��У��ú���������load���ú���ܵ��ã��Ա�֤Ŀ���ļ��Ĵ���
*             2. ��ͷ��β���±���һ�鵥����ڵ�����ݣ��������ݸ���
*	��    ��: L�����������õ���������ݱ��浽�ļ���
*	�� �� ֵ: ��
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

