#include "iic.h"

void write_eeprom(unsigned char *score)
{
	IIC_Init();		//IIC初始化	
	
	//把数据写进EEPROM
	IIC_Start();			//IIC起始信号
	IIC_Write_Byte(0xa0);	//写器件地址	//0xa0：1010 0000
	IIC_ACK();				//应答
	//写第一个字节
	IIC_Write_Byte(0x00);	//写字节地址
	IIC_ACK();				//应答
	IIC_Write_Byte(score[0]);	//写数据
	IIC_ACK();				//应答
	//写第二个字节
	IIC_Write_Byte(0x01);	//写字节地址
	IIC_ACK();				//应答
	IIC_Write_Byte(score[1]);	//写数据
	IIC_ACK();				//应答
	//写第三个字节
	IIC_Write_Byte(0x02);	//写字节地址
	IIC_ACK();				//应答
	IIC_Write_Byte(score[2]);	//写数据
	IIC_ACK();				//应答
	//写第四个字节
	IIC_Write_Byte(0x03);	//写字节地址
	IIC_ACK();				//应答
	IIC_Write_Byte(score[3]);	//写数据
	IIC_ACK();				//应答
	//写第五个字节
	IIC_Write_Byte(0x04);	//写字节地址
	IIC_ACK();				//应答
	IIC_Write_Byte(score[4]);	//写数据
	IIC_ACK();				//应答
	//写第六个字节
	IIC_Write_Byte(0x05);	//写字节地址
	IIC_ACK();				//应答
	IIC_Write_Byte(score[5]);	//写数据
	IIC_ACK();				//应答
	
	IIC_Stop();				//结束信号
}

void read_eeprom(unsigned char *score)
{
	IIC_Init();		//IIC初始化	
	
	//从EEPROM读数据
	IIC_Start();			//IIC起始信号	
	IIC_Write_Byte(0xa0);	//写器件地址	//0xa0：1010 0000
	IIC_ACK();				//应答
	//读第一个字节
	IIC_Write_Byte(0x00);	//数据地址
	IIC_ACK();				//应答
	IIC_Start();			//IIC起始信号
	IIC_Write_Byte(0xa1);	//7位地址位，加上最低位读写位，允许读	//0xa1：1010 0001
	IIC_ACK();				//应答
	score[0] = IIC_Read_Byte();	//读数据
	//读第二个字节
	IIC_Write_Byte(0x01);	//数据地址
	IIC_ACK();				//应答
	IIC_Start();			//IIC起始信号
	IIC_Write_Byte(0xa1);	//7位地址位，加上最低位读写位，允许读
	IIC_ACK();				//应答
	score[1] = IIC_Read_Byte();	//读数据
	//读第三个字节
	IIC_Write_Byte(0x02);	//数据地址
	IIC_ACK();				//应答
	IIC_Start();			//IIC起始信号
	IIC_Write_Byte(0xa1);	//7位地址位，加上最低位读写位，允许读
	IIC_ACK();				//应答
	score[2] = IIC_Read_Byte();	//读数据
	//读第四个字节
	IIC_Write_Byte(0x03);	//数据地址
	IIC_ACK();				//应答
	IIC_Start();			//IIC起始信号
	IIC_Write_Byte(0xa1);	//7位地址位，加上最低位读写位，允许读	//0xa1：1010 0001
	IIC_ACK();				//应答
	score[3] = IIC_Read_Byte();	//读数据
	//读第五个字节
	IIC_Write_Byte(0x04);	//数据地址
	IIC_ACK();				//应答
	IIC_Start();			//IIC起始信号
	IIC_Write_Byte(0xa1);	//7位地址位，加上最低位读写位，允许读
	IIC_ACK();				//应答
	score[4] = IIC_Read_Byte();	//读数据
	//读第六个字节
	IIC_Write_Byte(0x05);	//数据地址
	IIC_ACK();				//应答
	IIC_Start();			//IIC起始信号
	IIC_Write_Byte(0xa1);	//7位地址位，加上最低位读写位，允许读
	IIC_ACK();				//应答
	score[5] = IIC_Read_Byte();	//读数据
	
	IIC_Stop();				//IIC结束信号
}