#ifndef _ARROW_H
#define _ARROW_H

void shift(uint8 inx1,uint8 inx11);		//移动光标（开始菜单）
//移动光标（关卡菜单）
void shift_game_clear_left(uint8 y);	//抹去原光标（左边）
void shift_game_clear_right(uint8 y);	//抹去原光标（右边）
void shift_game_draw_left(uint8 y);		//画出新光标（指向左边）
void shift_game_draw_right(uint8 y);	//画出新光标（指向右边）

#endif