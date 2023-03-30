#include "display.h"
#include <stdio.h>
#include <windows.h>

//���ع��
void print_hide()
{
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 }; //���ù��ĺ����ɼ��Ľṹ��
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//ѡ��������ɫ
void print_color(short x)
{	//���ֳ��õģ�4�죬5�ϣ�6�ƣ�9����10�̣�11����
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);//xΪ0-15��ʾ����ɫ
}

//������ƶ���X,Y���괦
void print_toxy(int x, int y)
{
    //һ�㴰�ھ���Ч������50,12��
    COORD pos = { x , y };
    HANDLE Out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(Out, pos);
}

//��ʼ����������
void display()
{
    print_toxy(50, 8);
    printf("| 1����ݲ�ѯ |\n");
    print_toxy(50, 10);
    printf("| 2���·ݲ�ѯ |\n");
    print_toxy(50, 12);
    printf("| 3�����ڲ�ѯ |\n");
    print_toxy(50, 14);
    printf("| 4���ճ̹��� |\n");
    print_toxy(50, 16); 
    printf("| 0���˳����� |\n");
}

//�ճ̹���˵�
void display_4()
{
    print_toxy(50, 10);
    printf("| 1������µ��ճ� |\n");
    print_toxy(50, 12);
    printf("| 2��ɾ�������ճ� |\n");
    print_toxy(50, 14);
    printf("| 3���鿴�����ճ� |\n");
    print_toxy(50, 16);
    printf("| 0��������һ���� |\n");
}