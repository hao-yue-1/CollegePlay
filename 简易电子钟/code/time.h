#ifndef _TIME_H
#define _TIME_H

//时钟结构体
typedef struct
{
	int hour;		//时
	int minute;		//分
	int seconds;	//秒
	char flag;		//闹钟标志	1.设置了闹钟    2.没有设置闹钟
}MyTime;

extern MyTime my_time;		//普通时钟
extern MyTime alarm_time;	//闹钟

extern unsigned int cnt;	//定时器0中的计数器
extern char flag;			//刷新时间显示标志	1.显示刷新	2.不刷新显示

void InitTimer0();												//初始化定时器0
void InitMyTime(int hour,int minute,int seconds);				//初始化时钟
void InitAlarmTime(int hour,int minute,int seconds,int flag);	//初始化闹钟

void ModifyTimeInterrupt();	//修改时间（中断专用）	

#endif