#include "calendar.h"
#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "display.h"

/*
*********************************************************************************************************
*	函 数 名: IsYear
*	功能说明: 判断输入的年份是闰年还是平年
*	形    参: year ：年份
*	返 回 值: 1. 闰年
*			  0. 平年
*********************************************************************************************************
*/
Status IsYear(int year)
{
	//1. 普通年能被4整除，但不能被100整除是闰年
	//2. 世纪年能被400整除是闰年
	//总结：能被4整除且不能被100整除或能被400整除的是闰年
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return SUCCESS;		//闰年
	else
		return ERROR;		//平年
}

/*
*********************************************************************************************************
*	函 数 名: SumDayLater
*	功能说明: 计算输入的日期距1900/01/01的天数（该日期位于1900/01/01之后）
*	形    参: 1. year ：年
*			  2. month：月 [1,12]
*			  3. day  ：日 [1,31]
*	返 回 值: 当前日期距1900/01/01的天数（正数）；返回值类型为long long以支持尽量大的年份查询
*********************************************************************************************************
*/
long long SumDayLater(int year, int month, int day)
{
	long long day_y = 0;
	int day_m = 0, temp;
	//计算day_y
	day_y = (year - 1900) * 365;
	temp = (year - 1900) / 4;
	if ((year - 1900) % 4 == 0 && year != 1900)
		day_y += temp * 1 - 1;
	else 
		day_y += temp * 1;
	//计算day_m
	switch (month)
	{
		case 12: day_m += 30;
		case 11: day_m += 31;
		case 10: day_m += 30;
		case 9 : day_m += 31;
		case 8 : day_m += 31;
		case 7 : day_m += 30;
		case 6 : day_m += 31;
		case 5 : day_m += 30;
		case 4 : day_m += 31;
		case 3 : {
			if (IsYear(year) == 1)
				day_m += 29;
			else
				day_m += 28;
		}
		case 2 : day_m += 31;
		case 1 : break;
	}
	//计算总的天数
	return day_y + day_m + day - 1;
}

/*
*********************************************************************************************************
*	函 数 名: SumDayBefore
*	功能说明: 计算输入的日期距1900/01/01的天数（该日期位于1900/01/01之前）
*	形    参: 1. year ：年
*			  2. month：月 [1,12]
*			  3. day  ：日 [1,31]
*	返 回 值: 当前日期距1900/01/01的天数；返回值类型为long long以支持尽量大的年份查询
*********************************************************************************************************
*/
long long SumDayBefore(int year, int month, int day)
{	//以1899/12/31为基准（-1）
	long long day_y = 0;
	int day_m = 0, temp;
	//计算day_y
	day_y = (1900 - year) * 365;
	temp = (1900 - year) / 4;
	day_y += temp * 1 - 1;
	//计算day_m
	switch (month)
	{
		case 12: day_m += 30;
		case 11: day_m += 31;
		case 10: day_m += 30;
		case 9: day_m += 31;
		case 8: day_m += 31;
		case 7: day_m += 30;
		case 6: day_m += 31;
		case 5: day_m += 30;
		case 4: day_m += 31;
		case 3: {
			if (IsYear(year) == 1)
				day_m += 29;
			else
				day_m += 28;
		}
		case 2: day_m += 31;
		case 1: break;
	}
	//计算总的天数
	return day_y - day_m - day + 2;
}

/*
*********************************************************************************************************
*	函 数 名: WeekDay
*	功能说明: 计算输入的日期是周几
*	形    参: 1. year ：年
*			  2. month：月 [1,12]
*			  3. day  ：日 [1,31]
*	返 回 值: 0. 0：周日
*			  1. 1：周一
*			  2. 2：周二
*			  3. 3：周三
*			  4. 4：周四
*			  5. 5：周五
*			  6. 6：周六
*********************************************************************************************************
*/
int WeekDay(int year, int month, int day)
{
	int temp;
	if (year >= 1900)
	{
		temp = SumDayLater(year, month, day) % 7;
	}
	else
	{
		temp = SumDayBefore(year, month, day) % 7;
		if(temp != 0)
			temp = 7 - temp;
		/*
		*temp计算表 
		0 1  0
		1 0  6
		2 6  5
		3 5  4
		4 4  3
		5 3  2
		6 2  1
		*/
	}
	switch (temp)
	{
		case 0: return 1;	break;
		case 1: return 2;	break;
		case 2: return 3;	break;
		case 3: return 4;	break;
		case 4: return 5;	break;
		case 5: return 6;	break;
		case 6: return 0;	break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: PrintMonth
*	功能说明: 打印输入的某年某月的日历
*	形    参: 1. year ：年
*			  2. month：月 [1,12]
*	返 回 值: 打印成功返回1
*********************************************************************************************************
*/
Status PrintMonth(int year,int month)
{	
	int week, day;
	week = WeekDay(year, month, 1);	//该月第一天的星期
	//计算该月的天数
	if (month == 2)								
	{
		if (IsYear(year) == 1)
			day = 29;	//闰年二月29天
		else
			day = 28;	//平年二月28天
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11)
		day = 30;		//小月30天
	else
		day = 31;		//大月31天
	//打印日历模板
	printf("---------------------------------------------------\n");
	printf("\t\t  %d年\t%d月\n", year, month);
	printf("---------------------------------------------------\n");
	printf("日\t一\t二\t三\t四\t五\t六\n");
	//打印日历信息
	int month_day, month_week, flag = 1;
	for (month_day = 1, month_week = week; day > 0; month_day++, day--, month_week++)
	{
		//打印每一天的信息
		if (flag == 1)		//确定该月第一天在日历中的位置
		{
			int cnt = month_week;
			while (cnt--)	//把第一天调整到合适的位置
			{
				printf("\t");
			}
			flag = 0;	//标记第一天已经调整过
		}
		printf("%d\t", month_day);	//打印每一天的信息
		//打印一周后换行（在周六之后换行）
		if (month_week == 6)
		{
			printf("\n");
			month_week = -1;	//+1=0，重新在周日开始
		}
	}
	printf("\n");
	return SUCCESS;
}

/*
*********************************************************************************************************
*	函 数 名: PrintYear
*	功能说明: 打印输入的年份的日历
*	形    参: year ：年
*	返 回 值: 打印成功返回1
*********************************************************************************************************
*/
Status PrintYear(int year)
{
	int month = 0;	//打印12个月
	for (month = 1; month <= 12; month++)
	{
		PrintMonth(year, month);
		printf("\n");
	}
	return SUCCESS;
}

/*
*********************************************************************************************************
*	函 数 名: AddEvent
*	功能说明: 在指定日期添加日程
*	形    参: 无
*	返 回 值: 1. SUCCESS：添加成功
*			  2. ERROR  ：添加失败（1.该日期已有日程 2.输入日期不合法）
*********************************************************************************************************
*/
Status AddEvent(LinkedList L)
{
	int year=-1, month=-1, day=-1;	//提前赋值原理同main函数处注释
	char event[51] = { '\0' };
	//输入年月日
	print_toxy(48, 12);
	printf("依次输入年月日：");
	scanf_s("%d %d %d", &year, &month, &day);
	while (getchar() != '\n');  //输入容错，吃掉非法字符
	//输入日期范围判断
	if (year < 0|| month < 1 || month>12||day < 1 || day>31)
	{
		print_toxy(48, 14);
		printf("输入日期范围有误！！！\n");
		return ERROR;	//输入日期不合法返回ERROR
	}
	//输入日程事件
	print_toxy(48, 14);
	printf("输入要添加的日程（不多于25字）：");
	scanf_s("%s", event, 51);
	//插入结点
	char flag = InsertList(L,year, month, day, event);
	save(L);	//保存数据
	
	if (!flag)
		return ERROR;	//结点插入失败
	else
		return SUCCESS;
}

/*
*********************************************************************************************************
*	函 数 名: DeleteEvent
*	功能说明: 在指定日期删除日程
*	形    参: num：指定删除的日程的序号
*	返 回 值: 1. SUCCESS：删除成功
*			  2. ERROR  ：删除失败（该序号不合法）
*********************************************************************************************************
*/
Status DeleteEvent(LinkedList L, int num)
{
	//根据序号找到待删除结点
	LNode* p = L;
	while (num--)
	{
		p = p->next;
		if (!p)
		{
			printf("输入序号范围有误！！！\n");
			return ERROR;
		}
	}
	//删除结点
	DeleteList(L, p->year, p->month, p->day);
	save(L);	//保存数据

	return SUCCESS;
}
