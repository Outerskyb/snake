
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#define HEIGHT 20 //세로
#define WIDTH 40 //가로
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

typedef struct {
	int x;
	int y;
}ordered_pair;

int map[HEIGHT][WIDTH]; 
int point = 0;
int length;
int move = 0;
int life = 0;
int head = RIGHT;

ordered_pair record[4000];

void remove_cursor(void) {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y)//gotoxy
{
	COORD pos = { x - 1, y - 1 };//커서가 X좌표에서 -1 한값. Y좌표에서 -1한 값으로 이동
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void init_map(void) {
	memset(map, 0, sizeof(map));
}

void show_map(void) { //맵표현 gotoxy
	int i, j;
	gotoxy(1, 1);
	printf("\n");
	 

	printf("┌");
	for (j = 0; j < WIDTH; j++) { //메인창 바닥
		printf("─");
	}
	printf("┐");
	printf("\n");
	 
	for (i = 0; i <= HEIGHT + 1; i++) { //메인창의 높이
		if (i != HEIGHT + 1) {
			printf("┃"); //메인창 테두리
			for (j = 0; j < WIDTH; j++) { //메인창 너비
				if (map[i][j] == 0) {
					printf("　");
				}
				else if (map[i][j] == 1) {
					printf("■");//ㅁ한자에 있음
				}
				else {
					printf("★");
				}
			}
			printf("┃");// 테두리
			if (i == 4) {
				printf("score:%d", length - 2);
			}
			printf("\n");
		}
		else if(i == HEIGHT +1) {
			printf("└");
			for (j = 0; j < WIDTH; j++) { //메인창 바닥
				printf("─");
			}
			printf("┘");
			printf("\n");
		}
		
	}
}

void make_snake(void) {
	map[0][0] = 1;
	map[0][1] = 1;
	length = 2;
	record[move].x = 0;
	record[move].y = 0;
	move++;
	record[move].x = 1;
	record[move].y = 0;
}

void delete_tail(void) {
	map[record[move - length].y][record[move - length].x] = 0;
}

void make_point(void) {
	int x, y;

	x = rand() % 39;
	y = rand() % 19;
	if (map[y][x] == 1) {
		make_point();
	}
	else {
		map[y][x] = 2;
	}
}

int n_move_snake(int way) {
	int ver=0, ho=0;
	int wall_check_x = 100;
	int wall_check_y = 100;
	int check=0;
	if (way == LEFT) {
		ver = 0;
		ho = -1;
		wall_check_x = 0;
		wall_check_y = 100;
		
	}
	if (way == RIGHT) {
		ver = 0;
		ho = 1;
		wall_check_x = WIDTH;
		wall_check_y = 100;
	}
	if (way == UP) {
		ver = -1;
		ho = 0;
		wall_check_x = 100;
		wall_check_y = 0;
	}

	if (way == DOWN) {
		ver = 1;
		ho = 0;
		wall_check_x = 100;
		wall_check_y = HEIGHT;
	}
	if (record[move].x == wall_check_x || record[move].y == wall_check_y
		|| map[(record[move].y)+ver][(record[move].x) + ho] == 1) {//앞에 자기몸
		move++;
		life = 1;
		return 0;
	}
	else if (map[(record[move].y)+ver][(record[move].x) + ho] == 2) {//앞에 별있을때
		map[(record[move].y)+ver][(record[move].x) + ho] = 1;
		move++;
		record[move].y = record[move - 1].y + ver;
		record[move].x = record[move - 1].x + ho;
		make_point();
		check++;
		length++;
	}
	else {//앞에 빈칸
		map[(record[move].y)+ver][(record[move].x) + ho] = 1;
		move++;
		record[move].y = record[move - 1].y + ver;
		record[move].x = record[move - 1].x + ho;
		delete_tail();
		check++;
	}
	if (check != 0) {
		head = way;
	}
}

void game_over(void) {
	init_map();
	show_map();
	gotoxy((WIDTH + 7) / 2, (HEIGHT + 1) / 2);
	printf("Game Over");
}

int main(void) {
	remove_cursor();
	int tick=0;
	srand(time(NULL));
	init_map();
	make_snake();
	make_point();
	show_map();
	while (1) {
		if (life == 1) {
			break;
		}
		// 키보드 입력시 동작//
		while (_kbhit()) { 
			int key = getch();
			if ((head == RIGHT && key == LEFT) || (head == LEFT && key == RIGHT) || (head == UP && key == DOWN) || (head == DOWN && key == UP)) {
				life = 1;
				break;
			}
			else if(key != 224) {
				n_move_snake(key);
				show_map();
			}
			fflush(stdin);
		}
		////////////////////

		/////시간의 흐름/////
		if (tick == 22) {
			n_move_snake(head);
			show_map();
			tick = 0;
		}
		////////////////////
		tick++;
		_sleep(1);
	}
	game_over();
	return 0;
}
