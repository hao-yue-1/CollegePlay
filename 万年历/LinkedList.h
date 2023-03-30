#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

//定义一个学生结点
typedef struct Node
{
	int year;			//年
	int month;			//月
	int day;			//日
	char event[51];		//日程事件
	struct Node* next;	//下一个结点指针
}LNode, * LinkedList;

typedef enum Status
{
	ERROR,
	SUCCESS
}Status;

//单链表操作
Status InitList(LinkedList* L);													//初始化单链表
Status InsertList(LinkedList L, int year, int month, int day, char event[51]);	//插入结点
Status DeleteList(LinkedList L, int year, int month, int day);					//删除结点
void visit(LNode* p);															//遍历单链表
void TraverseList(LinkedList L, void (*visit)(LNode* p));

#endif