#include "game.h"
#include "LCD12864.h"
#include "select.h"
#include "menu.h"
#include <reg52.h>
#include "usart.h"
#include "score.h"
#include "eeprom.h"
#include "information.h"

//光标移动坐标
uchar tower_s=2;	//（1,2）
uchar tower_x=6;	//（0,1,2,3,4,5,6）
uchar tower_y=2;	//（2,6）
//野怪行进坐标
uchar monster_x=7;	//（0,1,2,3,4,5,6,7）
uchar monster_y=0;	//（0,2,4）
uchar monster_s=2;	//（1,2）
uchar monster2_x=7;	//二号野怪的坐标
uchar monster2_y=0;
uchar monster2_s=2;
uchar monster3_x=7;	//三号野怪的坐标
uchar monster3_y=0;
uchar monster3_s=2;
//标记量
uchar flag=0;		//标记上次移动光标是否有放置了防御塔
uchar score_temp=0;	//关卡得分
uchar money=2;		//金钱
uchar round=0;		//敌人进攻轮次
uchar flag_magic=0;	//标记选中的功能道具
uchar flag_tower=1;	//标记要放置的防御塔
uchar flag_1=0;
uchar flag_2=0;
uchar flag_3=0;
//血量
char hp1=2;		//一号野怪的血量
char hp2=4;		//二号野怪的血量
char hp3=6;		//三号野怪的血量
char hp_home=3;	//基地的血量
//防御塔的坐标
towers_xy tower_xy[12];		//防御塔坐标

//初始化游戏参数（添加野怪数量时要修改）
void init_game()
{
	init_tower();		//置空防御塔坐标
	init_monster();		//置空野怪
	init_flag();		//置空其他标记量
	init_magic();		//绘出道具栏
	init_information();	//初始化游戏信息
}

//第一关游戏界面
void game1_1()
{
	flag_1=0;
	flag_2=0;
	round=2;
	round_num(round);
	hp1=2;
	//第一波只野怪（一只1）
	while(1)	//野怪走到最左端则退出
	{
		game_monster1_go(monster_s,monster_x);	//刷新野怪
		//为野怪做血量
		if(is_tower_ud(monster_x))	//附近有防御塔，血量-1（防御塔伤害不叠加）
		{
			hp1-=is_tower_ud(monster_x);
			if(hp1<=0)	//血量为0，继续下一只野怪
			{
				game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
				score_temp++;	//击杀一只野怪得分+1
				game_score(score_temp);
				money++;		//击杀一只野怪金钱+1
				money_num(money);
				//若当前分数高于历史最高分，覆盖
				if(score_temp>score[0])
				{
					score[0]=score_temp;
					write_eeprom(score);	//保存数据
				}
				break;
			}
		}
		//为基地做血量
		if(monster_x==0)	//野怪走到最左端，基地血量-1，抹去野怪
		{
			game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
			hp_home--;
			hp_num(hp_home);
			if(hp_home==0)
			{
				score_temp=0;	//失败分数为0
				game_score(score_temp);
				gamefailure();
				return;
			}
			break;
		}
		monster_x--;
		if(monster_x==3)	//走到中间，切换半屏
			monster_s=1;
		delay(2000);
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=2;
	hp2=2;
	//第二波野怪（两只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			game_monster1_go(monster_s,monster_x);	//刷新野怪
			//为野怪做血量
			if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//血量为0，继续下一只野怪
				{
					game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
					score_temp++;	//击杀一只野怪得分+1
					game_score(score_temp);
					money++;		//击杀一只野怪金钱+1
					money_num(money);
					//若当前分数高于历史最高分，覆盖
					if(score_temp>score[0])
					{
						score[0]=score_temp;
						write_eeprom(score);	//保存数据
					}
					flag_1=1;		//标记野怪1不在进行刷新
				}
			}
			//为基地做血量
			if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
			{
				game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
				hp_home--;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//失败分数为0
					game_score(score_temp);
					gamefailure();
					return;
				}
				flag_1=1;		//标记野怪1不在进行刷新
			}
			monster_x--;
			if(monster_x==3)	//走到中间，切换半屏
				monster_s=1;
		}
		delay(2000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[0])
						{
							score[0]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪1不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(2000);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=2;
	hp2=2;
	flag_1=0;
	flag_2=0;
	//第三波野怪（两只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			game_monster1_go(monster_s,monster_x);	//刷新野怪
			//为野怪做血量
			if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//血量为0，继续下一只野怪
				{
					game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
					score_temp++;	//击杀一只野怪得分+1
					game_score(score_temp);
					money++;		//击杀一只野怪金钱+1
					money_num(money);
					//若当前分数高于历史最高分，覆盖
					if(score_temp>score[0])
					{
						score[0]=score_temp;
						write_eeprom(score);	//保存数据
					}
					flag_1=1;		//标记野怪1不在进行刷新
				}
			}
			//为基地做血量
			if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
			{
				game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
				hp_home--;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//失败分数为0
					game_score(score_temp);
					gamefailure();
					return;
				}
				flag_1=1;		//标记野怪1不在进行刷新
			}
			monster_x--;
			if(monster_x==3)	//走到中间，切换半屏
				monster_s=1;
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[0])
						{
							score[0]=score_temp;
							write_eeprom(score);	//保存数据
						}
						//break;
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪1不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		if(flag_1==1&&flag_2==1)
			break;
	}
	game_score(score_temp);
	if(hp_home>0)
		gamevictory();
}

//第二关游戏界面
void game1_2()
{
	flag_1=0;
	flag_2=0;
	round=2;
	round_num(round);
	hp1=2;
	hp2=2;
	//第一波野怪（两只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			game_monster1_go(monster_s,monster_x);	//刷新野怪
			//为野怪做血量
			if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//血量为0，继续下一只野怪
				{
					game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
					score_temp++;	//击杀一只野怪得分+1
					game_score(score_temp);
					money++;		//击杀一只野怪金钱+1
					money_num(money);
					//若当前分数高于历史最高分，覆盖
					if(score_temp>score[1])
					{
						score[1]=score_temp;
						write_eeprom(score);	//保存数据
					}
					flag_1=1;		//标记野怪1不在进行刷新
				}
			}
			//为基地做血量
			if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
			{
				game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
				hp_home--;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//失败分数为0
					game_score(score_temp);
					gamefailure();
					return;
				}
				flag_1=1;		//标记野怪1不在进行刷新
			}
			monster_x--;
			if(monster_x==3)	//走到中间，切换半屏
				monster_s=1;
		}
		delay(1500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[1])
						{
							score[1]=score_temp;
							write_eeprom(score);	//保存数据
						}
						//break;
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					//break;
					flag_2=1;		//标记野怪1不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=4;
	//第二波野怪（一只2）
	while(1)	//野怪走到最左端则退出
	{
		game_monster2_go(monster_s,monster_x);	//刷新野怪
		//为野怪做血量
		if(is_tower_ud(monster_x))	//附近有防御塔，血量-1（防御塔伤害不叠加）
		{
			hp1-=is_tower_ud(monster_x);
			if(hp1<=0)	//血量为0，继续下一只野怪
			{
				game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
				score_temp++;	//击杀一只野怪得分+1
				game_score(score_temp);
				money++;		//击杀一只野怪金钱+1
				money_num(money);
				//若当前分数高于历史最高分，覆盖
				if(score_temp>score[1])
				{
					score[1]=score_temp;
					write_eeprom(score);	//保存数据
				}
				break;
			}
		}
		//为基地做血量
		if(monster_x==0)	//野怪走到最左端，基地血量-1，抹去野怪
		{
			game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
			hp_home-=2;
			hp_num(hp_home);
			if(hp_home==0)
			{
				score_temp=0;	//失败分数为0
				game_score(score_temp);
				gamefailure();
				return;
			}
			break;
		}
		monster_x--;
		if(monster_x==3)	//走到中间，切换半屏
			monster_s=1;
		delay(2000);
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=4;
	hp2=4;
	flag_1=0;
	flag_2=0;
	//第三波只野怪（两只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			game_monster2_go(monster_s,monster_x);	//刷新野怪
			//为野怪做血量
			if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//血量为0，继续下一只野怪
				{
					game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
					score_temp++;	//击杀一只野怪得分+1
					game_score(score_temp);
					money++;		//击杀一只野怪金钱+1
					money_num(money);
					//若当前分数高于历史最高分，覆盖
					if(score_temp>score[1])
					{
						score[1]=score_temp;
						write_eeprom(score);	//保存数据
					}
					flag_1=1;		//标记野怪1不在进行刷新
				}
			}
			//为基地做血量
			if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
			{
				game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
				hp_home-=2;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//失败分数为0
					game_score(score_temp);
					gamefailure();
					return;
				}
				flag_1=1;		//标记野怪1不在进行刷新
			}
			monster_x--;
			if(monster_x==3)	//走到中间，切换半屏
				monster_s=1;
		}
		delay(1500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[1])
						{
							score[1]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪1不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==1&&flag_2==1)
			break;
	}
	game_score(score_temp);	
	if(hp_home>0)
		gamevictory();
}
//第三关游戏界面
void game1_3()
{
	flag_1=0;
	flag_2=0;
	round=3;
	round_num(round);
	hp1=2;
	hp2=2;
	//第一波野怪（两只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster1_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster1_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_1=1;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=4;
	hp2=4;
	flag_1=0;
	flag_2=0;
	//第二波野怪（两只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_1=1;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=4;
	hp2=4;
	flag_1=0;
	flag_2=0;
	//第三波野怪（两只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_1=1;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=6;
	//第四波野怪（一只3）
	while(1)
	{
		if(monster_y<=4)	//野怪从上往下走
		{
			game_monster3_down(monster_y);	//刷新野怪
			//为野怪做血量
			if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
			{
				hp1-=is_tower_l(monster_x,monster_y);
				if(hp1<=0)	//血量为0
				{
					game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
					score_temp++;	//击杀一只野怪得分+1
					game_score(score_temp);
					money++;		//击杀一只野怪金钱+1
					money_num(money);
					game_score(score_temp);
					//若当前分数高于历史最高分，覆盖
					if(score_temp>score[2])
					{
						score[2]=score_temp;
						write_eeprom(score);	//保存数据
					}
					break;
				}
			}
			monster_y+=2;
			delay(2000);
		}
		else		//野怪从右往左走
		{
			game_monster3_go(monster_s,monster_x);	//刷新野怪
			//为野怪做血量
			if(is_tower_ud(monster_x))	//附近有防御塔（防御塔伤害叠加）
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//血量为0，游戏结束
				{
					game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
					score_temp++;	//击杀一只野怪得分+1
					game_score(score_temp);
					money++;		//击杀一只野怪金钱+1
					money_num(money);
					//若当前分数高于历史最高分，覆盖
					if(score_temp>score[2])
					{
						score[2]=score_temp;
						write_eeprom(score);	//保存数据
					}
					break;
				}
			}
			//为基地做血量
			if(monster_x==0)	//野怪2走到最左端，基地血量-2，抹去野怪
			{
				hp_home-=2;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//失败分数为0
					game_score(score_temp);
					gamefailure();
					return;
				}
				break;
			}
			monster_x--;
			if(monster_x==3)	//走到中间，切换半屏
				monster_s=1;
			delay(2000);
		}
	}
	game_score(score_temp);
	if(hp_home>0)
		gamevictory();	
}
//第四关游戏界面
void game1_4()
{
	flag_1=0;
	flag_2=0;
	flag_3=0;
	round=4;
	round_num(round);
	hp1=2;
	hp2=2;
	//第一波野怪（两只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster1_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster1_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[3])
			{
				score[3]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=4;
	hp2=4;
	flag_1=0;
	flag_2=0;
	//第二波野怪（两只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
					hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==2&&flag_2==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[3])
			{
				score[3]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=4;
	hp2=4;
	hp3=4;
	flag_1=0;
	flag_2=0;
	flag_3=0;
	//第三波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1500);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[3])
			{
				score[3]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=4;
	hp2=4;
	hp3=4;
	flag_1=0;
	flag_2=0;
	flag_3=0;
	//第四波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//保存数据
						}
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_1=1;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2==1)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_3=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==1&&flag_2==1&&flag_3==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//置空野怪
	hp1=6;
	hp2=6;
	flag_1=0;
	flag_2=0;
	//第五波野怪（两只3）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster3_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//保存数据
						}
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster3_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_1=1;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster3_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==1&&flag_2==1)
			break;
	}
	game_score(score_temp);
	if(hp_home>0)
		gamevictory();
}

void game1_5()
{
	round=5;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=2;
	hp2=2;
	hp3=2;
	//第一波野怪（三只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster1_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster1_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster1_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=2;
	hp2=2;
	hp3=2;
	//第二波野怪（三只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster1_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster1_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(500);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster1_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=4;
	hp2=4;
	hp3=4;
	//第三波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1500);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=4;
	hp2=4;
	hp3=4;
	//第四波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=4;
	hp2=4;
	hp3=4;
	//第五波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(500);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=6;
	hp2=6;
	hp3=6;
	//第六波野怪（三只3）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster3_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster3_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[4])
						{
							score[4]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_1=1;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster3_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[4])
						{
							score[4]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_2=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2==1)
			{
				game_monster3_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						score_temp++;	//击杀一只野怪得分+1
						game_score(score_temp);
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						//若当前分数高于历史最高分，覆盖
						if(score_temp>score[4])
						{
							score[4]=score_temp;
							write_eeprom(score);	//保存数据
						}
						flag_3=1;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==1&&flag_2==1&&flag_3==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=6;
	hp2=6;
	hp3=6;
	game_score(score_temp);
	if(hp_home>0)
		gamevictory();
}

void game1_6()
{
	round=7;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=2;
	hp2=2;
	hp3=2;
	//第一波野怪（三只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster1_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster1_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster1_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=2;
	hp2=2;
	hp3=2;
	//第二波野怪（三只1）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster1_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster1_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(500);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster1_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=4;
	hp2=4;
	hp3=4;
	//第三波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=4;
	hp2=4;
	hp3=4;
	//第四波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
					flag_1=2;
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(500);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=4;
	hp2=4;
	hp3=4;
	//第五波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(500);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=4;
	hp2=4;
	hp3=4;
	//第六波野怪（三只2）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster2_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster2_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(250);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(250);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(250);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=6;
	hp2=6;
	hp3=6;
	//第七波野怪（三只3）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster3_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster3_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(1000);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster3_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(1000);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster3_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//击杀一波野怪得分+1
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();
	flag_1=0;
	flag_2=0;
	flag_3=0;
	hp1=6;
	hp2=6;
	hp3=6;
	//第八波野怪（三只3）
	while(1)	//野怪走到最左端则退出
	{
		//第一只野怪
		if(flag_1==0)
		{
			if(monster_y<=4)	//垂直
			{
				game_monster3_down(monster_y);	//刷新野怪
				//为野怪做血量
				if(is_tower_l(monster_x,monster_y))	//附近有防御塔，血量-1（防御塔伤害不叠加）
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//血量为0，直接结束游戏
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//水平
			{
				game_monster3_go(monster_s,monster_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster_s,monster_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_1=2;		//标记野怪1不再进行刷新
					}
				}
				//为基地做血量
				if(monster_x==0&&flag_1==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster_s,monster_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//标记野怪1不再进行刷新
				}
				monster_x--;
				if(monster_x==3)	//走到中间，切换半屏
					monster_s=1;
			}
		}
		delay(500);
		//第二只野怪
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster3_go(monster2_s,monster2_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster2_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster2_s,monster2_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_2=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster2_x==0&&flag_2==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster2_s,monster2_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//标记野怪2不在进行刷新
				}
				monster2_x--;
				if(monster2_x==3)	//走到中间，切换半屏
					monster2_s=1;
			}
		}
		delay(500);
		//第三只野怪
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster3_go(monster3_s,monster3_x);	//刷新野怪
				//为野怪做血量
				if(is_tower_ud(monster3_x))	//附近有防御塔，判断有几个（防御塔伤害叠加）
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//血量为0，继续下一只野怪
					{
						game_monster_die(monster3_s,monster3_x);	//抹去死亡的野怪
						money++;		//击杀一只野怪金钱+1
						money_num(money);
						flag_3=2;		//标记野怪2不在进行刷新
					}
				}
				//为基地做血量
				if(monster3_x==0&&flag_3==0)	//野怪走到最左端，基地血量-1，抹去野怪
				{
					game_monster_get(monster3_s,monster3_x);		//抹去死亡的野怪
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//失败分数为0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//标记野怪2不在进行刷新
				}
				monster3_x--;
				if(monster3_x==3)	//走到中间，切换半屏
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp+=2;	//击杀一波野怪得分+2
			game_score(score_temp);
			//若当前分数高于历史最高分，覆盖
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//保存数据
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	game_score(score_temp);
	if(hp_home>0)
		gamevictory();
}

//野怪1直线行进（左->右）（第三行）
void game_monster1_go(uchar s,uchar x)
{
	uint8 code ch0[]=
	{
		0x00,0x00,0xE0,0x10,0x10,0x3C,0x5C,0x5C,0x3C,0x14,0x14,0x74,0xC4,0x80,0x00,0x00,
		0x00,0x00,0x07,0x1C,0x3C,0x1C,0x04,0x04,0x04,0x04,0x1C,0x3C,0x1D,0x07,0x00,0x00,/*"小野怪1",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(s,4,(x+1)*16,ch0+32*1);
	show_ch(s,4,x*16,ch0+32*0);
	
	//当这一野怪在左半屏，上一野怪在右半屏时，手动抹去上一野怪
	if(x==3)
	{
		show_ch(2,4,4*16,ch0+32*1);
	}
}
//野怪1直线行进（上->下）（右半屏最右）
void game_monster1_down(uchar x)
{
	uint8 code ch0[]=
	{
		0x00,0x00,0xE0,0x10,0x10,0x3C,0x5C,0x5C,0x3C,0x14,0x14,0x74,0xC4,0x80,0x00,0x00,
		0x00,0x00,0x07,0x1C,0x3C,0x1C,0x04,0x04,0x04,0x04,0x1C,0x3C,0x1D,0x07,0x00,0x00,/*"小野怪1",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(2,(x-2),7*16,ch0+32*1);
	show_ch(2,x,7*16,ch0+32*0);
}

//野怪2直线行进（左->右）（第三行）
void game_monster2_go(uchar s,uchar x)
{
	uint8 code ch0[]=
	{
		0x00,0x08,0x88,0xB8,0xEC,0x4C,0x7C,0xCC,0xCC,0x7C,0x4C,0xEC,0xB8,0x88,0x08,0x00,
		0x00,0x02,0x0B,0x0F,0x3F,0x3F,0x39,0x09,0x09,0x39,0x3F,0x3F,0x0F,0x0B,0x02,0x00,/*"野怪2",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(s,4,(x+1)*16,ch0+32*1);
	show_ch(s,4,x*16,ch0+32*0);
	
	//当这一野怪在左半屏，上一野怪在右半屏时，手动抹去上一野怪
	if(x==3)
	{
		show_ch(2,4,4*16,ch0+32*1);
	}
}
//野怪2直线行进（上->下）（右半屏最右）
void game_monster2_down(uchar x)
{
	uint8 code ch0[]=
	{
		0x00,0x08,0x88,0xB8,0xEC,0x4C,0x7C,0xCC,0xCC,0x7C,0x4C,0xEC,0xB8,0x88,0x08,0x00,
		0x00,0x02,0x0B,0x0F,0x3F,0x3F,0x39,0x09,0x09,0x39,0x3F,0x3F,0x0F,0x0B,0x02,0x00,/*"野怪2",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(2,(x-2),7*16,ch0+32*1);
	show_ch(2,x,7*16,ch0+32*0);
}

//野怪3直线行进（左->右）（第三行）
void game_monster3_go(uchar s,uchar x)
{
	uint8 code ch0[]=
	{
		0x80,0x80,0x1C,0x7C,0x6C,0x48,0xF8,0xCF,0xCF,0xF9,0x49,0x6C,0x7C,0x1C,0x80,0x80,
		0x00,0x03,0x02,0xCE,0xCE,0x6E,0x72,0x03,0x03,0x72,0x6E,0xCE,0xCE,0x02,0x03,0x00,/*"野怪3",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(s,4,(x+1)*16,ch0+32*1);
	show_ch(s,4,x*16,ch0+32*0);
	
	//当这一野怪在左半屏，上一野怪在右半屏时，手动抹去上一野怪
	if(x==3)
	{
		show_ch(2,4,4*16,ch0+32*1);
	}
}
//野怪3直线行进（上->下）（右半屏最右）
void game_monster3_down(uchar x)
{
	uint8 code ch0[]=
	{
		0x80,0x80,0x1C,0x7C,0x6C,0x48,0xF8,0xCF,0xCF,0xF9,0x49,0x6C,0x7C,0x1C,0x80,0x80,
		0x00,0x03,0x02,0xCE,0xCE,0x6E,0x72,0x03,0x03,0x72,0x6E,0xCE,0xCE,0x02,0x03,0x00,/*"野怪3",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(2,(x-2),7*16,ch0+32*1);
	show_ch(2,x,7*16,ch0+32*0);
}
//抹去野怪（野怪死亡）
void game_monster_die(uchar s,uchar x)
{
	uint8 code ch0[]=
	{	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",0*/
		
		0x00,0x00,0x08,0x08,0x38,0x70,0xC0,0x80,0x80,0x40,0x20,0x18,0x08,0x08,0x00,0x00,
		0x00,0x20,0x20,0x30,0x38,0x24,0x02,0x01,0x03,0x06,0x2C,0x38,0x30,0x20,0x20,0x00,/*"X ",1*/
	};
	
	show_ch(s,4,x*16,ch0+32*1);
	delay(1000);
	show_ch(s,4,x*16,ch0+32*0);
}
//抹去野怪（野怪到达基地）
void game_monster_get(uchar s,uchar x)
{
	uint8 code ch0[]=
	{	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",0*/
		
		0x80,0x80,0x80,0xE0,0x22,0x30,0x10,0x88,0x88,0x20,0x20,0x18,0x00,0x00,0x00,0x00,
		0x01,0x01,0x01,0x07,0x44,0x0C,0x08,0x11,0x11,0x04,0x04,0x18,0x00,0x00,0x00,0x00,/*"特效",0*/
	};
	
	show_ch(s,4,x*16,ch0+32*1);
	delay(1000);
	show_ch(s,4,x*16,ch0+32*0);
}

//放一座防御塔1
void game_tower()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x00,0x00,0x3C,0xFC,0xF4,0x10,0x10,0xF4,0xFC,0x3C,0x00,0x00,0x00,0x00,
		0x00,0x00,0x30,0x30,0x3C,0x3D,0x05,0x07,0x07,0x05,0x3D,0x3C,0x30,0x30,0x00,0x00,/*"防御塔1",0*/
	};
	
	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}
//放一座防御塔2
void game_tower2()
{
	uint8 code ch0[]=
	{
		0x00,0x0E,0x0C,0x18,0x38,0xF0,0x30,0xF6,0xF6,0x30,0xF0,0x38,0x18,0x0C,0x0E,0x00,
		0x20,0x20,0x38,0x2E,0x3E,0x27,0x39,0x3F,0x3F,0x39,0x27,0x3E,0x2E,0x38,0x20,0x20,/*"防御塔2",0*/
	};
	
	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}
//放一座防御塔3
void game_tower3()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x0C,0x0C,0xE0,0x30,0x18,0x8B,0x8B,0x18,0x30,0xE0,0x0C,0x0C,0x00,0x00,
		0x00,0x00,0x20,0x3C,0x27,0x38,0x20,0x21,0x21,0x20,0x38,0x27,0x3C,0x20,0x00,0x00,/*"防御塔3",0*/
	};
	
	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}

//画出光标
void game_cursor_draw()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xF0,0xF0,0x80,0x80,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x0F,0x0F,0x01,0x01,0x01,0x00,0x00,0x00,0x00/*"+",0*/
	};

	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}
//抹去光标
void game_cursor_clear()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",0*/
	};

	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}

//记录防御塔坐标
void write_tower()
{
	uchar i;
	for(i=0;i<12;i++)
	{
		if(tower_xy[i].x==99)
		{
			tower_xy[i].s=tower_s;
			tower_xy[i].x=tower_x;
			tower_xy[i].y=tower_y;
			tower_xy[i].flag=flag_tower;
			return;
		}
	}
}
//判断该位置上是否已放置防御塔（是：1	否：0）
int is_tower()
{
	uchar i;
	for(i=0;i<12;i++)
	{
		if((tower_xy[i].s==tower_s)&&(tower_xy[i].x==tower_x)&&(tower_xy[i].y==tower_y))
			return 1;
	}
	return 0;
}
//检测野怪 上/下 是否有防御塔（返回野怪受到的伤害）
int is_tower_ud(uchar x)
{
	uchar i,flag=0;
	for(i=0;i<12;i++)
	{
		if(tower_xy[i].x!=99)	//每个塔判断一次旁边有没有野怪
		{
			if(tower_xy[i].x==x)	//塔在 上/下
			{
				if(tower_xy[i].flag==1)		//防御塔1
				{
					if(tower_xy[i].y==2||tower_xy[i].y==6)
						flag++;
				}
				if(tower_xy[i].flag==2)		//防御塔2
				{
					if(tower_xy[i].y==2||tower_xy[i].y==6)
						flag+=2;
				}
				if(tower_xy[i].flag==3)
				{
					if(tower_xy[i].y==2||tower_xy[i].y==6)
						flag++;
				}
			}
			if(tower_xy[i].x==x-1||tower_xy[i].x==x+1)	//塔在 上/下 的 前/后 一格
			{
				if(tower_xy[i].flag==3)		//防御塔3
				{
					if(tower_xy[i].y==2||tower_xy[i].y==6)
						flag++;
				}
			}
		}
	}
	return flag;
}
//检测野怪 靠左 是否有防御塔（返回野怪受到的伤害）
int is_tower_l(uchar x,uchar y)
{
	uchar i;
	for(i=0;i<12;i++)
	{
		if(tower_xy[i].y!=99)
		{
			if((tower_xy[i].y==y)&&(tower_xy[i].x==(x-1)))
			{
				if(tower_xy[i].flag==1)
					return 1;
				if(tower_xy[i].flag==2)
					return 2;
				if(tower_xy[i].flag==3)
					return 1;
			}
		}
	}
	return 0;
}

//置空防御塔坐标
void init_tower()
{
	uchar i;
	for(i=0;i<12;i++)
	{
		tower_xy[i].s=99;
		tower_xy[i].x=99;
		tower_xy[i].y=99;
		tower_xy[i].flag=0;
	}
}
//置空指定防御塔坐标
void remove_tower()
{
	uchar i;
	for(i=0;i<12;i++)
	{
		if((tower_xy[i].s==tower_s)&&(tower_xy[i].x==tower_x)&&(tower_xy[i].y==tower_y))
		{
			tower_xy[i].s=99;
			tower_xy[i].x=99;
			tower_xy[i].y=99;
			tower_xy[i].flag=0;
		}
	}
}
//置空野怪坐标
void init_monster()
{
	monster_x=7;
	monster_y=0;
	monster_s=2;
	monster2_x=7;
	monster2_y=0;
	monster2_s=2;
	monster3_x=7;
	monster3_y=0;
	monster3_s=2;
}
//置空其他标记量
void init_flag()
{
	tower_s=2;
	tower_x=6;
	tower_y=2;
	
	flag_magic=0;
	flag_tower=1;
	
	hp_home=3;
	hp_num(hp_home);//绘制基地初始血量
	money=2;
	money_num(money);  //绘制初始金钱
	score_temp=0;
	game_score(10);	//绘制初始分数
}

//初始化外部中断0
void initex()
{
	IT0=1;
	EX0=1;
	EA=1;
}
//用于响应按键，实现移动光标、放置防御塔/道具
void ex_isr()	interrupt 0
{
	if(flag==0&&is_tower()==0&&tower_y!=0)
		game_cursor_clear();	//按下按键后，先抹去原来的光标
	flag=0;		//清除标记
	if(receive()==0xfd)	//return
	{
		while(1)
		{
			if(receive()==0xfe)
			{
				delay(1000);
				break;
			}
		}
		return;
	}
	if(receive()==0xfe)	//enter
	{
		//放置防御塔
		if(tower_y!=0&&is_tower()==0)	//光标不在道具栏且该位置无防御塔才能放置防御塔
		{	
			if(flag_tower==1&&money>0)
			{
				game_tower();	//放置防御塔1
				flag=1;			//做好标记
				write_tower();	//记录防御塔坐标
				money--;		//放置一个小塔金钱-1
				money_num(money);
			}
			if(flag_tower==2&&money>=2)
			{
				game_tower2();	//放置防御塔2
				flag=1;			//做好标记
				write_tower();	//记录防御塔坐标
				money-=2;		//放置一个大塔金钱-2
				money_num(money);
			}
			if(flag_tower==3&&money>=3)
			{
				game_tower3();	//放置防御塔3
				flag=1;			//做好标记
				write_tower();	//记录防御塔坐标
				money-=3;		//放置一个远程塔金钱-3
				money_num(money);
			}
			return;
		}
		//移除防御塔
		if(flag_tower==0&&is_tower()==1)	//该处有防御塔
		{
			if(money>0)
			{
				remove_tower();
				game_cursor_clear();
				money--;		//移除一个防御塔金钱-1
				money_num(money);
			}
			return;
			
		}
		//使用功能道具
		if(tower_y==0)
		{
			switch(flag_magic)
			{
				case 1:{//使用基地回血道具
					if(money>=2)
					{
						hp_home++;
						hp_num(hp_home);
						money-=2;
						money_num(money);
					}
					break;
				}
				case 2:{//选中防御塔2
					flag_tower=2;
					init_magic();
					break;
				}
				case 3:{//选中防御塔1
					flag_tower=1;
					init_magic();
					break;
				}
				case 5:{//选中防御塔3
					flag_tower=3;
					init_magic();
					break;
				}
				case 4:{//移除防御塔
					flag_tower=0;
					init_magic();
					break;
				}
				default:break;
			}
			return;
		}
	}
	if(receive()==0xef)	//switch
	{
		if(tower_y==0)	//光标在道具栏
		{	
			if(flag_magic==4||flag_magic==0)	//基地回血
			{
				init_recover();
				flag_magic=1;
				return;
			}
			if(flag_magic==1)					//切换防御塔
			{
				if(flag_tower==1||flag_tower==0)	//切换为防御塔2
				{
					init_bigtower();
					flag_magic=2;
					return;
				}
				if(flag_tower==2||flag_tower==0)	//切换为防御塔3
				{
					init_remotetower();
					flag_magic=5;
					return;
				}
				if(flag_tower==3||flag_tower==0)	//切换为防御塔1
				{
					init_smalltower();
					flag_magic=3;
					return;
				}
			}
			if(flag_magic==2||flag_magic==3||flag_magic==5)	//移除防御塔
			{
				init_remove();
				flag_magic=4;
				return;
			}
		}
	}
	if(receive()==0xf7)	//up
	{
		if(tower_y==2)	//光标在上行
		{
			if(tower_x==6)	//光标可以上去道具栏
				tower_y=0;
		}
		if(tower_y==6)	//光标在下行
			tower_y=2;
	}
	if(receive()==0xfb)	//down
	{
		if(tower_y==2)	//光标在上行
			tower_y=6;
		if(tower_y==0)	//光标在道具栏
			tower_y=2;
	}	
	if(receive()==0xdf)	//left
	{
		if(tower_x>0&&tower_y!=0)	//坐标未到达最左端且不在道具栏，可以左移
		{
			tower_x--;	//坐标左移
			{
				if(tower_x==3)	//坐标越过半屏
					tower_s=1;	//切换半屏
			}
		}
	}
	if(receive()==0xbf&&tower_y!=0)	//right
	{
		if(tower_x<6)	//坐标未到达最右端（倒2）且不在道具栏，可以右移
		{
			tower_x++;	//坐标右移
			{
				if(tower_x==4)	//坐标越过半屏
					tower_s=2;	//切换半屏
			}
		}
	}
	delay(100);
	if(is_tower()==0&&tower_y!=0)	//这个位置上没有防御塔
		game_cursor_draw();	//调整光标位置后，再画出新光标
}