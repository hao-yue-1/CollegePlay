#include <reg51.h>
#include "lcd12864.h"
#include "display.h"
#include "time.h"
#include "menu.h"
#include "eeprom.h"

void main()
{	
	ReadTimeEEPROM();	//��EEPROM�ж�ȡ����

	InitLCD();			//��ʼ��LCD
	
	InitMyTime(my_time.hour,my_time.minute,my_time.seconds);							//��ʼ��ʱ��
	InitAlarmTime(alarm_time.hour,alarm_time.minute,alarm_time.seconds,alarm_time.flag);//��ʼ������
	
	while(1)
	{
		Menu0();			//�������˵�
	}
}