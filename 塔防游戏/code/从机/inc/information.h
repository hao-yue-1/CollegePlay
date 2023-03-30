#ifndef _INFORMATION_H
#define _INFORMATION_H

#include "LCD12864.h"

void money_num(uint8 money);	//更新金钱
void hp_num(uint8 hp);			//更新基地生命值
void round_num(uint8 round);	//更新敌人剩余进攻波次
void init_information();		//初始化游戏信息

void init_recover();			//绘出回血道具
void init_bigtower();			//绘出大塔道具
void init_smalltower();			//绘出小塔道具
void init_remotetower();		//绘出远程塔道具
void init_remove();				//绘出移除道具
void init_magic();				//绘出道具栏

#endif