#include "eeprom.h"
#include "iic.h"
#include "time.h"

//��ʱ�ӵ�����д��EEPROM��
/*
*********************************************************************************************************
*	�� �� ��: WriteTimeEEPROM
*	����˵��: ��ʱ�ӡ����ӵ�����д��EEPROM�У�ֱ�Ӵ�ȫ�ֱ���my_time,alarm_time�л�ȡ���ݲ�д��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void WriteTimeEEPROM()
{
	IIC_Init();		//IIC��ʼ��	
	
	//дEEPROM��ַ
	IIC_Start();			//IIC��ʼ�ź�
	IIC_Write_Byte(0xa0);	//д��ַ	//0xa0��1010 0000
	IIC_ACK();				//Ӧ��
	//д��һ���ֽ�
	IIC_Write_Byte(0x00);			//д�ֽڵ�ַ
	IIC_ACK();						//Ӧ��
	IIC_Write_Byte(my_time.hour);	//д����
	IIC_ACK();						//Ӧ��
	//д�ڶ����ֽ�
	IIC_Write_Byte(0x01);			//д�ֽڵ�ַ
	IIC_ACK();						//Ӧ��
	IIC_Write_Byte(my_time.minute);	//д����
	IIC_ACK();						//Ӧ��
	//д�������ֽ�
	IIC_Write_Byte(0x02);			//д�ֽڵ�ַ
	IIC_ACK();						//Ӧ��
	IIC_Write_Byte(my_time.seconds);//д����
	IIC_ACK();						//Ӧ��
	//д���ĸ��ֽ�
	IIC_Write_Byte(0x03);				//д�ֽڵ�ַ
	IIC_ACK();							//Ӧ��
	IIC_Write_Byte(alarm_time.hour);	//д����
	IIC_ACK();							//Ӧ��
	//д������ֽ�
	IIC_Write_Byte(0x04);				//д�ֽڵ�ַ
	IIC_ACK();							//Ӧ��
	IIC_Write_Byte(alarm_time.minute);	//д����
	IIC_ACK();							//Ӧ��
	//д�������ֽ�
	IIC_Write_Byte(0x05);				//д�ֽڵ�ַ
	IIC_ACK();							//Ӧ��
	IIC_Write_Byte(alarm_time.seconds);	//д����
	IIC_ACK();							//Ӧ��
	//д���߸��ֽ�
	IIC_Write_Byte(0x06);				//д�ֽڵ�ַ
	IIC_ACK();							//Ӧ��
	IIC_Write_Byte(alarm_time.flag);	//д����
	IIC_ACK();							//Ӧ��
	
	IIC_Stop();		//�����ź�
}

//��EEPROM�ж�ȡʱ�ӵ�����
/*
*********************************************************************************************************
*	�� �� ��: ReadTimeEEPROM
*	����˵��: ��ʱ�ӡ����ӵ����ݴ�EEPROM�ж�ȡ������ֱ�Ӱ�����д��ȫ�ֱ���my_time,alarm_time��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ReadTimeEEPROM()
{
	IIC_Init();		//IIC��ʼ��	
	
	//дEEPROM��ַ
	IIC_Start();			//IIC��ʼ�ź�	
	IIC_Write_Byte(0xa0);	//д��ַ	//0xa0��1010 0000
	IIC_ACK();				//Ӧ��
	//����һ���ֽ�
	IIC_Write_Byte(0x00);			//���ݵ�ַ
	IIC_ACK();						//Ӧ��
	IIC_Start();					//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);			//7λ��ַλ���������λ��дλ�������	//0xa1��1010 0001
	IIC_ACK();						//Ӧ��
	my_time.hour = IIC_Read_Byte();		//������
	//���ڶ����ֽ�	
	IIC_Write_Byte(0x01);			//���ݵ�ַ
	IIC_ACK();						//Ӧ��
	IIC_Start();					//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);			//7λ��ַλ���������λ��дλ�������
	IIC_ACK();						//Ӧ��
	my_time.minute = IIC_Read_Byte();	//������
	//���������ֽ�
	IIC_Write_Byte(0x02);			//���ݵ�ַ
	IIC_ACK();						//Ӧ��
	IIC_Start();					//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);			//7λ��ַλ���������λ��дλ�������
	IIC_ACK();						//Ӧ��
	my_time.seconds = IIC_Read_Byte();	//������
	//�����ĸ��ֽ�
	IIC_Write_Byte(0x03);				//���ݵ�ַ
	IIC_ACK();							//Ӧ��
	IIC_Start();						//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);				//7λ��ַλ���������λ��дλ�������	//0xa1��1010 0001
	IIC_ACK();							//Ӧ��
	alarm_time.hour = IIC_Read_Byte();	//������
	//��������ֽ�
	IIC_Write_Byte(0x04);				//���ݵ�ַ
	IIC_ACK();							//Ӧ��
	IIC_Start();						//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);				//7λ��ַλ���������λ��дλ�������
	IIC_ACK();							//Ӧ��
	alarm_time.minute = IIC_Read_Byte();		//������
	//���������ֽ�
	IIC_Write_Byte(0x05);				//���ݵ�ַ
	IIC_ACK();							//Ӧ��
	IIC_Start();						//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);				//7λ��ַλ���������λ��дλ�������
	IIC_ACK();							//Ӧ��
	alarm_time.seconds = IIC_Read_Byte();	//������
	//�����߸��ֽ�
	IIC_Write_Byte(0x06);				//���ݵ�ַ
	IIC_ACK();							//Ӧ��
	IIC_Start();						//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);				//7λ��ַλ���������λ��дλ�������
	IIC_ACK();							//Ӧ��
	alarm_time.flag = IIC_Read_Byte();		//������
	
	IIC_Stop();		//IIC�����ź�
}