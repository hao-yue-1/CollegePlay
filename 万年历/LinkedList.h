#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

//����һ��ѧ�����
typedef struct Node
{
	int year;			//��
	int month;			//��
	int day;			//��
	char event[51];		//�ճ��¼�
	struct Node* next;	//��һ�����ָ��
}LNode, * LinkedList;

typedef enum Status
{
	ERROR,
	SUCCESS
}Status;

//���������
Status InitList(LinkedList* L);													//��ʼ��������
Status InsertList(LinkedList L, int year, int month, int day, char event[51]);	//������
Status DeleteList(LinkedList L, int year, int month, int day);					//ɾ�����
void visit(LNode* p);															//����������
void TraverseList(LinkedList L, void (*visit)(LNode* p));

#endif