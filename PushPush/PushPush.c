#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

//색상 정리
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP 72  // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 80 
#define LEFT 75
#define RIGHT 77


int level = 0, walk = 0; // 현재스테이지,걸음수
int px, py; //플레이어 위치
#define MSIZE 13
#define MAXLEVEL 10 // 최대레벨
#define MAX_STACK_SIZE 200
#define CLEAR system("cls");
			// 맵 0: 벽 , 1 : 빈공간 , 2: 박스 3: 옮기는곳 4:플레이어 
int map[MAXLEVEL][MSIZE][MSIZE] = {
	{
		{ 0,1,1,1,1,2,1,3 },
		{ 0,1,1,1,4,2,1,3 },
		{ 0,1,1,1,1,2,1,3 }
	},
	{
		{ 0,1,1,3,1,3,1 },
		{ 0,1,4,2,2,2,3 },
		{ 0,1,1,1,1,1,1 }
	},
	{
		{ 0,0,1,1,4,0 },
		{ 0,0,1,2,2,0,0,0,0 },
		{ 0,0,1,2,1,0,0,0,3 },
		{ 0,0,0,0,1,1,1,0,3 },
		{ 0,0,0,1,1,1,1,1,3 },
		{ 0,0,0,1,1,1,1,1,1 },
		{ 0,0,0,0,1,1,1,0,1 }
	},
	{
		{ 0,0,3,0 },
		{ 0,0,2,1,2,3 },
		{ 0,3,2,4 },
		{ 0,0,0,2 },
		{ 0,0,0,3 }
	},
	{
		{ 0,0,1,1,1,1,1,0,0,0 },
		{ 0,0,2,0,0,0,1,1,1 },
		{ 0,1,4,1,2,1,1,2,1 },
		{ 0,1,3,3,0,1,2,1 },
		{ 0,0,3,3,0,1,1,1 }
	},
	{
		{ 0,0,1,1 },
		{ 0,0,4,2,1,1 },
		{ 0,0,0,1,0,1 },
		{ 0,3,0,1,0,1,1 },
		{ 0,3,2,1,1,0,1 },
		{ 0,3,1,1,1,2,1 }
	},
	{
		{ 0,0,0,0,1,1,1,1,1 },
		{ 0,1,1,1,3,0,0,0,1 },
		{ 0,1,0,1,0,1,1,1,2 },
		{ 0,1,0,1,2,1,1,0,3,1 },
		{ 0,1,0,1,2,3,1,1,0,1 },
		{ 0,1,3,0,2,1,2,1,0,1 },
		{ 0,0,1,1,4,1,0,1,0,1 },
		{ 0,0,1,0,0,0,3,1,1,1,1,1 },
		{ 0,0,1,1,1,1,1,0,0,1,1,1 }
	},
	{
		{ 0,0,0,0,1,1,0,1,1 },
		{ 0,0,0,1,1,1,0,2,1 },
		{ 0,0,0,1,2,4,1,1,1 },
		{ 0,0,0,1,2,0,0,1,1 },
		{ 0,0,0,2,2,1,0,1 },
		{ 0,3,3,3,3,3,1,1 }
	},
	{
		{ 0,0,1,1,0,0,1,1,1 },
		{ 0,0,1,1,1,2,1,1,1 },
		{ 0,0,2,1,0,0,0,1,2 },
		{ 0,0,1,0,3,3,3,0,1 },
		{ 0,0,1,0,3,3,3,0,1 },
		{ 0,1,2,1,1,2,1,1,2,1 },
		{ 0,1,1,1,1,1,0,1,4,1 }
	},
	{
		{ 0,0,0,1,1,1,1 },
		{ 0,0,0,2,2,2,1 },
		{ 0,1,1,2,3,3,1 },
		{ 0,4,2,3,3,3 },
		{ 0,0,0,0,1,1 }
	},
};
int playmaps[MSIZE][MSIZE];
typedef int element;
typedef struct keep {
	int top;
	element stack[MAX_STACK_SIZE];
}keep;
keep way;

//-----------------------------------------------
void gotoxy(int x, int y);
void textcolor(int color);
void UIimage();
void mapdraw();
void drawstat();//걸음이랑 스테이지 변화
void move(int ch);
void Reverse_move(int ch);
int checkEnd();
void start();
void push(keep* s, element data);// 행위 저장
element pop(keep* s);
void init(keep* s);
element peek(keep* s);// 이전 행위
void setlevel();
void dos();
void removeCursor(void);
void Rull();
//-----------------------------------------------


int main()
{
	dos();
	removeCursor();
	int ch, back;//입력과 돌리기
	start();
	gotoxy(17, 14);
	printf("Press any key");
	_getch();
	Rull();
	gotoxy(17, 14);
	printf("Press any key");
	_getch();
	setlevel();
	while (1)
	{
		CLEAR;
		UIimage();
		for (int i = 0; i < MSIZE; i++) { //플레이 할 맵 설정
			for (int j = 0; j < MSIZE; j++) {
				playmaps[i][j] = map[level][i][j];
			}
		}
		for (int i = 0; i < MSIZE; i++) { // 플레이어 위치 찾기
			for (int j = 0; j < MSIZE; j++) {
				if (playmaps[i][j] == 4)
				{
					px = j;
					py = i;
				}
			}
		}
		init(&way);
		while (2)
		{
			mapdraw();
			drawstat();
			ch = _getch();
			if (ch == 0 || ch == 0xE0) //입력키가 방향키일 경우
			{
				ch = _getch();

				switch (ch) {
				case LEFT:
				case RIGHT:
				case UP:
				case DOWN:
					move(ch);
					break;
				}
			}
			else
			{
				ch = tolower(ch);
				if (ch == 'r') {
					walk = 0;
					break;
				}
				if (ch == 'n') {
					if (level < MAXLEVEL - 1) {
						level++; walk = 0;
					}
					break;
				}
				if (ch == 'p') {
					if (level > 0) {
						level--; walk = 0;
					}
					break;
				}
				if (ch == 'q') {
					gotoxy(30, 17);
					exit(1);
				}
				if (ch == 'z') {
					back = peek(&way);
					Reverse_move(back);
					pop(&way);
				}
			}
			if (checkEnd() == 1) //정답일경우
			{
				CLEAR;
				if (level < MAXLEVEL - 1) {
					gotoxy(15, 4);
					textcolor(YELLOW1);
					printf("축하드립니다");
					gotoxy(14, 6);
					printf("%d 레벨을 풀었습니다", level + 1);
					gotoxy(14, 8);
					textcolor(GREEN1);
					printf("%d 레벨로 넘어갑니다", level + 2);
					gotoxy(15, 10);
					textcolor(WHITE);
					printf("아무키나 눌러주세요\n");
					_getch();
					level++;
				}
				else if (level == MAXLEVEL - 1) {
					gotoxy(15, 5);
					textcolor(YELLOW1);
					printf("축하드립니다");
					gotoxy(12, 7);
					textcolor(RED2);
					printf("마지막");
					textcolor(YELLOW1);
					printf("레벨을 풀었습니다");
					gotoxy(13, 9);
					textcolor(GREEN1);
					printf("프로그램이 종료됩니다", level + 2);
					gotoxy(12, 11);
					textcolor(WHITE);
					printf("아무키나 눌러주세요\n");
					_getch();
					return 0;
				}
				walk = 0;
				break;
			}
		}

	}
}
void setlevel() {
	CLEAR;
	gotoxy(10, 6);
	printf("레벨을 입력해주세요(1~10)");
	gotoxy(10, 8);
	printf("레벨 범위를 벗어난 입력은");
	gotoxy(10, 10);
	printf("1부터 시작입니다.");
	gotoxy(21, 12);
	scanf_s("%d", &level);
	level--;
}
void gotoxy(int x, int y) // gotoxy 좌표함수 
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void textcolor(int color) // 컬러 함수
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void start() {
	gotoxy(2, 4);
	printf(" ▣▣▣▣   ▣    ▣   ▣▣▣▣   ▣    ▣   ");
	gotoxy(2, 5);
	printf(" ▣    ▣   ▣    ▣   ▣         ▣    ▣   ");
	gotoxy(2, 6);
	printf(" ▣▣▣▣   ▣    ▣   ▣▣▣▣   ▣▣▣▣   ");
	gotoxy(2, 7);
	printf(" ▣         ▣    ▣         ▣   ▣    ▣   ");
	gotoxy(2, 8);
	printf(" ▣         ▣▣▣▣   ▣▣▣▣   ▣    ▣   ");
	gotoxy(23, 11);
	printf("made by Mingyu Kim");
}
void mapdraw() {	// 맵 0: 벽 , 1 : 빈공간 , 2: 박스 3: 옮기는곳 4:플레이어 
	int i, j, ch;//ch는 지도 판별
	i = j = 0;
	gotoxy(0, 0);
	for (i = 0; i < MSIZE; i++)
	{
		for (j = 0; j < MSIZE; j++)
		{
			ch = playmaps[i][j];
			switch (ch) {
			case 0: textcolor(GRAY1); printf("▩"); break;
			case 1: printf("  "); break;
			case 2: textcolor(GREEN1); printf("□"); break;
			case 3: textcolor(CYAN1); printf("○"); break;
			case 4: textcolor(YELLOW2); printf("P "); break;
			}
		}
		printf("\n");
	}
	textcolor(MAGENTA2);

}
void UIimage()
{
	int i;
	int xx = 26;
	textcolor(RED2);
	gotoxy(xx, 0);
	printf("********************\n");
	for (i = 0; i < 14; i++)
	{
		gotoxy(xx, 1 + i);
		printf("*                  *\n");
	}
	gotoxy(xx, 1 + i);
	printf("********************\n");

	xx = 29;
	gotoxy(xx, 1);
	textcolor(YELLOW2);
	printf("┌─────────────┐\n");
	for (i = 0; i < 2; i++)
	{
		gotoxy(xx, 2 + i);
		printf("│             │\n");
	}
	gotoxy(xx, 2 + i);
	printf("└─────────────┘\n");
	gotoxy(31, 2);
	textcolor(BLUE2);
	printf("Level : %d", level + 1);
	gotoxy(31, 3);
	printf("Walk  : %d", walk);
	textcolor(YELLOW2);
	gotoxy(29, 5);
	printf("↑→↓← : Move");
	gotoxy(29, 7);
	printf(" R : Restart");
	gotoxy(29, 9);
	printf(" N,P : NEXT,PRE");
	gotoxy(29, 11);
	printf(" Z : Back");
	gotoxy(29, 13);
	printf(" Q : QUIT");
}
void drawstat()
{
	textcolor(BLUE2);
	gotoxy(31, 2);
	printf("Level: %d/%d ", level + 1, MAXLEVEL);
	gotoxy(31, 3);
	printf("Walk : %d  ", walk);
}
void move(int ch) {
	int x = 0, y = 0;
	switch (ch) {
	case UP: y = -1; break;
	case DOWN: y = 1; break;
	case RIGHT: x = 1; break;
	case LEFT: x = -1; break;
	}

	if (playmaps[py + y][px + x] != 0) //벽이 아닐경우
	{
		push(&way, ch);// 맵 0: 벽 , 1 : 빈공간 , 2: 박스 3: 옮기는곳 4:플레이어 
		if (playmaps[py + y][px + x] == 2) //벽이 아니고 박스일경우
		{
			if (playmaps[py + y * 2][px + x * 2] == 2 || playmaps[py + y * 2][px + x * 2] == 0) //박스 바로 옆에 박스이거나 벽일경우
			{
				return; //멈춤
			}
			else if (map[level][py][px] == 3) {
				playmaps[py][px] = 3;
				playmaps[py + y][px + x] = 4;
				playmaps[py + y * 2][px + x * 2] = 2;
			}
			else		// 빈공간이거나 옮길곳일경우
			{
				playmaps[py][px] = 1;
				playmaps[py + y][px + x] = 4;
				playmaps[py + y * 2][px + x * 2] = 2;
			}
		}
		else if (map[level][py][px] == 3) // 플레이어의 현 위치가 옮기는 곳일경우
		{
			playmaps[py][px] = 3;
			playmaps[py + y][px + x] = 4;
		}
		else // 옆이 빈 공간일경우
		{
			playmaps[py][px] = 1;
			playmaps[py + y][px + x] = 4;
		}
	}
	else
		return;
	px += x;
	py += y;
	walk += 1;
}
void Reverse_move(int ch) { //거꾸로 움직이기.
	int x = 0, y = 0;
	switch (ch) {
	case UP: y = 1; break;
	case DOWN: y = -1; break;
	case RIGHT: x = -1; break;
	case LEFT: x = 1; break;
	}
	// 맵 0: 벽 , 1 : 빈공간 , 2: 박스 3: 옮기는곳 4:플레이어 
	if (playmaps[py + y][px + x] != 0) //전의 것이 벽이 아닐경우
	{
		// 맵 0: 벽 , 1 : 빈공간 , 2: 박스 3: 옮기는곳 4:플레이어 
		if (playmaps[py - y][px - x] == 2) //벽이 아니고 박스일경우
		{
			if (map[level][py - y][px - x] == 3)	 //옮길곳일경우
			{
				playmaps[py - y][px - x] = 3;
				playmaps[py][px] = 2;
				playmaps[py + y][px + x] = 4;
			}
			else // 빈공간일경우
			{
				playmaps[py - y][px - x] = 1;
				playmaps[py][px] = 2;
				playmaps[py + y][px + x] = 4;
			}
		}
		else if (playmaps[py + y][px + x] == 2)
		{
			playmaps[py][px] = 1;
			playmaps[py + y][px + x] = 4;
			playmaps[py + y * 2][px + x * 2] = 2;
		}
		else if (map[level][py][px] == 3) // 플레이어의 현 위치가 정답일경우
		{
			playmaps[py][px] = 3;
			playmaps[py + y][px + x] = 4;
		}
		else // 옆이 빈 공간일경우
		{
			playmaps[py][px] = 1;
			playmaps[py + y][px + x] = 4;
		}
	}
	else
		return;
	px += x;
	py += y;
	walk -= 1;
}
int checkEnd() { //정답확인 함수
	for (int i = 0; i < MSIZE; i++) {
		for (int j = 0; j < MSIZE; j++) {
			if (map[level][i][j] == 3 && playmaps[i][j] != 2)
				return 0;
		}
	}
	return 1;
}
void init(keep* s)
{
	s->top = -1;
}
// 공백 상태 검출 함수 
int is_empty(keep* s)
{
	return (s->top == -1);
}
// 포화 상태 검출 함수 
int is_full(keep* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
// 삽입함수 
void push(keep* s, element item)
{
	if (is_full(s)) {
		return;
	}
	else
	{
		s->stack[++(s->top)] = item;
	}
}
// 삭제함수 
element pop(keep* s)
{
	if (is_empty(s)) {
		walk = 0;
		return;
	}
	else {
		return s->stack[(s->top)--];
	}
}
// 피크함수 
element peek(keep* s)
{
	if (is_empty(s)) {
		walk = 0;
		return;
	}
	else
		return s->stack[s->top];
}
void dos() // 창 사이즈
{
	system("mode con: cols=46 lines=17");
	system("title 소코반(푸시푸시) by Mingyu Kim");
}
void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void Rull() {
	CLEAR;
	gotoxy(8, 6);
	textcolor(WHITE);
	printf("이 게임은 상자를 옮기는 게임입니다.");
	gotoxy(8, 8);
	textcolor(YELLOW2);	printf("P");
	textcolor(WHITE);	printf("를 이용해서 ");
	textcolor(GREEN1); printf("□");
	textcolor(WHITE);	printf("를  옮기는 게임입니다");
	gotoxy(8, 10);
	textcolor(CYAN1); printf("○");
	textcolor(WHITE); printf("의 위치로 ");
	textcolor(GREEN1); printf("□");
	textcolor(WHITE); printf("을 옮기면 됩니다.");
};

