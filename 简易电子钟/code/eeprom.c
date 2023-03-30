#include "eeprom.h"
#include "iic.h"
#include "time.h"

//把时钟的数据写进EEPROM中
/*
*********************************************************************************************************
*	函 数 名: WriteTimeEEPROM
*	功能说明: 把时钟、闹钟的数据写进EEPROM中，直接从全局变量my_time,alarm_time中获取数据并写入
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void WriteTimeEEPROM()
{
	IIC_Init();		//IIC初始化	
	
	//写EEPROM地址
	IIC_Start();			//IIC起始信号
	IIC_Write_Byte(0xa0);	//写地址	//0xa0：1010 0000
	IIC_ACK();				//应答
	//写第一个字节
	IIC_Write_Byte(0x00);			//写字节地址
	IIC_ACK();						//应答
	IIC_Write_Byte(my_time.hour);	//写数据
	IIC_ACK();						//应答
	//写第二个字节
	IIC_Write_Byte(0x01);			//写字节地址
	IIC_ACK();						//应答
	IIC_Write_Byte(my_time.minute);	//写数据
	IIC_ACK();						//应答
	//写第三个字节
	IIC_Write_Byte(0x02);			//写字节地址
	IIC_ACK();						//应答
	IIC_Write_Byte(my_time.seconds);//写数据
	IIC_ACK();						//应答
	//写第四个字节
	IIC_Write_Byte(0x03);				//写字节地址
	IIC_ACK();							//应答
	IIC_Write_Byte(alarm_time.hour);	//写数据
	IIC_ACK();							//应答
	//写第五个字节
	IIC_Write_Byte(0x04);				//写字节地址
	IIC_ACK();							//应答
	IIC_Write_Byte(alarm_time.minute);	//写数据
	IIC_ACK();							//应答
	//写第六个字节
	IIC_Write_Byte(0x05);				//写字节地址
	IIC_ACK();							//应答
	IIC_Write_Byte(alarm_time.seconds);	//写数据
	IIC_ACK();							//应答
	//写第七个字节
	IIC_Write_Byte(0x06);				//写字节地址
	IIC_ACK();							//应答
	IIC_Write_Byte(alarm_time.flag);	//写数据
	IIC_ACK();							//应答
	
	IIC_Stop();		//结束信号
}

//从EEPROM中读取时钟的数据
/*
*********************************************************************************************************
*	函 数 名: ReadTimeEEPROM
*	功能说明: 把时钟、闹钟的数据从EEPROM中读取出来，直接把数据写进全局变量my_time,alarm_time中
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ReadTimeEEPROM()
{
	IIC_Init();		//IIC初始化	
	
	//写EEPROM地址
	IIC_Start();			//IIC起始信号	
	IIC_Write_Byte(0xa0);	//写地址	//0xa0：1010 0000
	IIC_ACK();				//应答
	//读第一个字节
	IIC_Write_Byte(0x00);			//数据地址
	IIC_ACK();						//应答
	IIC_Start();					//IIC起始信号
	IIC_Write_Byte(0xa1);			//7位地址位，加上最低位读写位，允许读	//0xa1：1010 0001
	IIC_ACK();						//应答
	my_time.hour = IIC_Read_Byte();		//读数据
	//读第二个字节	
	IIC_Write_Byte(0x01);			//数据地址
	IIC_ACK();						//应答
	IIC_Start();					//IIC起始信号
	IIC_Write_Byte(0xa1);			//7位地址位，加上最低位读写位，允许读
	IIC_ACK();						//应答
	my_time.minute = IIC_Read_Byte();	//读数据
	//读第三个字节
	IIC_Write_Byte(0x02);			//数据地址
	IIC_ACK();						//应答
	IIC_Start();					//IIC起始信号
	IIC_Write_Byte(0xa1);			//7位地址位，加上最低位读写位，允许读
	IIC_ACK();						//应答
	my_time.seconds = IIC_Read_Byte();	//读数据
	//读第四个字节
	IIC_Write_Byte(0x03);				//数据地址
	IIC_ACK();							//应答
	IIC_Start();						//IIC起始信号
	IIC_Write_Byte(0xa1);				//7位地址位，加上最低位读写位，允许读	//0xa1：1010 0001
	IIC_ACK();							//应答
	alarm_time.hour = IIC_Read_Byte();	//读数据
	//读第五个字节
	IIC_Write_Byte(0x04);				//数据地址
	IIC_ACK();							//应答
	IIC_Start();						//IIC起始信号
	IIC_Write_Byte(0xa1);				//7位地址位，加上最低位读写位，允许读
	IIC_ACK();							//应答
	alarm_time.minute = IIC_Read_Byte();		//读数据
	//读第六个字节
	IIC_Write_Byte(0x05);				//数据地址
	IIC_ACK();							//应答
	IIC_Start();						//IIC起始信号
	IIC_Write_Byte(0xa1);				//7位地址位，加上最低位读写位，允许读
	IIC_ACK();							//应答
	alarm_time.seconds = IIC_Read_Byte();	//读数据
	//读第七个字节
	IIC_Write_Byte(0x06);				//数据地址
	IIC_ACK();							//应答
	IIC_Start();						//IIC起始信号
	IIC_Write_Byte(0xa1);				//7位地址位，加上最低位读写位，允许读
	IIC_ACK();							//应答
	alarm_time.flag = IIC_Read_Byte();		//读数据
	
	IIC_Stop();		//IIC结束信号
}