#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include <string.h>

//���ַ����ĸ�ֵ�У���Ҫ����"���ţ���Ӧ����"��ǰ����\����char *str="\"�ַ���\""
char ssid[]="\"haoyue\"";													//WiFi����
char password[]="\"aabbcc112233\"";											//WiFi����
char devid[]="\"PAB8DQ57MJ\"";												//��ƷID
char devname[]="\"SmartRemoteControl\"";									//�豸����
char devkey[]="\"LT3Erxism63JF3Upo+iaPw==\"";								//�豸��Կ
char devtheme[]="\"$thing/down/property/PAB8DQ57MJ/SmartRemoteControl\"";	//����

//����WiFi����
void ConnectWiFi(void)
{		
	printf("AT+CWMODE=1\r\n");
	delay_ms(500);
	printf("AT+CWJAP=%s,%s\r\n",ssid,password);
	//����ESP��������Զ�����WiFi������ֻҪ�ж�WiFi�Ƿ����Ͼ���
	while(1)		//������WiFi
	{
		//printf("AT+CWJAP=%s,%s\r\n",ssid,password);
		if(USART_RX_STA&0x8000)			//USART_RX_STA������״̬��ǣ���Ϣ���ݵ����ã���ȫ�ֱ�����
		{		
			if(USART_RX_BUF[0]=='W')		//WiFi���ӳɹ�
			{
				break;
			}
			USART_RX_STA=0;				//������
		}
	}
	while(1)		//������WiFi
	{
		//printf("AT+CWJAP=%s,%s\r\n",ssid,password);
		if(USART_RX_STA&0x8000)			//USART_RX_STA������״̬��ǣ���Ϣ���ݵ����ã���ȫ�ֱ�����
		{		
			if(USART_RX_BUF[5]=='G')		//WiFi���ӳɹ�
			{
				break;
			}
			USART_RX_STA=0;				//������
		}
	}
}

//������Ѷ���豸����
void ConnectDevice(void)
{	
	printf("AT+TCDEVINFOSET=1,%s,%s,%s\r\n",devid,devname,devkey);	//�����豸��Ϣ		
	delay_ms(500);
	printf("AT+TCMQTTCONN=1,5000,240,1,1\r\n");						//�������ӷ���������
	delay_ms(500);
	printf("AT+TCMQTTSUB=%s,0\r\n",devtheme);						//��������
	delay_ms(500);
//	//ǰ����ָ��صĽ������һ���ģ��������жϣ�ֻ��ⶩ�������Ƿ�ɹ�
//	while(1)		//��������Ѷ���豸
//	{
//		if(USART_RX_STA&0x8000)			//USART_RX_STA������״̬��ǣ���Ϣ���ݵ����ã���ȫ�ֱ�����
//		{		
//			if(USART_RX_BUF[0]=='O')		//WiFi���ӳɹ�
//			{
//				break;
//			}
//			USART_RX_STA=0;				//������
//		}
//	}
}		

