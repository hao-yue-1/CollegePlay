#include "checkpoint.h"
#include "game.h"
#include "select.h"
#include "menu.h"
#include "LCD12864.h"
#include <reg52.h>
#include "score.h"
#include "usart.h"
#include "information.h"

//第一关
void game1()
{
	init_beep();
	play_beep();	//播放音乐
	ClearScreen(0);	//清空屏幕
	Set_line(0);
	init_USART_receive();
	init_game();	//初始化游戏参数
	initex();		//开启中断，接收游戏按键的响应
	game1_1();		//进入游戏，野怪开始刷新
	play_beep();	//播放音乐
	EX0=0;			//关闭外部中断0
	EA=0;
	gameover();		//游戏结束界面
}

//第二关
void game2()
{
	init_beep();
	play_beep();	//播放音乐
	ClearScreen(0);	//清空屏幕
	Set_line(0);
	init_USART_receive();
	init_game();	//初始化游戏参数
	initex();		//开启中断，接收游戏按键的响应
	game1_2();		//进入游戏，野怪开始刷新
	play_beep();	//播放音乐
	EX0=0;			//关闭外部中断0
	EA=0;
	gameover();		//游戏结束界面
}

//第三关
void game3()
{
	init_beep();
	play_beep();	//播放音乐
	ClearScreen(0);	//清空屏幕
	Set_line(0);
	init_USART_receive();
	init_game();	//初始化游戏参数
	initex();		//开启中断，接收游戏按键的响应
	game1_3();		//进入游戏，野怪开始刷新
	play_beep();	//播放音乐
	EX0=0;			//关闭外部中断0
	EA=0;
	gameover();		//游戏结束界面
}

//第四关
void game4()
{
	init_beep();
	play_beep();	//播放音乐
	ClearScreen(0);	//清空屏幕
	Set_line(0);
	init_USART_receive();
	init_game();	//初始化游戏参数
	initex();		//开启中断，接收游戏按键的响应
	game1_4();		//进入游戏，野怪开始刷新
	play_beep();	//播放音乐
	EX0=0;			//关闭外部中断0
	EA=0;
	gameover();		//游戏结束界面
}

//第五关
void game5()
{
	init_beep();
	play_beep();	//播放音乐
	ClearScreen(0);	//清空屏幕
	Set_line(0);
	init_USART_receive();
	init_game();	//初始化游戏参数
	initex();		//开启中断，接收游戏按键的响应
	game1_5();		//进入游戏，野怪开始刷新
	play_beep();	//播放音乐
	EX0=0;			//关闭外部中断0
	EA=0;
	gameover();		//游戏结束界面
}

//第六关
void game6()
{
	init_beep();
	play_beep();	//播放音乐
	ClearScreen(0);	//清空屏幕
	Set_line(0);
	init_USART_receive();
	init_game();	//初始化游戏参数
	initex();		//开启中断，接收游戏按键的响应
	game1_6();		//进入游戏，野怪开始刷新
	play_beep();	//播放音乐
	EX0=0;			//关闭外部中断0
	EA=0;
	gameover();		//游戏结束界面
}