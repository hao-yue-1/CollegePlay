#include <reg51.h>
#include "usart.h"

void main()
{
	init_USART_send();
	while(1)
	{
		send(P1);
	}
}