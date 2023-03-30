#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "LinkedList.h"

//typedef enum Status
//{
//	ERROR,
//	SUCCESS
//}Status;

//日历
Status IsYear(int year);								//判断该年份是闰年还是平年
long long SumDayLater(int year, int month, int day);	//求该日期距1900.01.01的天数（该日期位于1900.01.01后）
long long SumDayBefore(int year, int month, int day);	//求该日期距1900.01.01的天数（该日期位于1900.01.01前）
int WeekDay(int year, int month, int day);				//求该日期是星期几
Status PrintYear(int year);								//打印年历
Status PrintMonth(int year, int month);					//打印月历

//日程
Status AddEvent(LinkedList L);				//添加新的日程
Status DeleteEvent(LinkedList L, int num);	//删除已有日程


#endif