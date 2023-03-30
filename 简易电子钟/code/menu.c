#include "menu.h"
#include <reg51.h>
#include "display.h"
#include "time.h"
#include "eeprom.h"

extern MyTime my_time;			//时钟
extern MyTime alarm_time;		//闹钟
extern char flag;

//按键定义
sbit key_enter=P1^4;	//确认键
sbit key_up=P1^0;		//向上键
sbit key_down=P1^1;		//向下键
sbit key_left=P1^2;		//向左键
sbit key_right=P1^3;	//向右键
sbit key_return=P1^5;	//返回键

int cursor=0;		//功能光标	0.显示  1.时钟	2.闹钟
int cursor_num=3;	//数字光标  1.时 2.分 3.秒
int key=0;

//按键扫描
/*
*********************************************************************************************************
*	函 数 名: KeyScan
*	功能说明: 获取用户按下的按键信号，并返回对应的按键标志
*	形    参: 无
*	返 回 值: 1. ENTER_PRES : 确认键
*			  2. UP_PRES    : 向上键
*			  3. DOWN_PRES  : 向下键
*			  4. LEFT_PRES  : 向左键
*			  5. RIGHT_PRES : 向右键
*			  6. RETURN_PRES: 返回键
*********************************************************************************************************
*/
int KeyScan()
{
	unsigned char key_flag=1;     //按键松开标志

	//ModifyTime();
	if(key_flag==1&&(key_enter==0||key_up==0||key_down==0||key_left==0||key_right==0||key_return==0))
    {
		/*
		* 把这个保存数据的功能放在这里而非主循环中是因为：该函数的缺点为运行时间较长，
		* 若放在主循环中会影响到单片机对按键的响应速度，导致有时候出现按键不响应的情况，
		* 放在这里既可以消除该影响，还能利用该函数的运行时间再配合延时函数做一个简单的按键消抖
		* （若不调用该函数，则延时应为delay(200)）
		*/
        delay(130);			//消抖
		WriteTimeEEPROM();	//把数据写进EEPROM中
        key_flag=0;
        if(key_enter==0)       	return ENTER_PRES;	//确认键
		else if(key_return==0)	return RETURN_PRES;	//返回键
        else if(key_up==0)  	return UP_PRES;		//向上键
        else if(key_down==0) 	return DOWN_PRES;   //向下键
		else if(key_left==0)  	return LEFT_PRES;	//向左键
        else if(key_right==0) 	return RIGHT_PRES;	//向右键
	}
	else if(key_enter==0&&key_up==0&&key_down==0&&key_left==0&&key_right==0&&key_return==0)
	{
		key_flag=1;
	}
	/*
	* 由于按键并非时刻有被按下，也意味着数据并非真正的时刻保存，
	* 因此除了充当消抖的延时函数外，再在无按键按下的情况进行调用，
	* 实测不会影响到按键的响应问题，而数据的保存也是实时的
	*/
	WriteTimeEEPROM();	//把数据写进EEPROM中
    return 0;   //无按键按下
}

//主菜单
/*
*********************************************************************************************************
*	函 数 名: Menu0
*	功能说明: 通过获取用户按下的按键标志，实现对应的功能（确认键：进入对应功能，向左/右键：移动光标）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Menu0()
{
	key=KeyScan();	//获取用户按下的按键
	switch(key)	//判断按下的是哪个按键
	{
		case ENTER_PRES:{	//确认键
			switch(cursor)		//判断按下确认键时光标在哪个功能
			{
				case 0:{		//时钟显示
					flag=1;
					//显示时间
					PrintTimeHour(my_time.hour);
					PrintTimeMinute(my_time.minute);
					PrintTimeSeconds(my_time.seconds);
					//有时候LCD屏出现花屏，可以通过不断点击该功能进行刷新
					PrintColon();
					PrintCursor(cursor);			//在LCD上显示光标
					PrintClock(alarm_time.flag);	//在LCD上显示闹钟
					PrintMode(cursor);				//在LCD上显示时钟显示
					break;
				}
				case 1:{		//时钟调整	
					flag=0;
					PrintMode(cursor);	//在LCD上显示时钟调整
					PrintColon();
					SetClock();			//调整时钟
					break;
				}
				case 2:{		//闹钟模调整
					flag=0;
					PrintMode(cursor);	//在LCD上显示闹钟调整
					PrintColon();
					SetAlarmClock();	//调整闹钟
					break;
				}
				default:break;
			}
			break;
		}
		case LEFT_PRES:{	//向左
			if(cursor>0)
			{
				cursor--;
				PrintCursor(cursor);
			}
			break;
		}
		case RIGHT_PRES:{	//向右
			if(cursor<2)
			{
				cursor++;
				PrintCursor(cursor);
			}
			break;
		}
		default:break;		//向上、下，返回不给予反应
	}
}

//设置闹钟
/*
*********************************************************************************************************
*	函 数 名: SetAlarmClock
*	功能说明: 不断循环获取用户按下的按键标志（按下确认键退出循环），修改闹钟的数值，被选中的参数会反向显示
*             以提示用户
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetAlarmClock()
{
	//获取旧闹钟的值并在LCD上显示
	int hour=alarm_time.hour;
	int minute=alarm_time.minute;
	int seconds=alarm_time.seconds;	
	//反向显示选中的参数
	switch(cursor_num)
	{
		case 1: {
			PrintTimeHourReverse(hour);
			PrintTimeMinute(minute);
			PrintTimeSeconds(seconds);
			break;
		}
		case 2: {
			PrintTimeHour(hour);
			PrintTimeMinuteReverse(minute);
			PrintTimeSeconds(seconds);
			break;
		}
		case 3: {
			PrintTimeHour(hour);
			PrintTimeMinute(minute);
			PrintTimeSecondsReverse(seconds);
			break;
		}
	}
	//进行按键扫描，获取闹钟值
	while(1)
	{
		key=KeyScan();	//获取用户按下的按键
		switch(key)
		{
			case ENTER_PRES:{	//确认
				PrintSetOK(1);	//在LCD上显示闹钟设置成功
				//退出按键扫描后，为闹钟赋值
				alarm_time.hour=hour;
				alarm_time.minute=minute;
				alarm_time.seconds=seconds;	
				alarm_time.flag=1;			//开启闹钟
				PrintClock(alarm_time.flag);
				return;
			}
			case RETURN_PRES:{	//返回
				PrintReturn(1);			//在LCD上显示退出闹钟调整
				alarm_time.flag=0;		//关闭闹钟
				PrintClock(alarm_time.flag);
				return;
			}
			case UP_PRES:{		//向上
				//调整参数：+1
				switch(cursor_num)
				{
					case 1: {
						hour++;
						if(hour==24)
							hour=0;
						PrintTimeHourReverse(hour);
						break;
					}
					case 2: {
						minute++;
						if(minute==60)
							minute=0;
						PrintTimeMinuteReverse(minute);
						break;
					}
					case 3: {
						seconds++;
						if(seconds==60)
							seconds=0;
						PrintTimeSecondsReverse(seconds);
						break;
					}
					default:break;
				}
				break;
			}
			case DOWN_PRES:{	//向下
				//调整参数：-1
				switch(cursor_num)
				{
					case 1: {
						hour--;
						if(hour==-1)
							hour=23;
						PrintTimeHourReverse(hour);
						break;
					}
					case 2: {
						minute--;
						if(minute==-1)
							minute=59;
						PrintTimeMinuteReverse(minute);
						break;
					}
					case 3: {
						seconds--;
						if(seconds==-1)
							seconds=59;
						PrintTimeSecondsReverse(seconds);
						break;
					}
					default:break;
				}
				break;
			}
			case LEFT_PRES:{	//向左
				if(cursor_num>1)
					cursor_num--;
				//反向显示选中的参数
				switch(cursor_num)
				{
					case 1: {
						PrintTimeHourReverse(hour);
						PrintTimeMinute(minute);
						PrintTimeSeconds(seconds);
						break;
					}
					case 2: {
						PrintTimeHour(hour);
						PrintTimeMinuteReverse(minute);
						PrintTimeSeconds(seconds);
						break;
					}
					case 3: {
						PrintTimeHour(hour);
						PrintTimeMinute(minute);
						PrintTimeSecondsReverse(seconds);
						break;
					}
				}
				break;
			}
			case RIGHT_PRES:{	//向右
				if(cursor_num<3)
					cursor_num++;
				//反向显示选中的参数
				switch(cursor_num)
				{
					case 1: {
						PrintTimeHourReverse(hour);
						PrintTimeMinute(minute);
						PrintTimeSeconds(seconds);
						break;
					}
					case 2: {
						PrintTimeHour(hour);
						PrintTimeMinuteReverse(minute);
						PrintTimeSeconds(seconds);
						break;
					}
					case 3: {
						PrintTimeHour(hour);
						PrintTimeMinute(minute);
						PrintTimeSecondsReverse(seconds);
						break;
					}
				}
				break;
			}
			default:break;
		}
	}
}

//设置时钟
/*
*********************************************************************************************************
*	函 数 名: SetClock
*	功能说明: 不断循环获取用户按下的按键标志（按下确认键退出循环），修改时钟的数值，被选中的参数会反向显示
*             以提示用户
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetClock()
{
	//获取旧时钟的值并在LCD上显示
	int hour=my_time.hour;
	int minute=my_time.minute;
	int seconds=my_time.seconds;	
	//反向显示选中的参数
	switch(cursor_num)
	{
		case 1: {
			PrintTimeHourReverse(hour);
			PrintTimeMinute(minute);
			PrintTimeSeconds(seconds);
			break;
		}
		case 2: {
			PrintTimeHour(hour);
			PrintTimeMinuteReverse(minute);
			PrintTimeSeconds(seconds);
			break;
		}
		case 3: {
			PrintTimeHour(hour);
			PrintTimeMinute(minute);
			PrintTimeSecondsReverse(seconds);
			break;
		}
	}
	//进行按键扫描，获取闹钟值
	while(1)
	{
		key=KeyScan();	//获取用户按下的按键
		switch(key)
		{
			case ENTER_PRES:{	//确认
				PrintSetOK(0);	//在LCD上显示时钟设置成功
				//退出按键扫描后，为时钟赋值
				my_time.hour=hour;
				my_time.minute=minute;
				my_time.seconds=seconds;
				cnt=0;	//把定时器的计数器置0
				return;
			}
			case RETURN_PRES:{	//返回
				PrintReturn(0);	//在LCD上显示退出时钟调整
				return;
			}
			case UP_PRES:{		//向上
				//调整参数：+1
				switch(cursor_num)
				{
					case 1: {
						hour++;
						if(hour==24)
							hour=0;
						PrintTimeHourReverse(hour);
						break;
					}
					case 2: {
						minute++;
						if(minute==60)
							minute=0;
						PrintTimeMinuteReverse(minute);
						break;
					}
					case 3: {
						seconds++;
						if(seconds==60)
							seconds=0;
						PrintTimeSecondsReverse(seconds);
						break;
					}
					default:break;
				}
				break;
			}
			case DOWN_PRES:{	//向下
				//调整参数：-1
				switch(cursor_num)
				{
					case 1: {
						hour--;
						if(hour==-1)
							hour=23;
						PrintTimeHourReverse(hour);
						break;
					}
					case 2: {
						minute--;
						if(minute==-1)
							minute=59;
						PrintTimeMinuteReverse(minute);
						break;
					}
					case 3: {
						seconds--;
						if(seconds==-1)
							seconds=59;
						PrintTimeSecondsReverse(seconds);
						break;
					}
					default:break;
				}
				break;
			}
			case LEFT_PRES:{	//向左
				if(cursor_num>1)
					cursor_num--;
				//反向显示选中的参数
				switch(cursor_num)
				{
					case 1: {
						PrintTimeHourReverse(hour);
						PrintTimeMinute(minute);
						PrintTimeSeconds(seconds);
						break;
					}
					case 2: {
						PrintTimeHour(hour);
						PrintTimeMinuteReverse(minute);
						PrintTimeSeconds(seconds);
						break;
					}
					case 3: {
						PrintTimeHour(hour);
						PrintTimeMinute(minute);
						PrintTimeSecondsReverse(seconds);
						break;
					}
				}
				break;
			}
			case RIGHT_PRES:{	//向右
				if(cursor_num<3)
					cursor_num++;
				//反向显示选中的参数
				switch(cursor_num)
				{
					case 1: {
						PrintTimeHourReverse(hour);
						PrintTimeMinute(minute);
						PrintTimeSeconds(seconds);
						break;
					}
					case 2: {
						PrintTimeHour(hour);
						PrintTimeMinuteReverse(minute);
						PrintTimeSeconds(seconds);
						break;
					}
					case 3: {
						PrintTimeHour(hour);
						PrintTimeMinute(minute);
						PrintTimeSecondsReverse(seconds);
						break;
					}
				}
				break;
			}
			default:break;
		}
	}
}


