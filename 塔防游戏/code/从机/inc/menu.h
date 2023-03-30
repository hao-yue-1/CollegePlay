#ifndef _MENU_H
#define _MENU_H

#define uint8 unsigned char

extern uint8 score[6];	//记录6个关卡的分数

void menu0();	//开始菜单
void menu0_1();	//开始菜单——排行榜
void menu0_2();	//开始菜单——操作说明
void menu1();	//关卡选择菜单

void welcome();		//欢迎界面
void gameover();	//结束界面
void gamevictory();	//胜利界面
void gamefailure();	//失败界面

#endif