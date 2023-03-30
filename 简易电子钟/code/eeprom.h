#ifndef _EEPROM_H
#define _EEPROM_H

#define uint unsigned int
#define uchar unsigned int

void WriteTimeEEPROM();				//把时钟的数据写进EEPROM中
void ReadTimeEEPROM();				//从EEPROM中读取时钟的数据

#endif