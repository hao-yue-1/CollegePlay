#include "game.h"
#include "LCD12864.h"
#include "select.h"
#include "menu.h"
#include <reg52.h>
#include "usart.h"
#include "score.h"
#include "eeprom.h"
#include "information.h"

//����ƶ�����
uchar tower_s=2;	//��1,2��
uchar tower_x=6;	//��0,1,2,3,4,5,6��
uchar tower_y=2;	//��2,6��
//Ұ���н�����
uchar monster_x=7;	//��0,1,2,3,4,5,6,7��
uchar monster_y=0;	//��0,2,4��
uchar monster_s=2;	//��1,2��
uchar monster2_x=7;	//����Ұ�ֵ�����
uchar monster2_y=0;
uchar monster2_s=2;
uchar monster3_x=7;	//����Ұ�ֵ�����
uchar monster3_y=0;
uchar monster3_s=2;
//�����
uchar flag=0;		//����ϴ��ƶ�����Ƿ��з����˷�����
uchar score_temp=0;	//�ؿ��÷�
uchar money=2;		//��Ǯ
uchar round=0;		//���˽����ִ�
uchar flag_magic=0;	//���ѡ�еĹ��ܵ���
uchar flag_tower=1;	//���Ҫ���õķ�����
uchar flag_1=0;
uchar flag_2=0;
uchar flag_3=0;
//Ѫ��
char hp1=2;		//һ��Ұ�ֵ�Ѫ��
char hp2=4;		//����Ұ�ֵ�Ѫ��
char hp3=6;		//����Ұ�ֵ�Ѫ��
char hp_home=3;	//���ص�Ѫ��
//������������
towers_xy tower_xy[12];		//����������

//��ʼ����Ϸ���������Ұ������ʱҪ�޸ģ�
void init_game()
{
	init_tower();		//�ÿշ���������
	init_monster();		//�ÿ�Ұ��
	init_flag();		//�ÿ����������
	init_magic();		//���������
	init_information();	//��ʼ����Ϸ��Ϣ
}

//��һ����Ϸ����
void game1_1()
{
	flag_1=0;
	flag_2=0;
	round=2;
	round_num(round);
	hp1=2;
	//��һ��ֻҰ�֣�һֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
		//ΪҰ����Ѫ��
		if(is_tower_ud(monster_x))	//�����з�������Ѫ��-1���������˺������ӣ�
		{
			hp1-=is_tower_ud(monster_x);
			if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
			{
				game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
				score_temp++;	//��ɱһֻҰ�ֵ÷�+1
				game_score(score_temp);
				money++;		//��ɱһֻҰ�ֽ�Ǯ+1
				money_num(money);
				//����ǰ����������ʷ��߷֣�����
				if(score_temp>score[0])
				{
					score[0]=score_temp;
					write_eeprom(score);	//��������
				}
				break;
			}
		}
		//Ϊ������Ѫ��
		if(monster_x==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
		{
			game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
			hp_home--;
			hp_num(hp_home);
			if(hp_home==0)
			{
				score_temp=0;	//ʧ�ܷ���Ϊ0
				game_score(score_temp);
				gamefailure();
				return;
			}
			break;
		}
		monster_x--;
		if(monster_x==3)	//�ߵ��м䣬�л�����
			monster_s=1;
		delay(2000);
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=2;
	hp2=2;
	//�ڶ���Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
			//ΪҰ����Ѫ��
			if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
				{
					game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
					score_temp++;	//��ɱһֻҰ�ֵ÷�+1
					game_score(score_temp);
					money++;		//��ɱһֻҰ�ֽ�Ǯ+1
					money_num(money);
					//����ǰ����������ʷ��߷֣�����
					if(score_temp>score[0])
					{
						score[0]=score_temp;
						write_eeprom(score);	//��������
					}
					flag_1=1;		//���Ұ��1���ڽ���ˢ��
				}
			}
			//Ϊ������Ѫ��
			if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
			{
				game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
				hp_home--;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//ʧ�ܷ���Ϊ0
					game_score(score_temp);
					gamefailure();
					return;
				}
				flag_1=1;		//���Ұ��1���ڽ���ˢ��
			}
			monster_x--;
			if(monster_x==3)	//�ߵ��м䣬�л�����
				monster_s=1;
		}
		delay(2000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[0])
						{
							score[0]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��1���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(2000);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=2;
	hp2=2;
	flag_1=0;
	flag_2=0;
	//������Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
			//ΪҰ����Ѫ��
			if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
				{
					game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
					score_temp++;	//��ɱһֻҰ�ֵ÷�+1
					game_score(score_temp);
					money++;		//��ɱһֻҰ�ֽ�Ǯ+1
					money_num(money);
					//����ǰ����������ʷ��߷֣�����
					if(score_temp>score[0])
					{
						score[0]=score_temp;
						write_eeprom(score);	//��������
					}
					flag_1=1;		//���Ұ��1���ڽ���ˢ��
				}
			}
			//Ϊ������Ѫ��
			if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
			{
				game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
				hp_home--;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//ʧ�ܷ���Ϊ0
					game_score(score_temp);
					gamefailure();
					return;
				}
				flag_1=1;		//���Ұ��1���ڽ���ˢ��
			}
			monster_x--;
			if(monster_x==3)	//�ߵ��м䣬�л�����
				monster_s=1;
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[0])
						{
							score[0]=score_temp;
							write_eeprom(score);	//��������
						}
						//break;
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��1���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
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

//�ڶ�����Ϸ����
void game1_2()
{
	flag_1=0;
	flag_2=0;
	round=2;
	round_num(round);
	hp1=2;
	hp2=2;
	//��һ��Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
			//ΪҰ����Ѫ��
			if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
				{
					game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
					score_temp++;	//��ɱһֻҰ�ֵ÷�+1
					game_score(score_temp);
					money++;		//��ɱһֻҰ�ֽ�Ǯ+1
					money_num(money);
					//����ǰ����������ʷ��߷֣�����
					if(score_temp>score[1])
					{
						score[1]=score_temp;
						write_eeprom(score);	//��������
					}
					flag_1=1;		//���Ұ��1���ڽ���ˢ��
				}
			}
			//Ϊ������Ѫ��
			if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
			{
				game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
				hp_home--;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//ʧ�ܷ���Ϊ0
					game_score(score_temp);
					gamefailure();
					return;
				}
				flag_1=1;		//���Ұ��1���ڽ���ˢ��
			}
			monster_x--;
			if(monster_x==3)	//�ߵ��м䣬�л�����
				monster_s=1;
		}
		delay(1500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[1])
						{
							score[1]=score_temp;
							write_eeprom(score);	//��������
						}
						//break;
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					//break;
					flag_2=1;		//���Ұ��1���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=4;
	//�ڶ���Ұ�֣�һֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
		//ΪҰ����Ѫ��
		if(is_tower_ud(monster_x))	//�����з�������Ѫ��-1���������˺������ӣ�
		{
			hp1-=is_tower_ud(monster_x);
			if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
			{
				game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
				score_temp++;	//��ɱһֻҰ�ֵ÷�+1
				game_score(score_temp);
				money++;		//��ɱһֻҰ�ֽ�Ǯ+1
				money_num(money);
				//����ǰ����������ʷ��߷֣�����
				if(score_temp>score[1])
				{
					score[1]=score_temp;
					write_eeprom(score);	//��������
				}
				break;
			}
		}
		//Ϊ������Ѫ��
		if(monster_x==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
		{
			game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
			hp_home-=2;
			hp_num(hp_home);
			if(hp_home==0)
			{
				score_temp=0;	//ʧ�ܷ���Ϊ0
				game_score(score_temp);
				gamefailure();
				return;
			}
			break;
		}
		monster_x--;
		if(monster_x==3)	//�ߵ��м䣬�л�����
			monster_s=1;
		delay(2000);
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=4;
	hp2=4;
	flag_1=0;
	flag_2=0;
	//������ֻҰ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
			//ΪҰ����Ѫ��
			if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
				{
					game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
					score_temp++;	//��ɱһֻҰ�ֵ÷�+1
					game_score(score_temp);
					money++;		//��ɱһֻҰ�ֽ�Ǯ+1
					money_num(money);
					//����ǰ����������ʷ��߷֣�����
					if(score_temp>score[1])
					{
						score[1]=score_temp;
						write_eeprom(score);	//��������
					}
					flag_1=1;		//���Ұ��1���ڽ���ˢ��
				}
			}
			//Ϊ������Ѫ��
			if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
			{
				game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
				hp_home-=2;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//ʧ�ܷ���Ϊ0
					game_score(score_temp);
					gamefailure();
					return;
				}
				flag_1=1;		//���Ұ��1���ڽ���ˢ��
			}
			monster_x--;
			if(monster_x==3)	//�ߵ��м䣬�л�����
				monster_s=1;
		}
		delay(1500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[1])
						{
							score[1]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��1���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
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
//��������Ϸ����
void game1_3()
{
	flag_1=0;
	flag_2=0;
	round=3;
	round_num(round);
	hp1=2;
	hp2=2;
	//��һ��Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster1_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_1=1;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=4;
	hp2=4;
	flag_1=0;
	flag_2=0;
	//�ڶ���Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_1=1;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=4;
	hp2=4;
	flag_1=0;
	flag_2=0;
	//������Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_1=1;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[2])
						{
							score[2]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		if(flag_1==1&&flag_2==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=6;
	//���Ĳ�Ұ�֣�һֻ3��
	while(1)
	{
		if(monster_y<=4)	//Ұ�ִ���������
		{
			game_monster3_down(monster_y);	//ˢ��Ұ��
			//ΪҰ����Ѫ��
			if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
			{
				hp1-=is_tower_l(monster_x,monster_y);
				if(hp1<=0)	//Ѫ��Ϊ0
				{
					game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
					score_temp++;	//��ɱһֻҰ�ֵ÷�+1
					game_score(score_temp);
					money++;		//��ɱһֻҰ�ֽ�Ǯ+1
					money_num(money);
					game_score(score_temp);
					//����ǰ����������ʷ��߷֣�����
					if(score_temp>score[2])
					{
						score[2]=score_temp;
						write_eeprom(score);	//��������
					}
					break;
				}
			}
			monster_y+=2;
			delay(2000);
		}
		else		//Ұ�ִ���������
		{
			game_monster3_go(monster_s,monster_x);	//ˢ��Ұ��
			//ΪҰ����Ѫ��
			if(is_tower_ud(monster_x))	//�����з��������������˺����ӣ�
			{
				hp1-=is_tower_ud(monster_x);
				if(hp1<=0)	//Ѫ��Ϊ0����Ϸ����
				{
					game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
					score_temp++;	//��ɱһֻҰ�ֵ÷�+1
					game_score(score_temp);
					money++;		//��ɱһֻҰ�ֽ�Ǯ+1
					money_num(money);
					//����ǰ����������ʷ��߷֣�����
					if(score_temp>score[2])
					{
						score[2]=score_temp;
						write_eeprom(score);	//��������
					}
					break;
				}
			}
			//Ϊ������Ѫ��
			if(monster_x==0)	//Ұ��2�ߵ�����ˣ�����Ѫ��-2��ĨȥҰ��
			{
				hp_home-=2;
				hp_num(hp_home);
				if(hp_home<=0)
				{
					score_temp=0;	//ʧ�ܷ���Ϊ0
					game_score(score_temp);
					gamefailure();
					return;
				}
				break;
			}
			monster_x--;
			if(monster_x==3)	//�ߵ��м䣬�л�����
				monster_s=1;
			delay(2000);
		}
	}
	game_score(score_temp);
	if(hp_home>0)
		gamevictory();	
}
//���Ĺ���Ϸ����
void game1_4()
{
	flag_1=0;
	flag_2=0;
	flag_3=0;
	round=4;
	round_num(round);
	hp1=2;
	hp2=2;
	//��һ��Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster1_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[3])
			{
				score[3]=score_temp;
				write_eeprom(score);	//��������
			}
			break;
		}
		if(flag_1&&flag_2)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=4;
	hp2=4;
	flag_1=0;
	flag_2=0;
	//�ڶ���Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
					hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1500);
		if(flag_1==2&&flag_2==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[3])
			{
				score[3]=score_temp;
				write_eeprom(score);	//��������
			}
			break;
		}
		if(flag_1&&flag_2)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=4;
	hp2=4;
	hp3=4;
	flag_1=0;
	flag_2=0;
	flag_3=0;
	//������Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1500);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(1500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[3])
			{
				score[3]=score_temp;
				write_eeprom(score);	//��������
			}
			break;
		}
		if(flag_1&&flag_2&&flag_3)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=4;
	hp2=4;
	hp3=4;
	flag_1=0;
	flag_2=0;
	flag_3=0;
	//���Ĳ�Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//��������
						}
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_1=1;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2==1)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_3=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==1&&flag_2==1&&flag_3==1)
			break;
	}
	round--;
	round_num(round);
	init_monster();	//�ÿ�Ұ��
	hp1=6;
	hp2=6;
	flag_1=0;
	flag_2=0;
	//���岨Ұ�֣���ֻ3��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster3_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//��������
						}
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster3_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_1=1;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster3_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[3])
						{
							score[3]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
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
	//��һ��Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster1_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster1_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//��������
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
	//�ڶ���Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster1_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(500);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster1_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//��������
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
	//������Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1500);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(1500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//��������
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
	//���Ĳ�Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//��������
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
	//���岨Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(500);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[4])
			{
				score[4]=score_temp;
				write_eeprom(score);	//��������
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
	//������Ұ�֣���ֻ3��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster3_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster3_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[4])
						{
							score[4]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_1=1;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1==1)
			{
				game_monster3_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[4])
						{
							score[4]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_2=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2==1)
			{
				game_monster3_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						score_temp++;	//��ɱһֻҰ�ֵ÷�+1
						game_score(score_temp);
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						//����ǰ����������ʷ��߷֣�����
						if(score_temp>score[4])
						{
							score[4]=score_temp;
							write_eeprom(score);	//��������
						}
						flag_3=1;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
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
	//��һ��Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster1_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster1_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//��������
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
	//�ڶ���Ұ�֣���ֻ1��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster1_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster1_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster1_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(500);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster1_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home--;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//��������
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
	//������Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//��������
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
	//���Ĳ�Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
					flag_1=2;
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(500);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//��������
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
	//���岨Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(500);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//��������
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
	//������Ұ�֣���ֻ2��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster2_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster2_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(250);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster2_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(250);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster2_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(250);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//��������
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
	//���߲�Ұ�֣���ֻ3��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster3_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster3_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(1000);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster3_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(1000);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster3_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(1000);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp++;	//��ɱһ��Ұ�ֵ÷�+1
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//��������
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
	//�ڰ˲�Ұ�֣���ֻ3��
	while(1)	//Ұ���ߵ���������˳�
	{
		//��һֻҰ��
		if(flag_1==0)
		{
			if(monster_y<=4)	//��ֱ
			{
				game_monster3_down(monster_y);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_l(monster_x,monster_y))	//�����з�������Ѫ��-1���������˺������ӣ�
				{
						hp1-=is_tower_l(monster_x,monster_y);
					if(hp1<=0)	//Ѫ��Ϊ0��ֱ�ӽ�����Ϸ
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;
					}
				}
				monster_y+=2;
			}
			else	//ˮƽ
			{
				game_monster3_go(monster_s,monster_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp1-=is_tower_ud(monster_x);
					if(hp1<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster_s,monster_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_1=2;		//���Ұ��1���ٽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster_x==0&&flag_1==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster_s,monster_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_1=1;		//���Ұ��1���ٽ���ˢ��
				}
				monster_x--;
				if(monster_x==3)	//�ߵ��м䣬�л�����
					monster_s=1;
			}
		}
		delay(500);
		//�ڶ�ֻҰ��
		if(flag_2==0)
		{
			if(monster_x<monster2_x-1||flag_1)
			{
				game_monster3_go(monster2_s,monster2_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster2_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp2-=is_tower_ud(monster2_x);
					if(hp2<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster2_s,monster2_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_2=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster2_x==0&&flag_2==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster2_s,monster2_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_2=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster2_x--;
				if(monster2_x==3)	//�ߵ��м䣬�л�����
					monster2_s=1;
			}
		}
		delay(500);
		//����ֻҰ��
		if(flag_3==0)
		{
			if(monster2_x<monster3_x-1||flag_2)
			{
				game_monster3_go(monster3_s,monster3_x);	//ˢ��Ұ��
				//ΪҰ����Ѫ��
				if(is_tower_ud(monster3_x))	//�����з��������ж��м������������˺����ӣ�
				{
					hp3-=is_tower_ud(monster3_x);
					if(hp3<=0)	//Ѫ��Ϊ0��������һֻҰ��
					{
						game_monster_die(monster3_s,monster3_x);	//Ĩȥ������Ұ��
						money++;		//��ɱһֻҰ�ֽ�Ǯ+1
						money_num(money);
						flag_3=2;		//���Ұ��2���ڽ���ˢ��
					}
				}
				//Ϊ������Ѫ��
				if(monster3_x==0&&flag_3==0)	//Ұ���ߵ�����ˣ�����Ѫ��-1��ĨȥҰ��
				{
					game_monster_get(monster3_s,monster3_x);		//Ĩȥ������Ұ��
					hp_home-=2;
					hp_num(hp_home);
					if(hp_home<=0)
					{
						score_temp=0;	//ʧ�ܷ���Ϊ0
						game_score(score_temp);
						gamefailure();
						return;
					}
					flag_3=1;		//���Ұ��2���ڽ���ˢ��
				}
				monster3_x--;
				if(monster3_x==3)	//�ߵ��м䣬�л�����
					monster3_s=1;
			}
		}
		delay(500);
		if(flag_1==2&&flag_2==2&&flag_3==2)
		{
			score_temp+=2;	//��ɱһ��Ұ�ֵ÷�+2
			game_score(score_temp);
			//����ǰ����������ʷ��߷֣�����
			if(score_temp>score[5])
			{
				score[5]=score_temp;
				write_eeprom(score);	//��������
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

//Ұ��1ֱ���н�����->�ң��������У�
void game_monster1_go(uchar s,uchar x)
{
	uint8 code ch0[]=
	{
		0x00,0x00,0xE0,0x10,0x10,0x3C,0x5C,0x5C,0x3C,0x14,0x14,0x74,0xC4,0x80,0x00,0x00,
		0x00,0x00,0x07,0x1C,0x3C,0x1C,0x04,0x04,0x04,0x04,0x1C,0x3C,0x1D,0x07,0x00,0x00,/*"СҰ��1",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(s,4,(x+1)*16,ch0+32*1);
	show_ch(s,4,x*16,ch0+32*0);
	
	//����һҰ�������������һҰ�����Ұ���ʱ���ֶ�Ĩȥ��һҰ��
	if(x==3)
	{
		show_ch(2,4,4*16,ch0+32*1);
	}
}
//Ұ��1ֱ���н�����->�£����Ұ������ң�
void game_monster1_down(uchar x)
{
	uint8 code ch0[]=
	{
		0x00,0x00,0xE0,0x10,0x10,0x3C,0x5C,0x5C,0x3C,0x14,0x14,0x74,0xC4,0x80,0x00,0x00,
		0x00,0x00,0x07,0x1C,0x3C,0x1C,0x04,0x04,0x04,0x04,0x1C,0x3C,0x1D,0x07,0x00,0x00,/*"СҰ��1",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(2,(x-2),7*16,ch0+32*1);
	show_ch(2,x,7*16,ch0+32*0);
}

//Ұ��2ֱ���н�����->�ң��������У�
void game_monster2_go(uchar s,uchar x)
{
	uint8 code ch0[]=
	{
		0x00,0x08,0x88,0xB8,0xEC,0x4C,0x7C,0xCC,0xCC,0x7C,0x4C,0xEC,0xB8,0x88,0x08,0x00,
		0x00,0x02,0x0B,0x0F,0x3F,0x3F,0x39,0x09,0x09,0x39,0x3F,0x3F,0x0F,0x0B,0x02,0x00,/*"Ұ��2",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(s,4,(x+1)*16,ch0+32*1);
	show_ch(s,4,x*16,ch0+32*0);
	
	//����һҰ�������������һҰ�����Ұ���ʱ���ֶ�Ĩȥ��һҰ��
	if(x==3)
	{
		show_ch(2,4,4*16,ch0+32*1);
	}
}
//Ұ��2ֱ���н�����->�£����Ұ������ң�
void game_monster2_down(uchar x)
{
	uint8 code ch0[]=
	{
		0x00,0x08,0x88,0xB8,0xEC,0x4C,0x7C,0xCC,0xCC,0x7C,0x4C,0xEC,0xB8,0x88,0x08,0x00,
		0x00,0x02,0x0B,0x0F,0x3F,0x3F,0x39,0x09,0x09,0x39,0x3F,0x3F,0x0F,0x0B,0x02,0x00,/*"Ұ��2",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(2,(x-2),7*16,ch0+32*1);
	show_ch(2,x,7*16,ch0+32*0);
}

//Ұ��3ֱ���н�����->�ң��������У�
void game_monster3_go(uchar s,uchar x)
{
	uint8 code ch0[]=
	{
		0x80,0x80,0x1C,0x7C,0x6C,0x48,0xF8,0xCF,0xCF,0xF9,0x49,0x6C,0x7C,0x1C,0x80,0x80,
		0x00,0x03,0x02,0xCE,0xCE,0x6E,0x72,0x03,0x03,0x72,0x6E,0xCE,0xCE,0x02,0x03,0x00,/*"Ұ��3",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(s,4,(x+1)*16,ch0+32*1);
	show_ch(s,4,x*16,ch0+32*0);
	
	//����һҰ�������������һҰ�����Ұ���ʱ���ֶ�Ĩȥ��һҰ��
	if(x==3)
	{
		show_ch(2,4,4*16,ch0+32*1);
	}
}
//Ұ��3ֱ���н�����->�£����Ұ������ң�
void game_monster3_down(uchar x)
{
	uint8 code ch0[]=
	{
		0x80,0x80,0x1C,0x7C,0x6C,0x48,0xF8,0xCF,0xCF,0xF9,0x49,0x6C,0x7C,0x1C,0x80,0x80,
		0x00,0x03,0x02,0xCE,0xCE,0x6E,0x72,0x03,0x03,0x72,0x6E,0xCE,0xCE,0x02,0x03,0x00,/*"Ұ��3",0*/
		
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",1*/
	};
	
	show_ch(2,(x-2),7*16,ch0+32*1);
	show_ch(2,x,7*16,ch0+32*0);
}
//ĨȥҰ�֣�Ұ��������
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
//ĨȥҰ�֣�Ұ�ֵ�����أ�
void game_monster_get(uchar s,uchar x)
{
	uint8 code ch0[]=
	{	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",0*/
		
		0x80,0x80,0x80,0xE0,0x22,0x30,0x10,0x88,0x88,0x20,0x20,0x18,0x00,0x00,0x00,0x00,
		0x01,0x01,0x01,0x07,0x44,0x0C,0x08,0x11,0x11,0x04,0x04,0x18,0x00,0x00,0x00,0x00,/*"��Ч",0*/
	};
	
	show_ch(s,4,x*16,ch0+32*1);
	delay(1000);
	show_ch(s,4,x*16,ch0+32*0);
}

//��һ��������1
void game_tower()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x00,0x00,0x3C,0xFC,0xF4,0x10,0x10,0xF4,0xFC,0x3C,0x00,0x00,0x00,0x00,
		0x00,0x00,0x30,0x30,0x3C,0x3D,0x05,0x07,0x07,0x05,0x3D,0x3C,0x30,0x30,0x00,0x00,/*"������1",0*/
	};
	
	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}
//��һ��������2
void game_tower2()
{
	uint8 code ch0[]=
	{
		0x00,0x0E,0x0C,0x18,0x38,0xF0,0x30,0xF6,0xF6,0x30,0xF0,0x38,0x18,0x0C,0x0E,0x00,
		0x20,0x20,0x38,0x2E,0x3E,0x27,0x39,0x3F,0x3F,0x39,0x27,0x3E,0x2E,0x38,0x20,0x20,/*"������2",0*/
	};
	
	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}
//��һ��������3
void game_tower3()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x0C,0x0C,0xE0,0x30,0x18,0x8B,0x8B,0x18,0x30,0xE0,0x0C,0x0C,0x00,0x00,
		0x00,0x00,0x20,0x3C,0x27,0x38,0x20,0x21,0x21,0x20,0x38,0x27,0x3C,0x20,0x00,0x00,/*"������3",0*/
	};
	
	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}

//�������
void game_cursor_draw()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xF0,0xF0,0x80,0x80,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x0F,0x0F,0x01,0x01,0x01,0x00,0x00,0x00,0x00/*"+",0*/
	};

	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}
//Ĩȥ���
void game_cursor_clear()
{
	uint8 code ch0[]=
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"  ",0*/
	};

	show_ch(tower_s,tower_y,tower_x*16,ch0+32*0);
}

//��¼����������
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
//�жϸ�λ�����Ƿ��ѷ��÷��������ǣ�1	��0��
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
//���Ұ�� ��/�� �Ƿ��з�����������Ұ���ܵ����˺���
int is_tower_ud(uchar x)
{
	uchar i,flag=0;
	for(i=0;i<12;i++)
	{
		if(tower_xy[i].x!=99)	//ÿ�����ж�һ���Ա���û��Ұ��
		{
			if(tower_xy[i].x==x)	//���� ��/��
			{
				if(tower_xy[i].flag==1)		//������1
				{
					if(tower_xy[i].y==2||tower_xy[i].y==6)
						flag++;
				}
				if(tower_xy[i].flag==2)		//������2
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
			if(tower_xy[i].x==x-1||tower_xy[i].x==x+1)	//���� ��/�� �� ǰ/�� һ��
			{
				if(tower_xy[i].flag==3)		//������3
				{
					if(tower_xy[i].y==2||tower_xy[i].y==6)
						flag++;
				}
			}
		}
	}
	return flag;
}
//���Ұ�� ���� �Ƿ��з�����������Ұ���ܵ����˺���
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

//�ÿշ���������
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
//�ÿ�ָ������������
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
//�ÿ�Ұ������
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
//�ÿ����������
void init_flag()
{
	tower_s=2;
	tower_x=6;
	tower_y=2;
	
	flag_magic=0;
	flag_tower=1;
	
	hp_home=3;
	hp_num(hp_home);//���ƻ��س�ʼѪ��
	money=2;
	money_num(money);  //���Ƴ�ʼ��Ǯ
	score_temp=0;
	game_score(10);	//���Ƴ�ʼ����
}

//��ʼ���ⲿ�ж�0
void initex()
{
	IT0=1;
	EX0=1;
	EA=1;
}
//������Ӧ������ʵ���ƶ���ꡢ���÷�����/����
void ex_isr()	interrupt 0
{
	if(flag==0&&is_tower()==0&&tower_y!=0)
		game_cursor_clear();	//���°�������Ĩȥԭ���Ĺ��
	flag=0;		//������
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
		//���÷�����
		if(tower_y!=0&&is_tower()==0)	//��겻�ڵ������Ҹ�λ���޷��������ܷ��÷�����
		{	
			if(flag_tower==1&&money>0)
			{
				game_tower();	//���÷�����1
				flag=1;			//���ñ��
				write_tower();	//��¼����������
				money--;		//����һ��С����Ǯ-1
				money_num(money);
			}
			if(flag_tower==2&&money>=2)
			{
				game_tower2();	//���÷�����2
				flag=1;			//���ñ��
				write_tower();	//��¼����������
				money-=2;		//����һ��������Ǯ-2
				money_num(money);
			}
			if(flag_tower==3&&money>=3)
			{
				game_tower3();	//���÷�����3
				flag=1;			//���ñ��
				write_tower();	//��¼����������
				money-=3;		//����һ��Զ������Ǯ-3
				money_num(money);
			}
			return;
		}
		//�Ƴ�������
		if(flag_tower==0&&is_tower()==1)	//�ô��з�����
		{
			if(money>0)
			{
				remove_tower();
				game_cursor_clear();
				money--;		//�Ƴ�һ����������Ǯ-1
				money_num(money);
			}
			return;
			
		}
		//ʹ�ù��ܵ���
		if(tower_y==0)
		{
			switch(flag_magic)
			{
				case 1:{//ʹ�û��ػ�Ѫ����
					if(money>=2)
					{
						hp_home++;
						hp_num(hp_home);
						money-=2;
						money_num(money);
					}
					break;
				}
				case 2:{//ѡ�з�����2
					flag_tower=2;
					init_magic();
					break;
				}
				case 3:{//ѡ�з�����1
					flag_tower=1;
					init_magic();
					break;
				}
				case 5:{//ѡ�з�����3
					flag_tower=3;
					init_magic();
					break;
				}
				case 4:{//�Ƴ�������
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
		if(tower_y==0)	//����ڵ�����
		{	
			if(flag_magic==4||flag_magic==0)	//���ػ�Ѫ
			{
				init_recover();
				flag_magic=1;
				return;
			}
			if(flag_magic==1)					//�л�������
			{
				if(flag_tower==1||flag_tower==0)	//�л�Ϊ������2
				{
					init_bigtower();
					flag_magic=2;
					return;
				}
				if(flag_tower==2||flag_tower==0)	//�л�Ϊ������3
				{
					init_remotetower();
					flag_magic=5;
					return;
				}
				if(flag_tower==3||flag_tower==0)	//�л�Ϊ������1
				{
					init_smalltower();
					flag_magic=3;
					return;
				}
			}
			if(flag_magic==2||flag_magic==3||flag_magic==5)	//�Ƴ�������
			{
				init_remove();
				flag_magic=4;
				return;
			}
		}
	}
	if(receive()==0xf7)	//up
	{
		if(tower_y==2)	//���������
		{
			if(tower_x==6)	//��������ȥ������
				tower_y=0;
		}
		if(tower_y==6)	//���������
			tower_y=2;
	}
	if(receive()==0xfb)	//down
	{
		if(tower_y==2)	//���������
			tower_y=6;
		if(tower_y==0)	//����ڵ�����
			tower_y=2;
	}	
	if(receive()==0xdf)	//left
	{
		if(tower_x>0&&tower_y!=0)	//����δ����������Ҳ��ڵ���������������
		{
			tower_x--;	//��������
			{
				if(tower_x==3)	//����Խ������
					tower_s=1;	//�л�����
			}
		}
	}
	if(receive()==0xbf&&tower_y!=0)	//right
	{
		if(tower_x<6)	//����δ�������Ҷˣ���2���Ҳ��ڵ���������������
		{
			tower_x++;	//��������
			{
				if(tower_x==4)	//����Խ������
					tower_s=2;	//�л�����
			}
		}
	}
	delay(100);
	if(is_tower()==0&&tower_y!=0)	//���λ����û�з�����
		game_cursor_draw();	//�������λ�ú��ٻ����¹��
}