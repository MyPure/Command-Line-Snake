#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
int row = 20;
int column = 25;
int score = 0;
int snakelength = 0;
int snakelength0 = 0;//要求过关的蛇长

struct World {//链表网表示世界
	int locx;
	int locy;
	char direction;
	int display;
	struct World *up;
	struct World *down;
	struct World *left;
	struct World *right;
};

struct World *pt = NULL, *pw1 = NULL, *pw2 = NULL;
struct World * creatworld();//生成世界
int printinfo_and_getchoose(int, int, int);//打印信息并获得选择，参数分别为选择列表号数，菜单框起始xy坐标，要求过关达到的蛇长
char creatsnake(struct World *, int);//初始化蛇
_Bool snakemove(char);//蛇的移动
void printworld(struct World *);//打印世界并计算蛇长
void gotoxy(int, int);//移动光标
void movepw(int);//蛇尾移动
void printrectangle(int x0, int y0, int column, int row, _Bool add);//打印一个框，变量分别为横纵坐标起始值，长（横）宽（竖），添加还是删除
void printstrwithframe(int x0, int y0, char str[], _Bool add);//打印一个带框的字符串，变量分别为横纵坐标起始值，要打印的字符串，添加还是删除
void printcursor(int x0, int y0, _Bool add);//打印光标
int getcursorchoose(int, int, int);//移动光标，变量分为为第一个选择的横纵坐标，选择的个数
void initialize(struct World *, _Bool);//世界初始化
struct World *creatfood(struct World *);//生成食物
void clean(int x1, int y1, int x2, int y2);//清空某片区域
void gamestart(void);//开始游戏
void gamerunstagemode(void);//运行闯关模式
void gameruncustommode(int, int);//运行休闲模式
int judgescore(struct World *);//判断得分
void creatwall(struct World *, int);//生成墙
void creatvenomousgrass(struct World *, int);
void cleanenomousgrass(struct World *);
struct World *creatlandmind(struct World *);
void rankingsave();

int main() {
	//以下为隐藏光标
	HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cinfo;
	cinfo.bVisible = 0;
	cinfo.dwSize = 1;
	SetConsoleCursorInfo(fd, &cinfo);
	//以上为隐藏光标
	gamestart();
}

void gamestart() {
	int choose2, choose3, choose4;
	do {
		if (printinfo_and_getchoose(1, 5, 20) == 2) {
			exit(0);
		}
		choose2 = printinfo_and_getchoose(2, 5, 20);
		if (choose2 == 1) {
			gamerunstagemode();
		}
		else if (choose2 == 2) {
			choose3 = printinfo_and_getchoose(3, 5, 20);
			choose4 = printinfo_and_getchoose(4, 5, 20);
			gameruncustommode(choose3, choose4);
		}
	} while (1);
}

int printinfo_and_getchoose(int state, int x0, int y0) {
	if (state >= 1 && state <= 4) {
		system("mode con cols=100 lines=38");
	}
	int x = x0 + 3;//菜单框的起始x坐标
	int y = y0 / 4;//菜单框的起始y坐标
	//以下是操作说明
	if (state != 5 && state != 6 && state != 7 && state != 8 && state != 9 && state != 10) {
		int infox = x + 16;
		printrectangle(infox, y - 2, 21, 34, 1);
		printstrwithframe(infox + 8, y + 1, "操作说明", 1);
		printstrwithframe(infox + 1, y + 4, "WASD移动 回车选择 空格暂停 ESC退出", 1);
		printstrwithframe(infox + 5, y + 7, "  ○○□    这是蛇皮", 1);
		printstrwithframe(infox + 5, y + 10, "    ●      这是食物", 1);
		printstrwithframe(infox + 5, y + 13, "    ■       这是墙 ", 1);
		printstrwithframe(infox + 5, y + 16, "    OO      这是地雷", 1);
		printstrwithframe(infox + 5, y + 19, "    WW      这是毒草", 1);
		printstrwithframe(infox + 5, y + 23, " 细线边框为游戏区域 ", 1);
		printstrwithframe(infox + 4, y + 26, "蛇皮可以通过它穿梭地图！", 1);
	}
	//以上是操作说明
	int cursor = 1;
	if (state == 1) {
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 2, y + 2, "欢迎来到蛇皮世界", 1);
		printstrwithframe(x + 4, y + 7, "开始游戏", 1);
		printstrwithframe(x + 4, y + 10, "退出游戏", 1);
		cursor = getcursorchoose(x + 4 + 7, y + 7 + 1, 2);
		if (cursor == 1) {
			clean(x + 1, y + 1, x + 12, y + 13);
			return 1;
		}
		else if (cursor == 2) {
			return 2;
		}
	}
	else if (state == 2) {
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 3, y + 2, " 请选择模式 ", 1);
		printstrwithframe(x + 4, y + 7, "闯关模式", 1);
		printstrwithframe(x + 4, y + 10, "休闲模式", 1);
		cursor = getcursorchoose(x + 4 + 7, y + 7 + 1, 2);
		if (cursor == 1) {
			clean(x + 1, y + 1, x + 12, y + 13);
			return 1;
		}
		else if (cursor == 2) {
			return 2;
		}
	}
	else if (state == 3) {
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 4, y + 1, "休闲模式", 1);
		gotoxy((x + 3) * 2, y + 4);
		printf(" 请选择地图大小");
		printstrwithframe(x + 5, y + 5, " 小 ", 1);
		printstrwithframe(x + 5, y + 8, " 中 ", 1);
		printstrwithframe(x + 5, y + 11, " 大 ", 1);
		int choose = getcursorchoose(x + 5 + 5, y + 5 + 1, 3);
		clean(x + 1, y + 1, x + 12, y + 13);
		switch (choose) {
		case 1:return 1;
		case 2:return 2;
		case 3:return 3;
		}
	}
	else if (state == 4) {
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 4, y + 1, "休闲模式", 1);
		gotoxy((x + 4) * 2, y + 4);
		printf(" 请选择速度");
		printstrwithframe(x + 4, y + 5, "闲庭信步", 1);
		printstrwithframe(x + 4, y + 8, "快马加鞭", 1);
		printstrwithframe(x + 4, y + 11, "云霄飞车", 1);
		int choose = getcursorchoose(x + 4 + 7, y + 5 + 1, 3);
		clean(x + 1, y + 1, x + 12, y + 13);
		switch (choose) {
		case 1:return 1;
		case 2:return 2;
		case 3:return 3;
		}
	}
	else if (state == 5) {
		clean(x + 1, y + 1, x + 12, y + 13);
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 4, y + 2, "游戏结束", 1);
		printstrwithframe(x + 4, y + 6, "重新开始", 1);
		printstrwithframe(x + 3, y + 9, " 返回主菜单 ", 1);
		int choose = getcursorchoose(x + 4 + 8, y + 6 + 1, 2);
		clean(x + 1, y + 1, x + 12, y + 13);
		return choose;
	}
	else if (state == 6) {
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 4, y + 2, "闯关模式", 1);
		printstrwithframe(x + 4, y + 5, " 第一关 ", 1);
		gotoxy((x + 5) * 2, y + 9);
		printf("得分：%-3d", score);
		gotoxy((x + 4) * 2, y + 11);
		printf("完成率：%%%-5.1f", (((float)snakelength-2) / snakelength0) * 100);
		return 0;
	}
	else if (state == 7) {
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 4, y + 2, "闯关模式", 1);
		printstrwithframe(x + 4, y + 5, " 第二关 ", 1);
		gotoxy((x + 5) * 2, y + 9);
		printf("得分：%-3d", score);
		gotoxy((x + 4) * 2, y + 11);
		printf("完成率：%%%-5.1f", (((float)snakelength-2) / snakelength0) * 100);
		return 0;
	}
	else if (state == 8) {
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 4, y + 2, "闯关模式", 1);
		printstrwithframe(x + 4, y + 5, " 第三关 ", 1);
		gotoxy((x + 5) * 2, y + 9);
		printf("得分：%-3d", score);
		gotoxy((x + 4) * 2, y + 11);
		printf("完成率：%%%-5.1f", (((float)snakelength-2) / snakelength0) * 100);
		return 0;
	}
	else if (state == 9) {
		clean(x + 1, y + 1, x + 12, y + 13);
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 4, y + 2, "恭喜通关", 1);
		gotoxy((x + 4) * 2, y + 6);
		printf("你的得分为：%d", score);
		printstrwithframe(x + 4, y + 7, "重新开始", 1);
		printstrwithframe(x + 3, y + 10, " 返回主菜单 ", 1);
		gotoxy(0, row + 4);
		printf("请输入你的名字：");
		rankingsave();
		gotoxy(0, row + 5);
		printf("分数保存成功！");
		int choose = getcursorchoose(x + 4 + 8, y + 7 + 1, 2);
		clean(x + 1, y + 1, x + 12, y + 13);
		return choose;
	}
	else if (state == 10) {
		printrectangle(x, y, 14, 15, 1);
		printstrwithframe(x + 4, y + 2, "休闲模式", 1);
		gotoxy((x + 5) * 2, y + 9);
		printf(" 得分：%-3d", score);
		gotoxy((x + 5) * 2, y + 11);
		printf("蛇皮长：%-4d", snakelength);
		return 0;
	}
}

void gamerunstagemode() {
	char direction;//初始方向
	char input;
	_Bool state;//1为正常，0为死亡
	struct World *pfood=NULL;
	struct World *plandmind=NULL;
	struct World *head = NULL;
	int stage = 1;
	int speed = 100;
	_Bool stageup;//是否过关
	_Bool gameover = 0;//是否结束游戏
	int grasscount = 0;
	int grasstime = 0;
	int grasstime0 = 0;
	score = 0;
	_Bool firsteat = 0;
	_Bool firstcreatlandmind = 0;
	do {
		grasstime0 = 0;
		if (stage == 1) {
			column = 12;
			row = 12;
			head = creatworld();
			direction = creatsnake(head, 1);
			char stCmd[64];
			sprintf(stCmd, "mode con cols=%d lines=%d", (3 + column + 15) * 2, row + 7);
			system(stCmd);
			printrectangle(0, 0, column + 2, row + 2, 1);//绘制地图边框
			speed = 120;
			snakelength0 = 45;
			grasscount = 3;
			grasstime = 60;
		}
		else if (stage == 2) {
			column = 20;
			row = 20;
			head = creatworld();
			creatwall(head, 2);
			direction = creatsnake(head, 2);
			char stCmd[64];
			sprintf(stCmd, "mode con cols=%d lines=%d", (3 + column + 15) * 2, row + 7);
			system(stCmd);
			printrectangle(0, 0, column + 2, row + 2, 1);//绘制地图边框
			speed = 90;
			snakelength0 = 40;
			grasscount = 6;
			grasstime = 54;
		}
		else if (stage == 3) {
			column = 30;
			row = 30;
			head = creatworld();
			creatwall(head, 3);
			direction = creatsnake(head, 3);
			char stCmd[64];
			sprintf(stCmd, "mode con cols=%d lines=%d", (3 + column + 15) * 2, row + 7);
			system(stCmd);
			printrectangle(0, 0, column + 2, row + 2, 1);//绘制地图边框
			speed = 70;
			snakelength0 = 40;
			grasscount = 8;
			grasstime = 48;
		}//关卡信息设置
			//运行主体部分
		firsteat = 0;
		firstcreatlandmind = 0;
		plandmind = NULL;
		state = 1;
		input = 0;
		stageup = 0;
		pfood = creatfood(head);
		while (state == 1 && input != 27) {
			printinfo_and_getchoose(stage + 5, column, row);//打印信息
			if (_kbhit()) {
				input = (char)_getwch();
				if (input == 32) {
					gotoxy(0, row + 5);
					system("pause");
					gotoxy(0, row + 5);
					printf("                   ");
					input = 0;
				}
				switch (input) {
				case 'w':direction = 'W'; break;
				case 'a':direction = 'A'; break;
				case 's':direction = 'S'; break;
				case 'd':direction = 'D'; break;
				case '=':snakelength0 = snakelength-2;
				}
			}
			state = snakemove(direction);//移动蛇并返回状态是否正常
			if (pfood->display != 3) {
				firsteat = 1;
			}
			if (firsteat == 1) {
				grasstime0 += 1;
				if (grasstime0 == grasstime / 4) {
					creatvenomousgrass(head, grasscount);
				}
				if (grasstime0 == grasstime) {
					cleanenomousgrass(head);
					grasstime0 = 0;
				}
				if (firstcreatlandmind == 0) {
					plandmind = creatlandmind(head);
					firstcreatlandmind = 1;
				}
			}
			if (pfood->display != 3) {
				score += judgescore(pfood);
				pfood = creatfood(head);
			}
			if (plandmind != NULL) {
				if (plandmind->display != 6) {
					plandmind = creatlandmind(head);
				}
			}
			printworld(head);//打印世界，更新蛇长
			if (snakelength-2 >= snakelength0) {
				switch (stage) {
				case 1:printinfo_and_getchoose(stage + 5, column, row); stage = 2; gotoxy(0, row + 3); printf("恭喜过关！请按任意键进入下一关");gotoxy(0, row + 5);system("pause");  break;
				case 2:printinfo_and_getchoose(stage + 5, column, row); stage = 3; gotoxy(0, row + 3); printf("恭喜过关！请按任意键进入下一关");gotoxy(0, row + 5);system("pause");  break;
				case 3:printinfo_and_getchoose(stage + 5, column, row); stage = 0; gameover = 1;
				}
				state = 0;
				stageup = 1;
			}
			Sleep(speed);
		}
		if (stageup == 0) {
			score = 0;
			if (printinfo_and_getchoose(5, column, row) == 2) {
				gameover = 1;
			}
		}//死亡，清空分数，根据选择重新开始或者返回主菜单
		initialize(head, 1);
		if (stage == 0) {
			if (printinfo_and_getchoose(9, column, row) == 1) {
				stage = 1;
				gameover = 0;
				score = 0;
			}
		}
	} while (stage != 0 && gameover == 0);
}

void gameruncustommode(int size, int difficulty) {
	int speed;
	switch (difficulty) {
	case 1:speed = 100; break;
	case 2:speed = 60; break;
	case 3:speed = 30;
	}
	switch (size) {
	case 1:column = 10; row = 10; break;
	case 2:column = 20; row = 20; break;
	case 3:column = 40; row = 30;
	}
	if (size == 3) {
		speed = 0;
	}
	char direction;//初始方向
	char input;
	_Bool state;//1为正常，0为死亡
	struct World *pfood = NULL;
	struct World *head = NULL;
	score = 0;
	char stCmd[64];
	sprintf(stCmd, "mode con cols=%d lines=%d", (3 + column + 15) * 2, row + 7);
	system(stCmd);
	printrectangle(0, 0, column + 2, row + 2, 1);//绘制地图边框
			//运行主体部分
	do {
		state = 1;
		input = 0;
		score = 0;
		head = creatworld();
		direction = creatsnake(head, 1);
		pfood = creatfood(head);
		while (state == 1 && input != 27) {
			printinfo_and_getchoose(10, column, row);//打印信息
			if (_kbhit()) {
				input = (char)_getwch();
				if (input == 32) {
					gotoxy(0, row + 5);
					system("pause");
					gotoxy(0, row + 5);
					printf("                   ");
					input = 0;
				}
				switch (input) {
				case 'w':direction = 'W'; break;
				case 'a':direction = 'A'; break;
				case 's':direction = 'S'; break;
				case 'd':direction = 'D'; break;
				}
			}
			state = snakemove(direction);//移动蛇并返回状态是否正常
			while (pfood->display != 3) {
				score += judgescore(pfood);
				pfood = creatfood(head);
			}//食物被吃掉后生成新的食物
			printworld(head);//打印世界，更新蛇长
			Sleep(speed);
		}
		initialize(head, 1);
	} while (printinfo_and_getchoose(5, column, row) == 1);
}

int judgescore(struct World *pfood) {
	int foodscore = 0;
	if (pfood->left->up->display == 1 || pfood->left->up->display == 4)foodscore++;
	if (pfood->left->down->display == 1 || pfood->left->down->display == 4)foodscore++;
	if (pfood->right->up->display == 1 || pfood->right->up->display == 4)foodscore++;
	if (pfood->right->down->display == 1 || pfood->right->down->display == 4)foodscore++;
	if (pfood->left->display == 1 || pfood->left->display == 4)foodscore++;
	if (pfood->right->display == 1 || pfood->right->display == 4)foodscore++;
	if (pfood->up->display == 1 || pfood->up->display == 4)foodscore++;
	if (pfood->down->display == 1 || pfood->down->display == 4)foodscore++;
	return foodscore;
}

struct World * creatworld() {
	struct World *head = (struct World *)malloc(sizeof(struct World));
	struct World *p1, *p2, *p3, *p4;
	p1 = p2 = p3 = p4 = head;
	p1->locx = 0;
	p1->locy = 0;
	p1->direction = 0;
	p1->display = 0;
	int x, y;
	for (y = 0; y < row - 1; y++) {
		for (x = 1; x < column; x++) {
			p1 = (struct World *)malloc(sizeof(struct World));
			if (x == 0 && y == 0) { head = p1; };
			p1->locx = x;
			p1->locy = y;
			p1->direction = 0;
			p1->display = 0;
			p1->left = p2;
			p2->right = p1;
			p2 = p1;
			if (y != 0) {
				p3 = p3->right;
				p1->up = p3;
				p3->down = p1;
			}
		}
		p2->right = p4;
		p4->left = p2;
		p3 = p4;
		p1 = (struct World *)malloc(sizeof(struct World));
		p1->locx = 0;
		p1->locy = y + 1;
		p1->direction = 0;
		p1->display = 0;
		p1->up = p3;
		p3->down = p1;
		p2 = p4 = p1;
	}
	for (x = 1; x < column; x++) {
		p1 = (struct World *)malloc(sizeof(struct World));
		p1->locx = x;
		p1->locy = row - 1;
		p1->direction = 0;
		p1->display = 0;
		p1->left = p2;
		p2->right = p1;
		p2 = p1;
		p3 = p3->right;
		p1->up = p3;
		p3->down = p1;
	}
	p2->right = p4;
	p4->left = p2;
	p1 = p4;
	p3 = head;
	for (x = 0; x < column; x++) {
		p1->down = p3;
		p3->up = p1;
		p1 = p1->right;
		p3 = p3->right;
	}
	return head;
}

char creatsnake(struct World *head, int mode) {
	struct World *p = head;
	if (mode == 2) {
		for (int i = 0; i < 5; i++) {
			p = p->down;
		}
	}
	for (int i = 0; i < 2; i++) {
		p->direction = 'D';
		p->display = 1;
		switch (i) {
		case 0:pw1 = pw2 = p; break;
		case 1:pt = p; p->display = 2; break;
		}
		p = p->right;
	}
	return 'D';
}

_Bool snakemove(char dir) {
	char tdir;
	int state;
	switch (dir) {
	case 'W':tdir = 'S'; break;
	case 'S':tdir = 'W'; break;
	case 'A':tdir = 'D'; break;
	case 'D':tdir = 'A'; break;
	}
	if (pt->direction != tdir)pt->direction = dir;
	if (pt->direction == 'W') {
		state = pt->up->display;
		if (pt->up == pw1) {
			state = 7;
			movepw(state);
		}
		pt->up->display = 2;
		pt->display = 1;
		pt = pt->up;
		pt->direction = 'W';
		if (state != 7)movepw(state);
	}
	else if (pt->direction == 'A') {
		state = pt->left->display;
		if (pt->left == pw1) {
			state = 7;
			movepw(state);
		}//头吃尾视为正常
		pt->left->display = 2;
		pt->display = 1;
		pt = pt->left;
		pt->direction = 'A';
		if (state != 7)movepw(state);
	}
	else if (pt->direction == 'S') {
		state = pt->down->display;
		if (pt->down == pw1) {
			state = 7;
			movepw(state);
		}
		pt->down->display = 2;
		pt->display = 1;
		pt = pt->down;
		pt->direction = 'S';
		if (state != 7)movepw(state);
	}
	else if (pt->direction == 'D') {
		state = pt->right->display;
		if (pt->right == pw1) {
			state = 7;
			movepw(state);
		}
		pt->right->display = 2;
		pt->display = 1;
		pt = pt->right;
		pt->direction = 'D';
		if (state != 7)movepw(state);
	}
	if (pt == pw1) {
		return 0;
	}
	switch (state) {
	case 0:return 1;//正常返回1，异常返回0
	case 3:return 1;
	case 5:score -= 10;return 1;
	case 6:return 1;
	case 7:return 1;
	default:return 0;
	}
}

void movepw(int state) {
	if (state == 0 || state == 4) {
		if (pw1->direction == 'W') {
			pw2 = pw1->up;
			pw1->display = 0;
			pw1->direction = 0;
			pw1 = pw2;
		}
		else if (pw1->direction == 'A') {
			pw2 = pw1->left;
			pw1->display = 0;
			pw1->direction = 0;
			pw1 = pw2;
		}
		else if (pw1->direction == 'S') {
			pw2 = pw1->down;
			pw1->display = 0;
			pw1->direction = 0;
			pw1 = pw2;
		}
		else if (pw1->direction == 'D') {
			pw2 = pw1->right;
			pw1->display = 0;
			pw1->direction = 0;
			pw1 = pw2;
		}
	}
	else if (state == 5) {
		for (int i = 0; i < 2; i++) {
			if (pw1->direction == 'W') {
				pw2 = pw1->up;
				pw1->display = 0;
				pw1->direction = 0;
				pw1 = pw2;
			}
			else if (pw1->direction == 'A') {
				pw2 = pw1->left;
				pw1->display = 0;
				pw1->direction = 0;
				pw1 = pw2;
			}
			else if (pw1->direction == 'S') {
				pw2 = pw1->down;
				pw1->display = 0;
				pw1->direction = 0;
				pw1 = pw2;
			}
			else if (pw1->direction == 'D') {
				pw2 = pw1->right;
				pw1->display = 0;
				pw1->direction = 0;
				pw1 = pw2;
			}
		}
	}
	else if (state == 6) {
		for (int i = 0; i <snakelength/2+1; i++) {
			if (pw1->direction == 'W') {
				pw2 = pw1->up;
				pw1->display = 0;
				pw1->direction = 0;
				pw1 = pw2;
			}
			else if (pw1->direction == 'A') {
				pw2 = pw1->left;
				pw1->display = 0;
				pw1->direction = 0;
				pw1 = pw2;
			}
			else if (pw1->direction == 'S') {
				pw2 = pw1->down;
				pw1->display = 0;
				pw1->direction = 0;
				pw1 = pw2;
			}
			else if (pw1->direction == 'D') {
				pw2 = pw1->right;
				pw1->display = 0;
				pw1->direction = 0;
				pw1 = pw2;
			}
		}
		score -= 10 * (snakelength / 2);
	}
	else if (state == 7) {
		if (pw1->direction == 'W') {
			pw2 = pw1->up;
			pw1 = pw2;
		}
		else if (pw1->direction == 'A') {
			pw2 = pw1->left;
			pw1 = pw2;
		}
		else if (pw1->direction == 'S') {
			pw2 = pw1->down;
			pw1 = pw2;
		}
		else if (pw1->direction == 'D') {
			pw2 = pw1->right;
			pw1 = pw2;
		}
	}
}

struct World *creatfood(struct World *head) {
	srand(time(NULL));
	struct World *p = head;
	int i, j;
	int x, y;
	do {
		x = rand() % column;
		y = rand() % row;
		for (i = 0; i < x; i++) {
			p = p->right;
		}
		for (j = 0; j < y; j++) {
			p = p->down;
		}
	} while (p->display != 0);
	p->display = 3;
	return p;
}

void creatvenomousgrass(struct World *head, int count) {
	srand(time(NULL));
	struct World *p = NULL;
	int i, j, k;
	int x, y;
	for (k = 0; k < count; k++) {
		p = head;
		do {
			x = rand() % column;
			y = rand() % row;
			for (i = 0; i < x; i++) {
				p = p->right;
			}
			for (j = 0; j < y; j++) {
				p = p->down;
			}
		} while (p->display != 0);
		p->display = 5;
	}
}

struct World *creatlandmind(struct World *head) {
	srand(time(NULL));
	struct World *p = head;
	int i, j;
	int x, y;
	do {
		x = rand() % column;
		y = rand() % row;
		for (i = 0; i < x; i++) {
			p = p->right;
		}
		for (j = 0; j < y; j++) {
			p = p->down;
		}
	} while (p->display != 0);
	p->display = 6;
	return p;
}

void printworld(struct World *p) {
	int sl = 0;
	int x = 0, y = 0;
	while (x != column && y != row) {
		while (x != column) {
			if (p->display == 0) {
				gotoxy((p->locx + 1) * 2, p->locy + 1);
				printf("  ");
			}
			else if (p->display != 0) {
				gotoxy((p->locx + 1) * 2, p->locy + 1);
				switch (p->display) {
				case 1:printf("○"); sl += 1; break;//判断蛇长
				case 2:printf("□"); sl += 1; break;
				case 3:printf("●"); break;
				case 4:printf("■"); break;
				case 5:printf("WW");break;
				case 6:printf("OO");
				}
			}
			p = p->right;
			x += 1;
		}
		x = 0;
		p = p->down;
		y += 1;
	}
	snakelength = sl;
}

void creatwall(struct World *p, int stage) {
	int x = 0, y = 0;
	while (x != column && y != row) {
		while (x != column) {
			if (stage == 2) {
				switch (x) {
				case 0:if (y >= 0 && y <= 4 || y >= 15 && y <= 19)p->display = 4; break;
				case 1:if (y == 0 || y == 19)p->display = 4; break;
				case 2:if (y == 0 || y == 19)p->display = 4; break;
				case 3:if (y == 0 || y == 4 || y == 15 || y == 19) p->display = 4; break;
				case 4:if (y == 0 || y == 3 || y == 4 || y == 15 || y == 16 || y == 19)p->display = 4; break;
				case 7:if (y == 7 || y == 8 || y == 11 || y == 12)p->display = 4; break;
				case 8:if (y == 7 || y == 12)p->display = 4; break;
				case 11:if (y == 7 || y == 12)p->display = 4; break;
				case 12:if (y == 7 || y == 8 || y == 11 || y == 12)p->display = 4; break;
				case 15:if (y == 0 || y == 3 || y == 4 || y == 15 || y == 16 || y == 19)p->display = 4; break;
				case 16:if (y == 0 || y == 4 || y == 15 || y == 19) p->display = 4; break;
				case 17:if (y == 0 || y == 19)p->display = 4; break;
				case 18:if (y == 0 || y == 19)p->display = 4; break;
				case 19:if (y >= 0 && y <= 4 || y >= 15 && y <= 19)p->display = 4; break;
				}
			}
			else if (stage == 3) {
				switch (x) {
				case 2:if (y == 2 || y == 3 || y >= 7 && y <= 14 || y == 26 || y == 27)p->display = 4; break;
				case 3:if (y == 2 || y == 7 || y == 14 || y == 27)p->display = 4; break;
				case 4:if (y == 19)p->display = 4; break;
				case 5:if (y == 19)p->display = 4; break;
				case 6:if (y >= 19 && y <= 21)p->display = 4; break;
				case 7:if (y == 7 || y == 14)p->display = 4; break;
				case 8:if (y == 7 || y == 14)p->display = 4; break;
				case 9:if (y >= 5 && y <= 7 || y >= 14 && y <= 16 || y >= 25 && y <= 29)p->display = 4; break;
				case 10:if (y == 5 || y == 16 || y >= 23 && y <= 25)p->display = 4; break;
				case 11:if (y >= 3 && y <= 5 || y >= 16 && y <= 18 || y == 23)p->display = 4; break;
				case 12:if (y == 9 || y == 12 || y == 23)p->display = 4; break;
				case 13:if (y == 8 || y == 9 || y == 12 || y == 13 || y == 23)p->display = 4; break;
				case 14:if (y == 23)p->display = 4; break;
				case 15:if (y == 23)p->display = 4; break;
				case 16:if (y == 8 || y == 9 || y == 12 || y == 13 || y == 23)p->display = 4; break;
				case 17:if (y == 9 || y == 12 || y == 23)p->display = 4; break;
				case 18:if (y >= 3 && y <= 5 || y >= 16 && y <= 18 || y == 23)p->display = 4; break;
				case 19:if (y == 5 || y == 16 || y >= 23 && y <= 25)p->display = 4; break;
				case 20:if (y >= 5 && y <= 7 || y >= 14 && y <= 16 || y >= 25 && y <= 29)p->display = 4; break;
				case 21:if (y == 7 || y == 14)p->display = 4; break;
				case 22:if (y == 7 || y == 14)p->display = 4; break;
				case 23:if (y >= 19 && y <= 21)p->display = 4; break;
				case 24:if (y == 19)p->display = 4; break;
				case 25:if (y == 19)p->display = 4; break;
				case 26:if (y == 2 || y == 7 || y == 14 || y == 27)p->display = 4; break;
				case 27:if (y == 2 || y == 3 || y >= 7 && y <= 14 || y == 26 || y == 27)p->display = 4; break;
				case 28:;
				case 29:;
				}
			}
			p = p->right;
			x += 1;
		}
		x = 0;
		p = p->down;
		y += 1;
	}
}

void printrectangle(int x0, int y0, int column, int row, _Bool add) {
	int i;
	for (i = 0; i < column; i++) {
		gotoxy((i + x0) * 2, y0);
		if (i == 0) {
			switch (add) {
			case 1:printf("┌"); break;
			case 0:printf("  "); break;
			}
		}
		else if (i == column - 1) {
			switch (add) {
			case 1:printf("┐"); break;
			case 0:printf("  "); break;
			}
		}
		else {
			switch (add) {
			case 1:printf("─"); break;
			case 0:printf("  "); break;
			}
		}
		gotoxy((i + x0) * 2, y0 + row - 1);
		if (i == 0) {
			switch (add) {
			case 1:printf("└"); break;
			case 0:printf("  "); break;
			}
		}
		else if (i == column - 1) {
			switch (add) {
			case 1:printf("┘"); break;
			case 0:printf("  "); break;
			}
		}
		else {
			switch (add) {
			case 1:printf("─"); break;
			case 0:printf("  "); break;
			}
		}
	}
	for (i = 0; i < row - 2; i++) {
		gotoxy(x0 * 2, i + y0 + 1);
		switch (add) {
		case 1:printf("│"); break;
		case 0:printf("  "); break;
		}
		gotoxy((x0 + column - 1) * 2, i + y0 + 1);
		switch (add) {
		case 1:printf("│"); break;
		case 0:printf("  "); break;
		}
	}
}

void printstrwithframe(int x0, int y0, char str[], _Bool add) {
	int i;
	int strlength = strlen(str) / 2;
	printrectangle(x0, y0, strlength + 2, 3, add);
	if (add == 0) {
		for (i = 0; i < strlength; i++) {
			gotoxy((x0 + i + 1) * 2, y0 + 1);
			printf("  ");
		}
	}
	else {
		gotoxy((x0 + 1) * 2, y0 + 1);
		printf("%s", str);
	}
}

void printcursor(int x0, int y0, _Bool add) {
	gotoxy(x0 * 2, y0);
	switch (add) {
	case 1:printf("●"); break;
	case 0:printf("  ");
	}
}

int getcursorchoose(int x0, int y0, int count) {
	int ip = 0;
	int i = 0;
	int state = 1;
	do {
		switch ((char)ip) {
		case 'w':state -= 1; break;
		case 's':state += 1;
		}
		if (state == 0) {
			state = count;
		}
		else if (state == count + 1) {
			state = 1;
		}
		for (i = 0; i < count; i++) {
			printcursor(x0, y0 + 3 * i, 0);
			if (i + 1 == state) {
				printcursor(x0, y0 + 3 * i, 1);
			}
		}
		ip = _getwch();
	} while (ip != 13);
	for (i = 0; i < count; i++) {
		printcursor(x0, y0 + 3 * i, 0);
	}
	return state;
}

void initialize(struct World *head, _Bool freeornot) {
	struct World *p1 = head;
	struct World *p2 = head;
	int i, j;
	if (freeornot == 0) {
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				p1->direction = 0;
				p1->display = 0;
				p1 = p1->right;
			}
			p1 = p1->down;
		}
		printworld(head);
	}
	else if (freeornot == 1) {
		p1 = p2 = head;
		for (i = 0; i < row; i++) {
			for (j = 0; j < column - 1; j++) {
				p1 = p1->right;
				free(p2);
				p2 = p1;
			}
			if (i != row - 1) {
				p1 = p1->down;
				free(p2);
				p2 = p1 = p1->right;
			}
		}
	}
}

void clean(int x1, int y1, int x2, int y2) {
	int i, j;
	for (i = 0; i < y2 - y1 + 1; i++) {
		for (j = 0; j < x2 - x1 + 1; j++) {
			gotoxy((j + x1) * 2, i + y1);
			printf("  ");
		}
	}
}

void cleanenomousgrass(struct World *head) {
	struct World *p = head;
	int x = 0, y = 0;
	while (x != column && y != row) {
		while (x != column) {
			if(p->display==5){
				p->display = 0;
		}
			p = p->right;
			x += 1;
		}
		x = 0;
		p = p->down;
		y += 1;
	}
}

void rankingsave() {
	char name[100];
	FILE *ranking;
	scanf("%s", name);
	ranking = fopen("ranking.csv", "a");
	if (ranking == NULL)
	{
		printf("文件打开失败！");
		exit(0);
	}
	fprintf(ranking,"%s,%d\n", name, score);
	fclose(ranking);
}

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

//■●○□
//  ┌─┐
//  │■│
//  └─┘