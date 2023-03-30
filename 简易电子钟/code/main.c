#include <reg51.h>
#include "lcd12864.h"
#include "display.h"
#include "time.h"
#include "menu.h"
#include "eeprom.h"

void main()
{	
	ReadTimeEEPROM();	//从EEPROM中读取数据

	InitLCD();			//初始化LCD
	
	InitMyTime(my_time.hour,my_time.minute,my_time.seconds);							//初始化时钟
	InitAlarmTime(alarm_time.hour,alarm_time.minute,alarm_time.seconds,alarm_time.flag);//初始化闹钟
	
	while(1)
	{
		Menu0();			//进入主菜单
	}
}