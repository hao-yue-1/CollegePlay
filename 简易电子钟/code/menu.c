#include "menu.h"
#include <reg51.h>
#include "display.h"
#include "time.h"
#include "eeprom.h"

extern MyTime my_time;			//ʱ��
extern MyTime alarm_time;		//����
extern char flag;

//��������
sbit key_enter=P1^4;	//ȷ�ϼ�
sbit key_up=P1^0;		//���ϼ�
sbit key_down=P1^1;		//���¼�
sbit key_left=P1^2;		//�����
sbit key_right=P1^3;	//���Ҽ�
sbit key_return=P1^5;	//���ؼ�

int cursor=0;		//���ܹ��	0.��ʾ  1.ʱ��	2.����
int cursor_num=3;	//���ֹ��  1.ʱ 2.�� 3.��
int key=0;

//����ɨ��
/*
*********************************************************************************************************
*	�� �� ��: KeyScan
*	����˵��: ��ȡ�û����µİ����źţ������ض�Ӧ�İ�����־
*	��    ��: ��
*	�� �� ֵ: 1. ENTER_PRES : ȷ�ϼ�
*			  2. UP_PRES    : ���ϼ�
*			  3. DOWN_PRES  : ���¼�
*			  4. LEFT_PRES  : �����
*			  5. RIGHT_PRES : ���Ҽ�
*			  6. RETURN_PRES: ���ؼ�
*********************************************************************************************************
*/
int KeyScan()
{
	unsigned char key_flag=1;     //�����ɿ���־

	//ModifyTime();
	if(key_flag==1&&(key_enter==0||key_up==0||key_down==0||key_left==0||key_right==0||key_return==0))
    {
		/*
		* ������������ݵĹ��ܷ������������ѭ��������Ϊ���ú�����ȱ��Ϊ����ʱ��ϳ���
		* ��������ѭ���л�Ӱ�쵽��Ƭ���԰�������Ӧ�ٶȣ�������ʱ����ְ�������Ӧ�������
		* ��������ȿ���������Ӱ�죬�������øú���������ʱ���������ʱ������һ���򵥵İ�������
		* ���������øú���������ʱӦΪdelay(200)��
		*/
        delay(130);			//����
		WriteTimeEEPROM();	//������д��EEPROM��
        key_flag=0;
        if(key_enter==0)       	return ENTER_PRES;	//ȷ�ϼ�
		else if(key_return==0)	return RETURN_PRES;	//���ؼ�
        else if(key_up==0)  	return UP_PRES;		//���ϼ�
        else if(key_down==0) 	return DOWN_PRES;   //���¼�
		else if(key_left==0)  	return LEFT_PRES;	//�����
        else if(key_right==0) 	return RIGHT_PRES;	//���Ҽ�
	}
	else if(key_enter==0&&key_up==0&&key_down==0&&key_left==0&&key_right==0&&key_return==0)
	{
		key_flag=1;
	}
	/*
	* ���ڰ�������ʱ���б����£�Ҳ��ζ�����ݲ���������ʱ�̱��棬
	* ��˳��˳䵱��������ʱ�����⣬�����ް������µ�������е��ã�
	* ʵ�ⲻ��Ӱ�쵽��������Ӧ���⣬�����ݵı���Ҳ��ʵʱ��
	*/
	WriteTimeEEPROM();	//������д��EEPROM��
    return 0;   //�ް�������
}

//���˵�
/*
*********************************************************************************************************
*	�� �� ��: Menu0
*	����˵��: ͨ����ȡ�û����µİ�����־��ʵ�ֶ�Ӧ�Ĺ��ܣ�ȷ�ϼ��������Ӧ���ܣ�����/�Ҽ����ƶ���꣩
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Menu0()
{
	key=KeyScan();	//��ȡ�û����µİ���
	switch(key)	//�жϰ��µ����ĸ�����
	{
		case ENTER_PRES:{	//ȷ�ϼ�
			switch(cursor)		//�жϰ���ȷ�ϼ�ʱ������ĸ�����
			{
				case 0:{		//ʱ����ʾ
					flag=1;
					//��ʾʱ��
					PrintTimeHour(my_time.hour);
					PrintTimeMinute(my_time.minute);
					PrintTimeSeconds(my_time.seconds);
					//��ʱ��LCD�����ֻ���������ͨ�����ϵ���ù��ܽ���ˢ��
					PrintColon();
					PrintCursor(cursor);			//��LCD����ʾ���
					PrintClock(alarm_time.flag);	//��LCD����ʾ����
					PrintMode(cursor);				//��LCD����ʾʱ����ʾ
					break;
				}
				case 1:{		//ʱ�ӵ���	
					flag=0;
					PrintMode(cursor);	//��LCD����ʾʱ�ӵ���
					PrintColon();
					SetClock();			//����ʱ��
					break;
				}
				case 2:{		//����ģ����
					flag=0;
					PrintMode(cursor);	//��LCD����ʾ���ӵ���
					PrintColon();
					SetAlarmClock();	//��������
					break;
				}
				default:break;
			}
			break;
		}
		case LEFT_PRES:{	//����
			if(cursor>0)
			{
				cursor--;
				PrintCursor(cursor);
			}
			break;
		}
		case RIGHT_PRES:{	//����
			if(cursor<2)
			{
				cursor++;
				PrintCursor(cursor);
			}
			break;
		}
		default:break;		//���ϡ��£����ز����跴Ӧ
	}
}

//��������
/*
*********************************************************************************************************
*	�� �� ��: SetAlarmClock
*	����˵��: ����ѭ����ȡ�û����µİ�����־������ȷ�ϼ��˳�ѭ�������޸����ӵ���ֵ����ѡ�еĲ����ᷴ����ʾ
*             ����ʾ�û�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SetAlarmClock()
{
	//��ȡ�����ӵ�ֵ����LCD����ʾ
	int hour=alarm_time.hour;
	int minute=alarm_time.minute;
	int seconds=alarm_time.seconds;	
	//������ʾѡ�еĲ���
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
	//���а���ɨ�裬��ȡ����ֵ
	while(1)
	{
		key=KeyScan();	//��ȡ�û����µİ���
		switch(key)
		{
			case ENTER_PRES:{	//ȷ��
				PrintSetOK(1);	//��LCD����ʾ�������óɹ�
				//�˳�����ɨ���Ϊ���Ӹ�ֵ
				alarm_time.hour=hour;
				alarm_time.minute=minute;
				alarm_time.seconds=seconds;	
				alarm_time.flag=1;			//��������
				PrintClock(alarm_time.flag);
				return;
			}
			case RETURN_PRES:{	//����
				PrintReturn(1);			//��LCD����ʾ�˳����ӵ���
				alarm_time.flag=0;		//�ر�����
				PrintClock(alarm_time.flag);
				return;
			}
			case UP_PRES:{		//����
				//����������+1
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
			case DOWN_PRES:{	//����
				//����������-1
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
			case LEFT_PRES:{	//����
				if(cursor_num>1)
					cursor_num--;
				//������ʾѡ�еĲ���
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
			case RIGHT_PRES:{	//����
				if(cursor_num<3)
					cursor_num++;
				//������ʾѡ�еĲ���
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

//����ʱ��
/*
*********************************************************************************************************
*	�� �� ��: SetClock
*	����˵��: ����ѭ����ȡ�û����µİ�����־������ȷ�ϼ��˳�ѭ�������޸�ʱ�ӵ���ֵ����ѡ�еĲ����ᷴ����ʾ
*             ����ʾ�û�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SetClock()
{
	//��ȡ��ʱ�ӵ�ֵ����LCD����ʾ
	int hour=my_time.hour;
	int minute=my_time.minute;
	int seconds=my_time.seconds;	
	//������ʾѡ�еĲ���
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
	//���а���ɨ�裬��ȡ����ֵ
	while(1)
	{
		key=KeyScan();	//��ȡ�û����µİ���
		switch(key)
		{
			case ENTER_PRES:{	//ȷ��
				PrintSetOK(0);	//��LCD����ʾʱ�����óɹ�
				//�˳�����ɨ���Ϊʱ�Ӹ�ֵ
				my_time.hour=hour;
				my_time.minute=minute;
				my_time.seconds=seconds;
				cnt=0;	//�Ѷ�ʱ���ļ�������0
				return;
			}
			case RETURN_PRES:{	//����
				PrintReturn(0);	//��LCD����ʾ�˳�ʱ�ӵ���
				return;
			}
			case UP_PRES:{		//����
				//����������+1
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
			case DOWN_PRES:{	//����
				//����������-1
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
			case LEFT_PRES:{	//����
				if(cursor_num>1)
					cursor_num--;
				//������ʾѡ�еĲ���
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
			case RIGHT_PRES:{	//����
				if(cursor_num<3)
					cursor_num++;
				//������ʾѡ�еĲ���
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


