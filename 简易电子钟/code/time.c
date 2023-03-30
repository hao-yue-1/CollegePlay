#include "time.h"
#include <reg51.h>
#include "lcd12864.h"
#include "display.h"

sbit LED=P1^7;		//LED灯	//闹钟到点的提示

unsigned int cnt=0;			//定时器0中的计数器
unsigned int cnt_alarm=7;	//LED灯闪烁次数计数器
char flag=1;		//刷新时间显示标志


MyTime my_time;		//普通时钟
MyTime alarm_time;	//闹钟

//初始化时钟
void InitMyTime(int hour,int minute,int seconds)
{	
	InitTimer0();	//初始化定时器0
	//初始化时钟
	my_time.hour=hour;
	my_time.minute=minute;
	my_time.seconds=seconds;
	my_time.flag=0;
	//初始化LCD显示
	ClearScreen(0);
	Set_line(0);
	//时分秒
	PrintTimeHour(my_time.hour);
	PrintTimeMinute(my_time.minute);
	PrintTimeSeconds(my_time.seconds);
	//初始化显示
	PrintMode(0);
	PrintCursor(0);
	PrintColon();
}

//初始化闹钟
void InitAlarmTime(int hour,int minute,int seconds,int flag)
{
	//初始化闹钟
	alarm_time.hour=hour;
	alarm_time.minute=minute;
	alarm_time.seconds=seconds;
	alarm_time.flag=flag;
	//初始化显示
	PrintClock(alarm_time.flag);
	LED=1;					//关闭LED灯
}

//修改时间（中断专用）
/*
*********************************************************************************************************
*	函 数 名: ModifyTimeInterrupt
*	功能说明: 1. 对时钟的参数进行修改，实现时分秒的自加，以及进位
*			  2. 若到达闹钟以经开启且到达设定的时间，进行LED灯闪烁3次的提醒，并关闭闹钟 		
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ModifyTimeInterrupt()
{
	//秒
	my_time.seconds++;
	if(flag)
	{
		PrintTimeSecondsInterrupt(my_time.seconds);		//在LCD上更新秒
	}
	else	//无效延时
	{
		PrintClockNULLInterrupt(my_time.seconds);
	}
	//秒->分
	if(my_time.seconds==60)
	{
		my_time.minute++;
		my_time.seconds=0;
		if(flag)
		{
			PrintTimeMinuteInterrupt(my_time.minute);	//在LCD上更新分
			PrintTimeSecondsInterrupt(my_time.seconds);	//在LCD上更新秒
		}
	}
	//分->时
	if(my_time.minute==60)
	{
		my_time.hour++;
		my_time.minute=0;
		if(flag)
		{
			PrintTimeHourInterrupt(my_time.hour);		//在LCD上更新时
			PrintTimeMinuteInterrupt(my_time.minute);	//在LCD上更新分
		}
	}
	//时
	if(my_time.hour==24)
	{
		my_time.hour=0;
		if(flag)
		{
			PrintTimeHourInterrupt(my_time.hour);		//在LCD上更新时
		}
	}
	//判断闹钟是否到点
	if(alarm_time.flag==1&&alarm_time.hour==my_time.hour&&alarm_time.minute==my_time.minute&&alarm_time.seconds==my_time.seconds)	//闹钟是否到点
	{	
		cnt_alarm--;	//开启计数
	}
	//LED闪烁3次后关闭闹钟	//LED闪烁次数=(i的初始值-1)/2
	if(cnt_alarm<7)
	{
		LED=!LED;	
		cnt_alarm--;
		if(cnt_alarm==0)
		{
			cnt_alarm=7;		//计数器归零
			alarm_time.flag=0;	//关闭闹钟
			PrintClockInterrupt(alarm_time.flag);
		}
	}
}

//初始化定时器0
void InitTimer0()
{
    TMOD=0x01;		//0000 0001	//T0工作在方式1：16位定时器
    TH0=(65536-50200)/256;	//50000us	256=2^8
    TL0=(65536-50200)%256;	//取低8位
    ET0=1;	//开启定时器0的中断
    EA=1;	//开启总中断
    TR0=1;	//启动定时器0
}

/*
* 将修改时间放在中断中
* 50000:16  一分钟1.00.81  三分钟3.04.61  四分钟4.04.87
* 51000:16  一分钟1.04.63  四分钟4.09.81
* 50500:16  一分钟1.03.95  两分钟2.04.69  五分钟5.06.88
* 50200:16  一分钟1.03.13  两分钟2.03.45  五分钟5.04.59  （第一分钟会慢3秒，后面趋于准确）
*/

//定时器0中断执行函数
void Timer0_isr()	interrupt 1
{
    //进行初值重载
    TH0=(65536-50200)/256;
    TL0=(65536-50200)%256;
    //进入中断
	cnt++;
	//只有进行了一秒的周期之后才会进行一次数据处理
	if(cnt==16)	//决定了一秒的时间长度	//理论值为50000:20，根据实际情况调试
	{
		cnt=0;					//计数器归零
		ModifyTimeInterrupt();	//修改时间
	}
}