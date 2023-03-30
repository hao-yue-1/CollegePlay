#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include <string.h>

//在字符串的赋值中，若要加入"符号，则应该在"号前加入\，则char *str="\"字符串\""
char ssid[]="\"haoyue\"";													//WiFi名称
char password[]="\"aabbcc112233\"";											//WiFi密码
char devid[]="\"PAB8DQ57MJ\"";												//产品ID
char devname[]="\"SmartRemoteControl\"";									//设备名称
char devkey[]="\"LT3Erxism63JF3Upo+iaPw==\"";								//设备秘钥
char devtheme[]="\"$thing/down/property/PAB8DQ57MJ/SmartRemoteControl\"";	//主题

//设置WiFi连接
void ConnectWiFi(void)
{		
	printf("AT+CWMODE=1\r\n");
	delay_ms(500);
	printf("AT+CWJAP=%s,%s\r\n",ssid,password);
	//由于ESP开机后会自动连接WiFi，所以只要判断WiFi是否连上就行
	while(1)		//连接上WiFi
	{
		//printf("AT+CWJAP=%s,%s\r\n",ssid,password);
		if(USART_RX_STA&0x8000)			//USART_RX_STA：接受状态标记（消息传递的作用）（全局变量）
		{		
			if(USART_RX_BUF[0]=='W')		//WiFi连接成功
			{
				break;
			}
			USART_RX_STA=0;				//清除标记
		}
	}
	while(1)		//连接上WiFi
	{
		//printf("AT+CWJAP=%s,%s\r\n",ssid,password);
		if(USART_RX_STA&0x8000)			//USART_RX_STA：接受状态标记（消息传递的作用）（全局变量）
		{		
			if(USART_RX_BUF[5]=='G')		//WiFi连接成功
			{
				break;
			}
			USART_RX_STA=0;				//清除标记
		}
	}
}

//设置腾讯云设备连接
void ConnectDevice(void)
{	
	printf("AT+TCDEVINFOSET=1,%s,%s,%s\r\n",devid,devname,devkey);	//设置设备信息		
	delay_ms(500);
	printf("AT+TCMQTTCONN=1,5000,240,1,1\r\n");						//配置连接服务器参数
	delay_ms(500);
	printf("AT+TCMQTTSUB=%s,0\r\n",devtheme);						//订阅主题
	delay_ms(500);
//	//前两条指令返回的结果都是一样的，不就行判断，只检测订阅主题是否成功
//	while(1)		//连接上腾讯云设备
//	{
//		if(USART_RX_STA&0x8000)			//USART_RX_STA：接受状态标记（消息传递的作用）（全局变量）
//		{		
//			if(USART_RX_BUF[0]=='O')		//WiFi连接成功
//			{
//				break;
//			}
//			USART_RX_STA=0;				//清除标记
//		}
//	}
}		

