#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
#define y_length 6
#define x_length 16
#define Map_x 47
#define Map_y 25
#define Snake_x 47
#define Snake_y 25

int map[Map_y][Map_x];
int key;	//키보드 입력을 받을 때 사용하는 변수
int cnt = 3;	//뱀 몸통 길이
int global_life = 4;	 //생명이 0 됐을 때 프로그램을 끝내기 위한 변수
int score = 0;	//점수

typedef struct _location
{
	int x;
	int y;
}location;
location food;	//음식이 출력될 위치가 저장되는 변수
location snake_head;	//뱀의 머리 위치를 저장
location snake_tail;	//뱀의 꼬리의 위치를 저장
location body[25];	//뱀의 각 몸통 부분의 위치를 저장하는 변수
location temp[25];	//body[][]변수를 업데이트 할 때 사용하는 임시 저장 변수

typedef struct _direction
{
	int up;
	int down;
	int left;
	int right;
}direction;
direction snake_d[Snake_y][Snake_x];	//뱀의 각 몸통이 이동할 위치를 저장하는 변수
direction d;	//현재 이동 위치를 저장하는 변수

void Game_instructions();
void Console();

void Map_init();	//맵 초기화 하는 함수
void Map();		//맵 출력하는 함수
void Score();	//점수 출력하는 함수
void Life();	//생명 출력하는 함수
void Food();	//음식 출력하는 함수
void Foodeat();	//음식을 먹으면 점수가 올라가고 음식이 다시 생성됨
void Die();	//벽이나 자기 자신한테 닿았을  생명 깍이고 다시 가운데로 돌아가는 함수

void Up();	//현재 이동 위치를 up으로 설정
void Down();	//현재 이동 위치를 down으로 설정
void Left();	//현재 이동 위치를 left로 설정
void Right();	//현재 이동 위치를 right로 설정

void Snake_init();	//뱀 초기화 하는 함수
void Temp_init();	//뱀 몸통 각 부분의 위치를 저장하는 변수 업데이트
void Create(); //뱀 머리 생성하는 함수
void Delete();	//뱀 꼬리 삭제하는 함수
void Snake_plus();	//점수 먹을 때 몸통 늘어나게 하는 함수
void Move();	//뱀 움직이게 하는 함수

void gotoxy(int x, int y);	//원하는 위치에 출력
void Event();	//이스터에그, 히든 엔딩, 뱀의 머리와 꼬리가 만나면 실행
int main();	//메인 함수

void Game_instructions()
{
	system("color 07");
	printf("\n\n\n\n");
	printf("\t\t\t\tGame instruction\n");
	printf("\n\n\n");
	printf("-> 방향키를 눌러서 뱀을 움직임니다.\n\n");
	printf("-> 뱀이 움직일 수 있는 창 안에서 음식이 랜덤으로 생성될 것입니다. 뱀이 음식을 먹을 마다 뱀의 길이가 한칸 늘어나고, 점수가 10점 오름니다.\n\n");
	printf("-> 생명은 총 3개 입니다. 뱀이 벽에 닿거나 뱀의 몸통에 닿으면 생명이 하나씩 줄어듭니다.\n\n");
	printf("-> 아무키나 눌러서 게임을 도중에 일시정지 할 수 있습니다. 다시 게임을 진행하려면 방향키를 누르면 됩니다.\n\n");
	printf("-> 만약 당신이 게임을 도중에 나가고 싶다면 esc를 누르세요\n\n");
	printf("Press the key to play game...");
	if (_getch()) {
	}
}

void Console()
{
	system("cls");
	system("color 72");
}

void Map_init()	//map[][]초기화 하는 함수
{
	int i;
	int j;
	for (i = 0; i < Map_y; i++) {
		for (j = 0; j < Map_x; j++) {
			if (i == 0 || i == 24 || j == 0 || j == 46) {
				map[i][j] = 1;
			}
			else {
				map[i][j] = 0;
			}
		}
	}
}

void Map()	//맵 출력하는 함수
{
	int i;
	int j;
	for (i = 0; i < Map_y; i++) {
		for (j = 0; j < Map_x; j++) {
			if (map[i][j] == 1) {
				gotoxy(x_length - 2 + (j * 2), y_length - 1 + i);
				printf("■");
			}
			else {
				gotoxy(x_length - 2 + (j * 2), y_length - 1 + i);
				printf("  ");
			}
		}
		printf("\n");
	}

	gotoxy(x_length + 24, 3);
	printf("Score: %d", score);
}

void Score()	//점수를 위쪽에 출력
{
	score += 10;
	gotoxy(x_length + 24, 3);
	cnt += 1;
	printf("Score: %d", score);
}

void Life()		//생명을 위쪽에 출력
{
	static int life = 4;
	life -= 1;
	global_life -= 1;
	gotoxy(x_length + 60, 3);
	printf("Life: %d", life);
}

void Food()
{
	srand(time(NULL));
	food.x = rand() % 45;
	food.y = rand() % 23;
	gotoxy(x_length + (food.x * 2), y_length + food.y);
	printf("★");
}

void Foodeat()
{
	if (food.x == snake_head.x && food.y == snake_head.y) {
		Score();
		Food();
	}
}

void Die()
{
	int i;
	int temp = 1;
	if (snake_head.x == 46 || snake_head.x < 0 || snake_head.y < 0 || snake_head.y == 23) {
		main();
	}
	for (i = 1; i < cnt - 1; i++) {
		if (snake_head.x == body[i].x && snake_head.y == body[i].y) {
			main();
		}
	}
	if (snake_head.x == body[cnt - 1].x && snake_head.y == body[cnt - 1].y) {
		Event();
	}
}

void Up()
{
	d.up = 1;
	d.down = 0;
	d.left = 0;
	d.right = 0;
}

void Down()
{
	d.up = 0;
	d.down = 1;
	d.left = 0;
	d.right = 0;
}

void Left()
{
	d.up = 0;
	d.down = 0;
	d.left = 1;
	d.right = 0;
}

void Right()
{
	d.up = 0;
	d.down = 0;
	d.left = 0;
	d.right = 1;
}

void Snake_init()
{
	int i;
	int j;
	int k;
	for (i = 0; i < Snake_y; i++) {
		for (j = 0; j < Snake_x; j++) {
			body[i].x = 0;
			body[i].y = 0;
			temp[i].x = 0;
			temp[i].y = 0;
			snake_d[i][j].up = 0;
			snake_d[i][j].down = 0;
			snake_d[i][j].left = 0;
			snake_d[i][j].right = 0;
		}
	}

	snake_head.x = 23;
	snake_head.y = 11;
	snake_tail.x = 24 - cnt;
	snake_tail.y = 11;

	for (k = 0; k < cnt; k++) {
		gotoxy(x_length + (snake_head.x - k) * 2, y_length + snake_head.y);
		printf("■");
		body[k].x = snake_head.x - k;
		body[k].y = 11;
		if (k == cnt - 1) {
			break;
		}
		snake_d[11][22 - k].right = 1;
	}
}

void Temp_init()
{
	int i;
	for (i = 0; i < cnt; i++) {
		temp[i] = body[i];
	}
}

void Create()	//맨 앞으로 뱀 머리 생성하는 함수
{
	int i;
	if (d.up == 1) {
		snake_d[snake_head.y][snake_head.x].up = 1;
		snake_head.y -= 1;
		gotoxy(x_length + (snake_head.x) * 2, y_length + snake_head.y);
		printf("■");
	}
	else if (d.down == 1) {
		snake_d[snake_head.y][snake_head.x].down = 1;
		snake_head.y += 1;
		gotoxy(x_length + (snake_head.x) * 2, y_length + snake_head.y);
		printf("■");
	}
	else if (d.left == 1) {
		snake_d[snake_head.y][snake_head.x].left = 1;
		snake_head.x -= 1;
		gotoxy(x_length + (snake_head.x) * 2, y_length + snake_head.y);
		printf("■");
	}
	else if (d.right == 1) {
		snake_d[snake_head.y][snake_head.x].right = 1;
		snake_head.x += 1;
		gotoxy(x_length + (snake_head.x) * 2, y_length + snake_head.y);
		printf("■");
	}
	Temp_init();
	for (i = 0; i < cnt; i++) {
		if (i == cnt - 1) {
			break;
		}
		body[i + 1] = temp[i];
	}
	body[0].x = snake_head.x;
	body[0].y = snake_head.y;
}

void Delete()	//맨뒤에 있는 뱀 꼬리 삭제하는 함수
{
	if (snake_d[snake_tail.y][snake_tail.x].up == 1) {
		snake_d[snake_tail.y][snake_tail.x].up = 0;
		gotoxy(x_length + (snake_tail.x) * 2, y_length + snake_tail.y);
		printf("  ");
		snake_tail.y -= 1;
	}
	else if (snake_d[snake_tail.y][snake_tail.x].down == 1) {
		snake_d[snake_tail.y][snake_tail.x].down = 0;
		gotoxy(x_length + (snake_tail.x) * 2, y_length + snake_tail.y);
		printf("  ");
		snake_tail.y += 1;
	}
	else if (snake_d[snake_tail.y][snake_tail.x].left == 1) {
		snake_d[snake_tail.y][snake_tail.x].left = 0;
		gotoxy(x_length + (snake_tail.x) * 2, y_length + snake_tail.y);
		printf("  ");
		snake_tail.x -= 1;
	}
	else if (snake_d[snake_tail.y][snake_tail.x].right == 1) {
		snake_d[snake_tail.y][snake_tail.x].right = 0;
		gotoxy(x_length + (snake_tail.x) * 2, y_length + snake_tail.y);
		printf("  ");
		snake_tail.x += 1;
	}
}

void Snake_plus()
{
	if (food.x == snake_head.x && food.y == snake_head.y) {
		Foodeat();
		Create();
	}
}

void Move()
{
	while (1) {
		Create();
		Snake_plus();
		Delete();
		Sleep(100);
		Die();
		if (_kbhit()) {
			break;
		}
	}
}

void gotoxy(int x, int y)	//원하는 위치에 출력
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void Event()
{
	int i;
	int j;
	Console();
	gotoxy(x_length + 24, 3);
	printf("Score: %d", score);
	gotoxy(x_length + 60, 3);
	printf("Life:   ");
	for (i = 0; i < Map_y; i++) {
		for (j = 0; j < Map_x; j++) {
			gotoxy(x_length - 2 + (j * 2), y_length - 1 + i);
			printf("■");
			Sleep(1);
		}
	}
	gotoxy(x_length - 2 + 37, y_length - 1 + 13);
	printf(" HIDDEN ENDING ");
	gotoxy(x_length - 2 + 37, y_length - 1 + 14);
	printf("Press the key...");
	if (_getch()) {}
	exit(0);
}

int main()
{
	if (global_life == 4) {
		Game_instructions();
	}
	Console();
	Map_init();
	Map();
	Snake_init();
	Life();
	Food();
	while (1) {
		if (_kbhit()) {
			key = _getch();
			if (key == 72) {
				Up();
				Move();
				key = 0;
			}
			else if (key == 80) {
				Down();
				Move();
				key = 0;
			}
			else if (key == 75) {
				Left();
				Move();
				key = 0;
			}
			else if (key == 77) {
				Right();
				Move();
				key = 0;
			}
			else if (key == 27) {
				Console();
				gotoxy(x_length + 35, y_length + 9);
				printf("GAME OVER");
				gotoxy(x_length + 35, y_length + 10);
				printf("Press the key...");
				if (_getch()) {
				}
				exit(0);
			}
			else {
				if (_getch) {
				}
			}
		}
		if (global_life == 0) {
			Console();
			Map();
			gotoxy(x_length + 35, y_length + 9);
			printf("GAME OVER");
			gotoxy(x_length + 35, y_length + 10);
			printf("Your Score: %d", (cnt - 3) * 10);
			gotoxy(x_length + 35, y_length + 11);
			printf("Press the key...");
			if (_getch()) {
			}
			exit(0);
		}
	}
}
