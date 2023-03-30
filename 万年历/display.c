#include "display.h"
#include <stdio.h>
#include <windows.h>

//隐藏光标
void print_hide()
{
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 }; //设置光标的厚度与可见的结构体
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//选择字体颜色
void print_color(short x)
{	//几种常用的：4红，5紫，6黄，9蓝，10绿，11青蓝
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);//x为0-15表示字体色
}

//将光标移动到X,Y坐标处
void print_toxy(int x, int y)
{
    //一般窗口居中效果：（50,12）
    COORD pos = { x , y };
    HANDLE Out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(Out, pos);
}

//初始化交互界面
void display()
{
    print_toxy(50, 8);
    printf("| 1：年份查询 |\n");
    print_toxy(50, 10);
    printf("| 2：月份查询 |\n");
    print_toxy(50, 12);
    printf("| 3：日期查询 |\n");
    print_toxy(50, 14);
    printf("| 4：日程管理 |\n");
    print_toxy(50, 16); 
    printf("| 0：退出程序 |\n");
}

//日程管理菜单
void display_4()
{
    print_toxy(50, 10);
    printf("| 1：添加新的日程 |\n");
    print_toxy(50, 12);
    printf("| 2：删除已有日程 |\n");
    print_toxy(50, 14);
    printf("| 3：查看所有日程 |\n");
    print_toxy(50, 16);
    printf("| 0：返回上一界面 |\n");
}