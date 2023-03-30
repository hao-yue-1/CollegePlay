#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

void print_hide();				//隐藏光标
void print_color(short x);		//选择字体颜色
void print_toxy(int x, int y);	//将光标移动到X,Y坐标处

void display();			//初始化显示界面
void display_4();		//日程管理菜单

#endif
