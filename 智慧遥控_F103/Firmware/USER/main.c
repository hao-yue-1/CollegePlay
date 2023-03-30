#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "infrared.h"
#include "esp8266.h"
#include <string.h>
#include <stdlib.h>

/************************************************
 
	������Ѷ������ƽ�������ǻ�ң��
	ESP8266-12S+STM32F103ZET6
	ʵ��Զ�̶Կյ��Ŀ���
 
	1. ���ع���
	
	�����ڣ�2021.9.19
	
************************************************/


/*
*********************************************************************************************************
*	�� �� ��: StrCrop
*	����˵��: �ü����ڽ��յ����ַ�������ȡ���м�ֵ����Ϣ��������ĳ���������µļ�ֵ��{******}��
*	��    ��: 1. str�����ü����ַ���
			  2. front����ʼ�ü���λ�ã�����}����ֹ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StrCrop(char str[], int front)
{
	int len = strlen(str);		//��ȡ�ַ����ĳ���
	int i;
	for (i = 0; front <len; i++, front++)
	{
		str[i] = str[front];
		if (str[i] == '}')
			break;
	}
	i++;
	str[i] = '\0';	//�ֶ�����ֹ��
}

int main(void)
{
	int A,B,C;		//���������ģ�鷢������
	
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	LED_Init();							//��ʼ��LED	
    TIM3_PWM_Init(378-1,5-1);       	//72M/5=14.4M�ļ���Ƶ�ʣ��Զ���װ��Ϊ378����ôPWMƵ��Ϊ14.4M/378=38kHZ
	//�Ƚ���WiFi�����������������ã�ȷ��ESP8266�ɹ����ӵ��ƶ��豸����������
	LED=0;			
	ConnectWiFi();	//����WiFi
	LED=1;
	delay_ms(500);
	LED=0;
	ConnectDevice();//������Ѷ���豸
	LED=1;
	delay_ms(500);
	LED=0;
	ConnectDevice();//������Ѷ���豸	//����Ҫ�����βŻ����ϣ������ϲ�Ӧ������
	LED=1;
	//��ʼ������ģ�������	//���䣬�Զ���26
	A=0xB2;
	B=0xBF;
	C=0xD0;
	//��ѭ��
	while(1) 
	{			
		if(USART_RX_STA&0x8000)			//USART_RX_STA������״̬��ǣ���Ϣ���ݵ����ã���ȫ�ֱ�����
		{	
			StrCrop((char*)USART_RX_BUF,138);		//��ȡ���϶̵��м�ֵ���ַ�����{***}
			//printf("%s\r\n",(char*)USART_RX_BUF);
			
			if (USART_RX_BUF[2] == 's')			//����
			{
				switch (USART_RX_BUF[10])
				{
					case '0': {
						Normal_Code(0xB2,0x7B,0xE0);	//�ػ�
						//printf("�ػ�\r\n");
						break;
					}
					case '1': {
						Normal_Code(0xB2,0xBF,0xD0);	//���䣬�Զ���26
						//printf("����\r\n");
						break;
					}
				}
				Normal_Code(A,B,C);
			}
			else if (USART_RX_BUF[2] == 't')	//�¶�
			{
				StrCrop((char*)USART_RX_BUF,15);	//��ȡ���ַ����е����ֲ���
				switch(atoi((char*)USART_RX_BUF))	//ת��Ϊ����
				{
					case 17:{
						C &= ~(1 << 4);
						C &= ~(1 << 5);
						C &= ~(1 << 6);
						C &= ~(1 << 7);
						//printf("17\r\n");
						break;
					}
					case 18:{
						C |= (1 << 4);
						C &= ~(1 << 5);
						C &= ~(1 << 6);
						C &= ~(1 << 7);
						//printf("18\r\n");
						break;
					}
					case 19:{
						C |= (1 << 4);
						C |= (1 << 5);
						C &= ~(1 << 6);
						C &= ~(1 << 7);
						//printf("19\r\n");
						break;
					}
					case 20:{
						C &= ~(1 << 4);
						C |= (1 << 5);
						C &= ~(1 << 6);
						C &= ~(1 << 7);
						//printf("20\r\n");
						break;
					}
					case 21:{
						C &= ~(1 << 4);
						C |= (1 << 5);
						C |= (1 << 6);
						C &= ~(1 << 7);
						//printf("21\r\n");
						break;
					}
					case 22:{
						C |= (1 << 4);
						C |= (1 << 5);
						C |= (1 << 6);
						C &= ~(1 << 7);
						//printf("22\r\n");
						break;
					}
					case 23:{
						C |= (1 << 4);
						C &= ~(1 << 5);
						C |= (1 << 6);
						C &= ~(1 << 7);
						//printf("23\r\n");
						break;
					}
					case 24:{
						C &= ~(1 << 4);
						C &= ~(1 << 5);
						C |= (1 << 6);
						C &= ~(1 << 7);
						//printf("24\r\n");
						break;
					}
					case 25:{
						C &= ~(1 << 4);
						C &= ~(1 << 5);
						C |= (1 << 6);
						C |= (1 << 7);
						//printf("25\r\n");
						break;
					}
					case 26:{
						C |= (1 << 4);
						C &= ~(1 << 5);
						C |= (1 << 6);
						C |= (1 << 7);
						//printf("26\r\n");
						break;
					}
					case 27:{
						C |= (1 << 4);
						C &= ~(1 << 5);
						C &= ~(1 << 6);
						C |= (1 << 7);
						//printf("27\r\n");
						break;
					}
					case 28:{
						C &= ~(1 << 4);
						C &= ~(1 << 5);
						C &= ~(1 << 6);
						C |= (1 << 7);
						//printf("28\r\n");
						break;
					}
					case 29:{
						C &= ~(1 << 4);
						C |= (1 << 5);
						C &= ~(1 << 6);
						C |= (1 << 7);
						//printf("29\r\n");
						break;
					}
					case 30:{
						C |= (1 << 4);
						C |= (1 << 5);
						C &= ~(1 << 6);
						C |= (1 << 7);
						//printf("30\r\n");
						break;
					}					
				}
				//printf("��������\r\n");
				Normal_Code(A,B,C);
			}
			else if (USART_RX_BUF[2] == 'w')	//����
			{
				switch (USART_RX_BUF[13])
				{
					case '0': {
						B |= (1 << 5);
						B &= ~(1 << 6);
						B |= (1 << 7);
						//printf("�Զ�\r\n");
						break;
					}
					case '1': {
						B &= ~(1 << 5);
						B &= ~(1 << 6);
						B |= (1 << 7);
						//printf("�ͷ�\r\n");
						break;
					}
					case '2': {
						B &= ~(1 << 5);
						B |= (1 << 6);
						B &= ~(1 << 7);
						//printf("�з�\r\n");
						break;
					}
					case '3': {
						B |= (1 << 5);
						B &= ~(1 << 6);
						B &= ~(1 << 7);
						//printf("�߷�\r\n");
						break;
					}
				}
				//printf("��������\r\n");
				Normal_Code(A,B,C);
			}
			else if (USART_RX_BUF[2] == 'm')		//ģʽ
			{
				switch (USART_RX_BUF[8])
				{
					case '0': {	
						C &= ~(1 << 2);
						C |= (1 << 3);						
						//printf("�Զ�\r\n");
						break;
					}
					case '1': {
						C &= ~(1 << 2);
						C &= ~(1 << 3);
						//printf("����\r\n");
						break;
					}
					case '2': {
						C |= (1 << 2);
						C &= ~(1 << 3);
						//printf("��ʪ\r\n");
						break;
					}
					case '3': {
						C |= (1 << 2);
						C |= (1 << 3);
						//printf("����\r\n");
						break;
					}
					case '4': {
						C |= (1 << 2);
						C &= ~(1 << 3);
						//printf("�ͷ�\r\n");
						break;
					}
				}
				//printf("��������\r\n");
				Normal_Code(A,B,C);
			}
			USART_RX_STA=0;				//������
		}
	}
}

