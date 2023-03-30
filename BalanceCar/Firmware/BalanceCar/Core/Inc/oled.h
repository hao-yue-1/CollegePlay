#ifndef _OLED_H
#define _OLED_H

//////////////////////////////////
//0.96���OLED��������
//////////////////////////////////

void OLED_send_cmd(unsigned char o_command);	//д����
void OLED_send_data(unsigned char o_data);		//д����
void OLED_clear(void);			//����OLED
void OLED_full(void);			//�������OLED
void OLED_init(void);			//��ʼ��OLED
void Picture_display(const unsigned char *ptr_pic);
void Picture_ReverseDisplay(const unsigned char *ptr_pic);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);

void OLED_ShowNum(unsigned char x,unsigned char y,int num,unsigned char len,unsigned char TextSize);
void OLED_ShowCh(unsigned char x,unsigned char y,unsigned char ch,unsigned char TextSize);
void OLED_ShowFloat(unsigned char x,unsigned char y,double num,unsigned char N);

#endif
