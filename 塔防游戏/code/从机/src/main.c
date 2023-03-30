//#include <reg51.h>
#include "usart.h"
#include "LCD12864.h"
#include "menu.h"
#include "select.h"

void main()
{
	InitLCD();
	init_USART_receive();
	
	welcome();
	while(1)
	{
		keyscan();
	}
}