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

    //��ʼ����㣬��������
    LNode* L;
    InitList(&L);
    load(L);

    int year, month, day;
    char ch, flag;
    while (1)
    {
        //Ĭ��Ϊ�����ո�ֵ�Ƿ���ֵ�����û������������ʹ�����ᴥ���������
        year = -1;
        month = -1;
        day = -1;
        
        display();
        ch = getch();
        system("cls");
        if (ch == '0')      //�˳�����
        {
            print_toxy(50, 12);
            printf("�ɹ��˳�����\n");
            break;
        }
        switch (ch)
        {
            case '1': {     //��ݲ�ѯ
                //�������
                print_toxy(48, 12);
                printf("������ݣ�");
                scanf_s("%d", &year);
                while (getchar() != '\n');  //�����ݴ��Ե��Ƿ��ַ�
                //�������ڷ�Χ�ж�
                if (year < 0)
                {
                    print_toxy(48, 14);
                    printf("�������ڷ�Χ���󣡣���\n");
                    print_toxy(48, 16);
                    break;
                }
                system("cls");
                //������ж�
                printf("---------------------------------------------------\n");
                if (IsYear(year) == 1)
                    printf("\t\t| %d�������� |\n", year);
                else
                    printf("\t\t| %d�겻������ |\n", year);
                printf("---------------------------------------------------\n");
                //��ӡһ���������
                PrintYear(year);
                break;
            }
            case '2': {     //�·ݲ�ѯ
                //��������
                print_toxy(48, 12);
                printf("�����������£�");
                scanf_s("%d %d", &year,&month);
                while (getchar() != '\n');  //�����ݴ��Ե��Ƿ��ַ�
               //�������ڷ�Χ�ж�
                if (year < 0 || month < 1 || month>12)
                {
                    print_toxy(48, 14);
                    printf("�������ڷ�Χ���󣡣���\n");
                    print_toxy(48, 16);
                    break;
                }
                system("cls");
                //��ӡ���µ�����
                PrintMonth(year, month);
                printf("\n");
                break;
            }
            case '3': {     //���ڲ�ѯ
                //����������
                print_toxy(48, 12);
                printf("�������������գ�");
                scanf_s("%d %d %d", &year, &month, &day);
                while (getchar() != '\n');  //�����ݴ��Ե��Ƿ��ַ�
                //�������ڷ�Χ�ж�
                if (year < 0 || month < 1 || month>12 || day < 1 || day>31)
                {
                    print_toxy(48, 14);
                    printf("�������ڷ�Χ���󣡣���\n");
                    print_toxy(48, 16);
                    break;
                }
                system("cls");
                //��ӡ�����������ڼ�
                print_toxy(40, 12);
                printf("-------------------------------------\n");
                print_toxy(48, 13);
                printf("%d��%d��%d��������", year, month, day);
                switch (WeekDay(year,month,day))
                {
                    case 0: printf("��\n");  break;
                    case 1: printf("һ\n");  break;
                    case 2: printf("��\n");  break;
                    case 3: printf("��\n");  break;
                    case 4: printf("��\n");  break;
                    case 5: printf("��\n");  break;
                    case 6: printf("��\n");  break;
                }
                print_toxy(40, 14);
                printf("-------------------------------------\n");
                print_toxy(48, 16);
                break;
            }
            case '4': {     //�ճ̹���          //��������Ӳ˵�
                while (1)
                {
                    display_4();
                    ch = getch();
                    system("cls");
                    if (ch == '0')      //������һ����
                    {
                        print_toxy(50, 12);
                        printf("�ɹ�������һ����\n");
                        print_toxy(50, 14);
                        break;
                    }
                    switch (ch)
                    {
                        case '1': {     //����µ��ճ�
                            if (AddEvent(L) == 1)
                            {
                                print_toxy(48, 16);
                                printf("����ճ̳ɹ�������\n");
                                print_toxy(48, 18);
                            }
                            else
                            {
                                print_toxy(48, 16);
                                printf("����ճ�ʧ�ܣ�����\n");
                                print_toxy(48, 18);
                            }
                            break;
                        }
                        case '2': {     //ɾ�������ճ�
                            printf("------------------------------------------------------------\n");
                            printf("\t\t    | �����ճ̰��� |\n");
                            printf("------------------------------------------------------------\n");
                            TraverseList(L, visit);
                            int num;
                            printf("����Ҫɾ�����ճ���ţ�");
                            scanf_s("%d", &num);
                            //����ŷ�Χ�����жϣ������ڸ÷�Χ�����DeleteEvent�����д����������
                            if (num < 1)
                            {
                                printf("������ŷ�Χ���󣡣���\n");
                                printf("ɾ���ճ�ʧ�ܣ�����\n\n");
                                break;
                            }
                            if (DeleteEvent(L, num) == 1)
                            {
                                printf("ɾ���ճ̳ɹ�������\n");
                            }
                            else
                            {
                                printf("ɾ���ճ�ʧ�ܣ�����\n");
                            }
                            printf("\n");
                            break;
                        }
                        case '3': {     //�鿴�����ճ�
                            printf("------------------------------------------------------------\n");
                            printf("\t\t    | �����ճ̰��� |\n");
                            printf("------------------------------------------------------------\n");
                            TraverseList(L, visit);
                            break;
                        }
                        default: {
                            print_toxy(50, 12);
                            printf("����ָ�����󣡣���\n");
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
                printf("����ָ�����󣡣���\n");
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