#ifndef _MENU_H
#define _MENU_H

//按键返回值定义
#define ENTER_PRES  1
#define UP_PRES     2
#define DOWN_PRES   3
#define LEFT_PRES   4
#define RIGHT_PRES  5
#define RETURN_PRES 6

int KeyScan();			//按键扫描
void Menu0();			//主菜单
void SetAlarmClock();	//设置闹钟
void SetClock();		//设置时钟

#endif