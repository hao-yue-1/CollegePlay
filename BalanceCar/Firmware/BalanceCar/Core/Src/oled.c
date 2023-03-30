#include "oled.h"
#include "myiic.h"
#include "delay.h"
#include "oled_font.h"

const unsigned char OLED_init_cmd[25]=
{
  /*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//�ر���ʾ
       0xD5,//����ʱ�ӷ�Ƶ����,��Ƶ��
       0x80,  //[3:0],��Ƶ����;[7:4],��Ƶ��

       0xA8,//��������·��
       0X3F,//Ĭ��0X3F(1/64)
       0xD3,//������ʾƫ��
       0X00,//Ĭ��Ϊ0
       0x40,//������ʾ��ʼ�� [5:0],����.                              
       0x8D,//��ɱ�����
       0x14,//bit2������/�ر�
       0x20,//�����ڴ��ַģʽ
       0x02,//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
       0xA1,//���ض�������,bit0:0,0->0;1,0->127;
       0xC8,//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
       0xDA,//����COMӲ����������
       0x12,//[5:4]����            
       0x81,//�Աȶ�����
       0xEF,//1~255;Ĭ��0X7F (��������,Խ��Խ��)
       0xD9,//����Ԥ�������
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
       0xDB,//����VCOMH ��ѹ����
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
       0xA4,//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
       0xA6,//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ        
       0xAF,//������ʾ     
};

//д����
void OLED_send_cmd(unsigned char o_command)
{
	IIC_Start();
    IIC_Send_Byte(0x78);//OLED��ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);//�Ĵ�����ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(o_command);
    IIC_Wait_Ack();
    IIC_Stop();
}
//д����
void OLED_send_data(unsigned char o_data)
{ 
	IIC_Start();
    IIC_Send_Byte(0x78);//OLED��ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(0x40);//�Ĵ�����ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(o_data);
    IIC_Wait_Ack();
    IIC_Stop();
}

void Column_set(unsigned char column)
{
	OLED_send_cmd(0x10|(column>>4));    //�����е�ַ��λ
	OLED_send_cmd(0x00|(column&0x0f));  //�����е�ַ��λ   
}
void Page_set(unsigned char page)
{
	OLED_send_cmd(0xb0+page);
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_clear
*	����˵��: ��������OLED����ʾ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_clear(void)
{
	unsigned char page,column;
	for(page=0;page<8;page++)             //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_send_data(0x00);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_full
*	����˵��: �������OLED����ʾ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_full(void)
{
	unsigned char page,column;
	for(page=0;page<8;page++)             //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_send_data(0xff);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_init
*	����˵��: ��ʼ��OLED
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_init(void)
{
	unsigned char i;
	IIC_Init();		//��ʼ��IIC
	for(i=0;i<25;i++)
	{
		OLED_send_cmd(OLED_init_cmd[i]);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Picture_display
*	����˵��: ��ʾͼƬ��128*64��
*	��    ��: *ptr_pic��ͼƬ�����ͷָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Picture_display(const unsigned char *ptr_pic)
{
	unsigned char page,column;
	for(page=0;page<(64/8);page++)        //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_send_data(*ptr_pic++);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Picture_ReverseDisplay
*	����˵��: ��ת����ʽ��ʾͼƬ��128*64��
*	��    ��: *ptr_pic��ͼƬ�����ͷָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Picture_ReverseDisplay(const unsigned char *ptr_pic)
{
	unsigned char page,column,data;
	for(page=0;page<(64/8);page++)        //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			data=*ptr_pic++;
			data=~data;
			OLED_send_data(data);
		}
	}
}

//���ù��λ��
void OLED_SetPos(unsigned char x, unsigned char y)
{ 
    OLED_send_cmd(0xb0+y);
    OLED_send_cmd(((x&0xf0)>>4)|0x10);
    OLED_send_cmd((x&0x0f)|0x01);
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ShowStr
*	����˵��: ��ʾ�ַ��������ֿ��е��ַ���
*	��    ��: 1. x,y����ʼ��ʾ����		//y=0,2,4,6��TextSize=2��  y=0,1,2,3,4,5,6,7��TextSize=1��
			  2. ch[]���ַ���    
			  3. TextSize���ַ���С��1:6*8  2:8*16��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<6;i++)
                    OLED_send_data(F6x8[c][i]);
                x += 6;
                j++;
            }
            break;
        }
        case 2:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<8;i++)
                    OLED_send_data(F8X16[c*16+i]);
                OLED_SetPos(x,y+1);
                for(i=0;i<8;i++)
                    OLED_send_data(F8X16[c*16+i+8]);
                x += 8;
                j++;
            }
            break;
        }
        default: break;
    }
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ShowCh
*	����˵��: ��ʾһ���ַ������ֿ��е��ַ���
*	��    ��: 1. x,y����ʼ��ʾ����		//y=0,2,4,6��TextSize=2��  y=0,1,2,3,4,5,6,7��TextSize=1��
			  2. ch���ַ�
			  3. TextSize���ַ���С��1:6*8  2:8*16��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_ShowCh(unsigned char x,unsigned char y,unsigned char ch,unsigned char TextSize)
{
    unsigned char c=0,i=0;
    c=ch-' ';//�õ�ƫ�ƺ��ֵ
    switch (TextSize)
    {
        case 1:
        {
            OLED_SetPos(x,y);
            for(i=0;i<6;i++)
            {
                OLED_send_data(F6x8[c][i]);
            }
            break;
        }
        case 2:
        {
            OLED_SetPos(x,y);
            for(i=0;i<8;i++)
                OLED_send_data(F8X16[c*16+i]);
            OLED_SetPos(x,y+1);
            for(i=0;i<8;i++)
            {
                OLED_send_data(F8X16[c*16+i+8]);
            }
            break;
        }
        default: break;
    }
}

//m^n����
int oled_pow(unsigned char m,unsigned char n)
{
    int result=1;
    while(n--)result*=m;
    return result;
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ShowNum
*	����˵��: ��ʾһ��int������
*	��    ��: 1. x,y����ʼ��ʾ����		//y=0,2,4,6��TextSize=2��  y=0,1,2,3,4,5,6,7��TextSize=1��
			  2. num������(0~4294967295)
			  3. len����������
			  4. TextSize���ַ���С��1:6*8  2:8*16��
*	�� �� ֵ: ��
*   ע    ��: ��������ʾ��ʽ�����Ҷ���ķ�ʽ����len����ʵ�ʵ�num�ĳ���ʱ��num�῿����ʾ���ճ����
*********************************************************************************************************
*/
void OLED_ShowNum(unsigned char x,unsigned char y,int num,unsigned char len,unsigned char TextSize)
{
    unsigned char t,temp;
    unsigned char enshow=0;
    switch (TextSize)
    {
        case 1:
        {
            for(t=0;t<len;t++)
            {
                temp=(num/oled_pow(10,len-t-1))%10;
                if(enshow==0&&t<(len-1))
                {
                    if(temp==0)
                    {
                        OLED_ShowCh(x+6*t,y,' ',TextSize);
                        continue;
                    }
                    else
                        enshow=1;

                }
                OLED_ShowCh(x+6*t,y,temp+'0',TextSize);
            }
            break;
        }
        case 2:
        {
            for(t=0;t<len;t++)
            {
                temp=(num/oled_pow(10,len-t-1))%10;
                if(enshow==0&&t<(len-1))
                {
                    if(temp==0)
                    {
                        OLED_ShowCh(x+8*t,y,' ',TextSize);
                        continue;
                    }
                    else
                        enshow=1;

                }
                OLED_ShowCh(x+8*t,y,temp+'0',TextSize);
            }
            break;
        }
        default : break;
    }
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ShowFloat
*	����˵��: ��ʾһ��double��С��
*	��    ��: 1. x,y����ʼ��ʾ����		//y=0,1,2,3,4,5,6,7��TextSize=1��
             2. num��double��С��
			 3. N��С�����λ��
*	�� �� ֵ: ��
*   ע    ��: �ú����ǳ����Ĳ������ĺ�������Ҫ�ǲ����Լ���������Ĵ�С��ֻ����6*8�Ĵ�С��ʾ    //TextSize=1
*********************************************************************************************************
*/
void OLED_ShowFloat(unsigned char x,unsigned char y,double num,unsigned char N)
{
    unsigned char   i_Count=1;
    unsigned char   n[12]={0};
    long   j=1;
    int    real_int=0;
    double decimal=0;
    unsigned int   real_decimal=0;
    if(num<0)
    {
        real_int=(int)(-num);
    }
    else
    {
        real_int=(int)num;
    }
    decimal=num-real_int;
    real_decimal=decimal*1e4;
    while(real_int/10/j!=0)
    {
        j=j*10;i_Count++;
    }
    n[0]=(real_int/10000)%10;
    n[1]=(real_int/1000)%10;
    n[2]=(real_int/100)%10;
    n[3]=(real_int/10)%10;
    n[4]=(real_int/1)%10;
    n[5]='.';
    n[6]=(real_decimal/1000)%10;
    n[7]=(real_decimal/100)%10;
    n[8]=(real_decimal/10)%10;
    n[9]=real_decimal%10;
    n[6+N]='\0';
    for(j=0;j<10;j++) n[j]=n[j]+16+32;
    if(num<0)
    {
        i_Count+=1;
        n[5-i_Count]='-';
    }
    n[5]='.';
    n[6+N]='\0';
    OLED_ShowStr(x,y,&n[5-i_Count],1);
}