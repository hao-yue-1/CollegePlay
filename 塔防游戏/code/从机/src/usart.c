#include <reg51.h>
#include "usart.h"
#include "select.h"

void init_USART_receive()
{
	TMOD=0x20;                     //���ö�ʱ��T1�����ڷ�ʽ2
	SCON=0x50;                     //���ڹ�����ʽ1�������������
	PCON=0x00;                     //�����ʲ�����
	TH1=0xf4;                      //������Ϊ2400b/s
	TL1=0xf4;
	TR1=1;                         //������ʱ��T1
	REN=1;                         //�����������
}

unsigned char receive()         //���ݽ��պ���
{
	unsigned char dat;
	
	while(RI==0);                   //����û�н������ʱ�ȴ�����
	RI=0;                           //�������ʱ����
	dat=SBUF;                       //�����ջ������е����ݴ���dat
	return dat;                     //�����յ������ݷ���
}