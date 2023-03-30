#ifndef _DISPLAY_H
#define _DISPLAY_H

void delay(unsigned int x);	//简单的延时函数

void PrintCursor(int cursor);		//在LCD上移动光标
void PrintMode(int mode);			//在LCD上显示模式

void PrintTimeHour(int hour);					//在LCD中央显示时间（时）
void PrintTimeMinute(int minute);				//在LCD中央显示时间（分）
void PrintTimeSeconds(int seconds); 			//在LCD中央显示时间（秒）
void PrintTimeHourReverse(int hour);			//在LCD中央显示时间（时）（反向）
void PrintTimeMinuteReverse(int minute);		//在LCD中央显示时间（分）（反向）
void PrintTimeSecondsReverse(int seconds); 		//在LCD中央显示时间（秒）（反向）
void PrintTimeHourInterrupt(int hour);			//在LCD中央显示时间（时）（中断专用）
void PrintTimeMinuteInterrupt(int minute);		//在LCD中央显示时间（分）（中断专用）
void PrintTimeSecondsInterrupt(int seconds); 	//在LCD中央显示时间（秒）（中断专用）

void PrintSetOK(int mode);				//在LCD上显示设置成功
void PrintColon();						//在LCD上显示冒号（:）
void PrintClock(int flag);				//在LCD上显示闹钟图标
void PrintClockInterrupt(int flag);		//在LCD上显示闹钟图标（中断专用）
void PrintReturn(int mode);				//在LCD上显示退出调整

void PrintClockNULLInterrupt(int minute);	//一个无效的延时

#endif