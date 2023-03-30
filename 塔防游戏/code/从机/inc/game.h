#ifndef _GAME_H
#define _GAME_H

#define uchar unsigned char
#define uint unsigned int
	
extern uchar score_temp;

//防御塔的坐标结构体
typedef struct node_tower
{
	uchar s;
	uchar x;
	uchar y;
	uchar flag;
}towers_xy;

void initex();		//初始化外部中断0

void game_monster1_go(uchar s,uchar x);	//野怪1直线行进（左->右）（第三行）
void game_monster1_down(uchar x);		//野怪1直线行进（上->下）（右半屏最右）
void game_monster2_go(uchar s,uchar x);	//野怪2直线行进（左->右）（第三行）
void game_monster2_down(uchar x);		//野怪2直线行进（上->下）（右半屏最右）
void game_monster3_go(uchar s,uchar x);	//野怪3直线行进（左->右）（第三行）
void game_monster3_down(uchar x);		//野怪3直线行进（上->下）（右半屏最右）
void game_monster_die(uchar s,uchar x);	//抹去野怪（野怪死亡）
void game_monster_get(uchar s,uchar x);	//抹去野怪（野怪到达基地）

void game_tower();			//放一座防御塔
void game_tower2();			//放一座防御塔（大塔）
void game_tower3();			//放一座防御塔（远程塔）
void game_cursor_draw();	//画出光标
void game_cursor_clear();	//抹去光标

void game1_1();	//第一关游戏界面
void game1_2();	//第二关游戏界面
void game1_3();	//第三关游戏界面
void game1_4();	//第四关游戏界面
void game1_5();	//第五关游戏界面
void game1_6();	//第六关游戏界面

void init_game();		//初始化游戏参数
void init_tower();		//置空防御塔坐标
void remove_tower();	//置空指定防御塔坐标
void init_monster();	//置空野怪坐标
void init_flag();		//置空其他标记量

void write_tower();		//记录防御塔坐标
int is_tower();			//判断该位置上是否已放置防御塔
int is_tower_ud(uchar x);	//判断野怪 上/下 是否有防御塔
int is_tower_l(uchar x,uchar y);	//判断野怪 左 是否有防御塔

#endif