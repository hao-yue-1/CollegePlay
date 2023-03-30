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
 
	基于腾讯物联网平开发的智慧遥控
	ESP8266-12S+STM32F103ZET6
	实现远程对空调的控制
 
	1. 开关功能
	
	创建于：2021.9.19
	
************************************************/


/*
*********************************************************************************************************
*	函 数 名: StrCrop
*	功能说明: 裁剪串口接收到的字符串，提取出有价值的信息（代表了某个按键按下的键值：{******}）
*	形    参: 1. str：待裁剪的字符串
			  2. front：开始裁减的位置（到‘}’截止）
*	返 回 值: 无
*********************************************************************************************************
*/
void StrCrop(char str[], int front)
{
	int len = strlen(str);		//获取字符串的长度
	int i;
	for (i = 0; front <len; i++, front++)
	{
		str[i] = str[front];
		if (str[i] == '}')
			break;
	}
	i++;
	str[i] = '\0';	//手动加终止符
}

int main(void)
{
	int A,B,C;		//用于向红外模块发送数据
	
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口
	LED_Init();							//初始化LED	
    TIM3_PWM_Init(378-1,5-1);       	//72M/5=14.4M的计数频率，自动重装载为378，那么PWM频率为14.4M/378=38kHZ
	//先进行WiFi和物联网的连接设置，确保ESP8266成功连接到云端设备，正常工作
	LED=0;			
	ConnectWiFi();	//连接WiFi
	LED=1;
	delay_ms(500);
	LED=0;
	ConnectDevice();//连接腾讯云设备
	LED=1;
	delay_ms(500);
	LED=0;
	ConnectDevice();//连接腾讯云设备	//这里要连两次才会连上，理论上不应该这样
	LED=1;
	//初始化红外模块的数据	//制冷，自动，26
	A=0xB2;
	B=0xBF;
	C=0xD0;
	//主循环
	while(1) 
	{			
		if(USART_RX_STA&0x8000)			//USART_RX_STA：接受状态标记（消息传递的作用）（全局变量）
		{	
			StrCrop((char*)USART_RX_BUF,138);		//截取出较短的有价值的字符串：{***}
			//printf("%s\r\n",(char*)USART_RX_BUF);
			
			if (USART_RX_BUF[2] == 's')			//开关
			{
				switch (USART_RX_BUF[10])
				{
					case '0': {
						Normal_Code(0xB2,0x7B,0xE0);	//关机
						//printf("关机\r\n");
						break;
					}
					case '1': {
						Normal_Code(0xB2,0xBF,0xD0);	//制冷，自动，26
						//printf("开机\r\n");
						break;
					}
				}
				Normal_Code(A,B,C);
			}
			else if (USART_RX_BUF[2] == 't')	//温度
			{
				StrCrop((char*)USART_RX_BUF,15);	//截取出字符串中的数字部分
				switch(atoi((char*)USART_RX_BUF))	//转换为整数
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
				//printf("发送数据\r\n");
				Normal_Code(A,B,C);
			}
			else if (USART_RX_BUF[2] == 'w')	//风速
			{
				switch (USART_RX_BUF[13])
				{
					case '0': {
						B |= (1 << 5);
						B &= ~(1 << 6);
						B |= (1 << 7);
						//printf("自动\r\n");
						break;
					}
					case '1': {
						B &= ~(1 << 5);
						B &= ~(1 << 6);
						B |= (1 << 7);
						//printf("低风\r\n");
						break;
					}
					case '2': {
						B &= ~(1 << 5);
						B |= (1 << 6);
						B &= ~(1 << 7);
						//printf("中风\r\n");
						break;
					}
					case '3': {
						B |= (1 << 5);
						B &= ~(1 << 6);
						B &= ~(1 << 7);
						//printf("高风\r\n");
						break;
					}
				}
				//printf("发送数据\r\n");
				Normal_Code(A,B,C);
			}
			else if (USART_RX_BUF[2] == 'm')		//模式
			{
				switch (USART_RX_BUF[8])
				{
					case '0': {	
						C &= ~(1 << 2);
						C |= (1 << 3);						
						//printf("自动\r\n");
						break;
					}
					case '1': {
						C &= ~(1 << 2);
						C &= ~(1 << 3);
						//printf("制冷\r\n");
						break;
					}
					case '2': {
						C |= (1 << 2);
						C &= ~(1 << 3);
						//printf("抽湿\r\n");
						break;
					}
					case '3': {
						C |= (1 << 2);
						C |= (1 << 3);
						//printf("制热\r\n");
						break;
					}
					case '4': {
						C |= (1 << 2);
						C &= ~(1 << 3);
						//printf("送风\r\n");
						break;
					}
				}
				//printf("发送数据\r\n");
				Normal_Code(A,B,C);
			}
			USART_RX_STA=0;				//清除标记
		}
	}
}

