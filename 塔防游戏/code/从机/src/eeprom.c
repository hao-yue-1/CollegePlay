#include "iic.h"

void write_eeprom(unsigned char *score)
{
	IIC_Init();		//IIC��ʼ��	
	
	//������д��EEPROM
	IIC_Start();			//IIC��ʼ�ź�
	IIC_Write_Byte(0xa0);	//д������ַ	//0xa0��1010 0000
	IIC_ACK();				//Ӧ��
	//д��һ���ֽ�
	IIC_Write_Byte(0x00);	//д�ֽڵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Write_Byte(score[0]);	//д����
	IIC_ACK();				//Ӧ��
	//д�ڶ����ֽ�
	IIC_Write_Byte(0x01);	//д�ֽڵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Write_Byte(score[1]);	//д����
	IIC_ACK();				//Ӧ��
	//д�������ֽ�
	IIC_Write_Byte(0x02);	//д�ֽڵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Write_Byte(score[2]);	//д����
	IIC_ACK();				//Ӧ��
	//д���ĸ��ֽ�
	IIC_Write_Byte(0x03);	//д�ֽڵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Write_Byte(score[3]);	//д����
	IIC_ACK();				//Ӧ��
	//д������ֽ�
	IIC_Write_Byte(0x04);	//д�ֽڵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Write_Byte(score[4]);	//д����
	IIC_ACK();				//Ӧ��
	//д�������ֽ�
	IIC_Write_Byte(0x05);	//д�ֽڵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Write_Byte(score[5]);	//д����
	IIC_ACK();				//Ӧ��
	
	IIC_Stop();				//�����ź�
}

void read_eeprom(unsigned char *score)
{
	IIC_Init();		//IIC��ʼ��	
	
	//��EEPROM������
	IIC_Start();			//IIC��ʼ�ź�	
	IIC_Write_Byte(0xa0);	//д������ַ	//0xa0��1010 0000
	IIC_ACK();				//Ӧ��
	//����һ���ֽ�
	IIC_Write_Byte(0x00);	//���ݵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Start();			//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);	//7λ��ַλ���������λ��дλ�������	//0xa1��1010 0001
	IIC_ACK();				//Ӧ��
	score[0] = IIC_Read_Byte();	//������
	//���ڶ����ֽ�
	IIC_Write_Byte(0x01);	//���ݵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Start();			//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);	//7λ��ַλ���������λ��дλ�������
	IIC_ACK();				//Ӧ��
	score[1] = IIC_Read_Byte();	//������
	//���������ֽ�
	IIC_Write_Byte(0x02);	//���ݵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Start();			//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);	//7λ��ַλ���������λ��дλ�������
	IIC_ACK();				//Ӧ��
	score[2] = IIC_Read_Byte();	//������
	//�����ĸ��ֽ�
	IIC_Write_Byte(0x03);	//���ݵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Start();			//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);	//7λ��ַλ���������λ��дλ�������	//0xa1��1010 0001
	IIC_ACK();				//Ӧ��
	score[3] = IIC_Read_Byte();	//������
	//��������ֽ�
	IIC_Write_Byte(0x04);	//���ݵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Start();			//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);	//7λ��ַλ���������λ��дλ�������
	IIC_ACK();				//Ӧ��
	score[4] = IIC_Read_Byte();	//������
	//���������ֽ�
	IIC_Write_Byte(0x05);	//���ݵ�ַ
	IIC_ACK();				//Ӧ��
	IIC_Start();			//IIC��ʼ�ź�
	IIC_Write_Byte(0xa1);	//7λ��ַλ���������λ��дλ�������
	IIC_ACK();				//Ӧ��
	score[5] = IIC_Read_Byte();	//������
	
	IIC_Stop();				//IIC�����ź�
}