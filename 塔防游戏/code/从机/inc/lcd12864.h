#ifndef _LCD12864_H
#define _LCD12864_H

#define uint8 unsigned char
#define uint16 unsigned int

//ÉùÃ÷º¯Êı
void Read_busy();
void write_LCD_command(uint8);
void write_LCD_data(uint8);
void Set_page(uint8);
void Set_line(uint8);
void Set_column(uint8);
void SetOnOff(uint8);
void SelectScreen(uint8);
void ClearScreen(uint8);
void InitLCD();
void show_ch(uint8,uint8,uint8,uint8 *);

//void show_ch12(uint8 screen,uint8 page,uint8 column,uint8 *p);

#endif