#include <stdio.h>
#include <stdlib.h>
#include "calendar.h"
#include "display.h"
#include "LinkedList.h"
#include "file.h"

int main()
{
    print_hide();
    print_color(11);

    //初始化结点，加载数据
    LNode* L;
    InitList(&L);
    load(L);

    int year, month, day;
    char ch, flag;
    while (1)
    {
        //默认为年月日赋值非法数值，若用户输入数据类型错误，则会触发报错机制
        year = -1;
        month = -1;
        day = -1;
        
        display();
        ch = getch();
        system("cls");
        if (ch == '0')      //退出程序
        {
            print_toxy(50, 12);
            printf("成功退出程序\n");
            break;
        }
        switch (ch)
        {
            case '1': {     //年份查询
                //输入年份
                print_toxy(48, 12);
                printf("输入年份：");
                scanf_s("%d", &year);
                while (getchar() != '\n');  //输入容错，吃掉非法字符
                //输入日期范围判断
                if (year < 0)
                {
                    print_toxy(48, 14);
                    printf("输入日期范围有误！！！\n");
                    print_toxy(48, 16);
                    break;
                }
                system("cls");
                //闰年的判断
                printf("---------------------------------------------------\n");
                if (IsYear(year) == 1)
                    printf("\t\t| %d年是闰年 |\n", year);
                else
                    printf("\t\t| %d年不是闰年 |\n", year);
                printf("---------------------------------------------------\n");
                //打印一整年的日历
                PrintYear(year);
                break;
            }
            case '2': {     //月份查询
                //输入年月
                print_toxy(48, 12);
                printf("依次输入年月：");
                scanf_s("%d %d", &year,&month);
                while (getchar() != '\n');  //输入容错，吃掉非法字符
               //输入日期范围判断
                if (year < 0 || month < 1 || month>12)
                {
                    print_toxy(48, 14);
                    printf("输入日期范围有误！！！\n");
                    print_toxy(48, 16);
                    break;
                }
                system("cls");
                //打印该月的月历
                PrintMonth(year, month);
                printf("\n");
                break;
            }
            case '3': {     //日期查询
                //输入年月日
                print_toxy(48, 12);
                printf("依次输入年月日：");
                scanf_s("%d %d %d", &year, &month, &day);
                while (getchar() != '\n');  //输入容错，吃掉非法字符
                //输入日期范围判断
                if (year < 0 || month < 1 || month>12 || day < 1 || day>31)
                {
                    print_toxy(48, 14);
                    printf("输入日期范围有误！！！\n");
                    print_toxy(48, 16);
                    break;
                }
                system("cls");
                //打印该日期是星期几
                print_toxy(40, 12);
                printf("-------------------------------------\n");
                print_toxy(48, 13);
                printf("%d年%d月%d日是星期", year, month, day);
                switch (WeekDay(year,month,day))
                {
                    case 0: printf("日\n");  break;
                    case 1: printf("一\n");  break;
                    case 2: printf("二\n");  break;
                    case 3: printf("三\n");  break;
                    case 4: printf("四\n");  break;
                    case 5: printf("五\n");  break;
                    case 6: printf("六\n");  break;
                }
                print_toxy(40, 14);
                printf("-------------------------------------\n");
                print_toxy(48, 16);
                break;
            }
            case '4': {     //日程管理          //进入二级子菜单
                while (1)
                {
                    display_4();
                    ch = getch();
                    system("cls");
                    if (ch == '0')      //返回上一界面
                    {
                        print_toxy(50, 12);
                        printf("成功返回上一界面\n");
                        print_toxy(50, 14);
                        break;
                    }
                    switch (ch)
                    {
                        case '1': {     //添加新的日程
                            if (AddEvent(L) == 1)
                            {
                                print_toxy(48, 16);
                                printf("添加日程成功！！！\n");
                                print_toxy(48, 18);
                            }
                            else
                            {
                                print_toxy(48, 16);
                                printf("添加日程失败！！！\n");
                                print_toxy(48, 18);
                            }
                            break;
                        }
                        case '2': {     //删除已有日程
                            printf("------------------------------------------------------------\n");
                            printf("\t\t    | 所有日程安排 |\n");
                            printf("------------------------------------------------------------\n");
                            TraverseList(L, visit);
                            int num;
                            printf("输入要删除的日程序号：");
                            scanf_s("%d", &num);
                            //对序号范围进行判断，若大于该范围则会在DeleteEvent函数中触发报错机制
                            if (num < 1)
                            {
                                printf("输入序号范围有误！！！\n");
                                printf("删除日程失败！！！\n\n");
                                break;
                            }
                            if (DeleteEvent(L, num) == 1)
                            {
                                printf("删除日程成功！！！\n");
                            }
                            else
                            {
                                printf("删除日程失败！！！\n");
                            }
                            printf("\n");
                            break;
                        }
                        case '3': {     //查看所有日程
                            printf("------------------------------------------------------------\n");
                            printf("\t\t    | 所有日程安排 |\n");
                            printf("------------------------------------------------------------\n");
                            TraverseList(L, visit);
                            break;
                        }
                        default: {
                            print_toxy(50, 12);
                            printf("输入指令有误！！！\n");
                            print_toxy(50, 14);
                            break;
                        }
                    }
                    system("pause");
                    system("cls");
                }
                break;
            }
            default: {
                print_toxy(50, 12);
                printf("输入指令有误！！！\n");
                print_toxy(50, 14);
                break;
            }
        }
        system("pause");
        system("cls");
    }

    print_toxy(50, 14);
	system("pause");
	return 0;
}