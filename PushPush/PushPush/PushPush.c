#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

//���� ����
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

#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.

#define UP 72  // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN 80 
#define LEFT 75
#define RIGHT 77


int level = 0, walk = 0; // ���罺������,������
int px, py; //�÷��̾� ��ġ
#define MSIZE 13
#define MAXLEVEL 10 // �ִ뷹��
#define MAX_STACK_SIZE 200
#define CLEAR system("cls");
			// �� 0: �� , 1 : ����� , 2: �ڽ� 3: �ű�°� 4:�÷��̾� 
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
void drawstat();//�����̶� �������� ��ȭ
void move(int ch);
void Reverse_move(int ch);
int checkEnd();
void start();
void push(keep* s, element data);// ���� ����
element pop(keep* s);
void init(keep* s);
element peek(keep* s);// ���� ����
void setlevel();
void dos();
void removeCursor(void);
void Rull();
//-----------------------------------------------


int main()
{
	dos();
	removeCursor();
	int ch, back;//�Է°� ������
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
		for (int i = 0; i < MSIZE; i++) { //�÷��� �� �� ����
			for (int j = 0; j < MSIZE; j++) {
				playmaps[i][j] = map[level][i][j];
			}
		}
		for (int i = 0; i < MSIZE; i++) { // �÷��̾� ��ġ ã��
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
			if (ch == 0 || ch == 0xE0) //�Է�Ű�� ����Ű�� ���
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
			if (checkEnd() == 1) //�����ϰ��
			{
				CLEAR;
				if (level < MAXLEVEL - 1) {
					gotoxy(15, 4);
					textcolor(YELLOW1);
					printf("���ϵ帳�ϴ�");
					gotoxy(14, 6);
					printf("%d ������ Ǯ�����ϴ�", level + 1);
					gotoxy(14, 8);
					textcolor(GREEN1);
					printf("%d ������ �Ѿ�ϴ�", level + 2);
					gotoxy(15, 10);
					textcolor(WHITE);
					printf("�ƹ�Ű�� �����ּ���\n");
					_getch();
					level++;
				}
				else if (level == MAXLEVEL - 1) {
					gotoxy(15, 5);
					textcolor(YELLOW1);
					printf("���ϵ帳�ϴ�");
					gotoxy(12, 7);
					textcolor(RED2);
					printf("������");
					textcolor(YELLOW1);
					printf("������ Ǯ�����ϴ�");
					gotoxy(13, 9);
					textcolor(GREEN1);
					printf("���α׷��� ����˴ϴ�", level + 2);
					gotoxy(12, 11);
					textcolor(WHITE);
					printf("�ƹ�Ű�� �����ּ���\n");
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
	printf("������ �Է����ּ���(1~10)");
	gotoxy(10, 8);
	printf("���� ������ ��� �Է���");
	gotoxy(10, 10);
	printf("1���� �����Դϴ�.");
	gotoxy(21, 12);
	scanf_s("%d", &level);
	level--;
}
void gotoxy(int x, int y) // gotoxy ��ǥ�Լ� 
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void textcolor(int color) // �÷� �Լ�
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void start() {
	gotoxy(2, 4);
	printf(" �âââ�   ��    ��   �âââ�   ��    ��   ");
	gotoxy(2, 5);
	printf(" ��    ��   ��    ��   ��         ��    ��   ");
	gotoxy(2, 6);
	printf(" �âââ�   ��    ��   �âââ�   �âââ�   ");
	gotoxy(2, 7);
	printf(" ��         ��    ��         ��   ��    ��   ");
	gotoxy(2, 8);
	printf(" ��         �âââ�   �âââ�   ��    ��   ");
	gotoxy(23, 11);
	printf("made by Mingyu Kim");
}
void mapdraw() {	// �� 0: �� , 1 : ����� , 2: �ڽ� 3: �ű�°� 4:�÷��̾� 
	int i, j, ch;//ch�� ���� �Ǻ�
	i = j = 0;
	gotoxy(0, 0);
	for (i = 0; i < MSIZE; i++)
	{
		for (j = 0; j < MSIZE; j++)
		{
			ch = playmaps[i][j];
			switch (ch) {
			case 0: textcolor(GRAY1); printf("��"); break;
			case 1: printf("  "); break;
			case 2: textcolor(GREEN1); printf("��"); break;
			case 3: textcolor(CYAN1); printf("��"); break;
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
	printf("������������������������������\n");
	for (i = 0; i < 2; i++)
	{
		gotoxy(xx, 2 + i);
		printf("��             ��\n");
	}
	gotoxy(xx, 2 + i);
	printf("������������������������������\n");
	gotoxy(31, 2);
	textcolor(BLUE2);
	printf("Level : %d", level + 1);
	gotoxy(31, 3);
	printf("Walk  : %d", walk);
	textcolor(YELLOW2);
	gotoxy(29, 5);
	printf("����� : Move");
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

	if (playmaps[py + y][px + x] != 0) //���� �ƴҰ��
	{
		push(&way, ch);// �� 0: �� , 1 : ����� , 2: �ڽ� 3: �ű�°� 4:�÷��̾� 
		if (playmaps[py + y][px + x] == 2) //���� �ƴϰ� �ڽ��ϰ��
		{
			if (playmaps[py + y * 2][px + x * 2] == 2 || playmaps[py + y * 2][px + x * 2] == 0) //�ڽ� �ٷ� ���� �ڽ��̰ų� ���ϰ��
			{
				return; //����
			}
			else if (map[level][py][px] == 3) {
				playmaps[py][px] = 3;
				playmaps[py + y][px + x] = 4;
				playmaps[py + y * 2][px + x * 2] = 2;
			}
			else		// ������̰ų� �ű���ϰ��
			{
				playmaps[py][px] = 1;
				playmaps[py + y][px + x] = 4;
				playmaps[py + y * 2][px + x * 2] = 2;
			}
		}
		else if (map[level][py][px] == 3) // �÷��̾��� �� ��ġ�� �ű�� ���ϰ��
		{
			playmaps[py][px] = 3;
			playmaps[py + y][px + x] = 4;
		}
		else // ���� �� �����ϰ��
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
void Reverse_move(int ch) { //�Ųٷ� �����̱�.
	int x = 0, y = 0;
	switch (ch) {
	case UP: y = 1; break;
	case DOWN: y = -1; break;
	case RIGHT: x = -1; break;
	case LEFT: x = 1; break;
	}
	// �� 0: �� , 1 : ����� , 2: �ڽ� 3: �ű�°� 4:�÷��̾� 
	if (playmaps[py + y][px + x] != 0) //���� ���� ���� �ƴҰ��
	{
		// �� 0: �� , 1 : ����� , 2: �ڽ� 3: �ű�°� 4:�÷��̾� 
		if (playmaps[py - y][px - x] == 2) //���� �ƴϰ� �ڽ��ϰ��
		{
			if (map[level][py - y][px - x] == 3)	 //�ű���ϰ��
			{
				playmaps[py - y][px - x] = 3;
				playmaps[py][px] = 2;
				playmaps[py + y][px + x] = 4;
			}
			else // ������ϰ��
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
		else if (map[level][py][px] == 3) // �÷��̾��� �� ��ġ�� �����ϰ��
		{
			playmaps[py][px] = 3;
			playmaps[py + y][px + x] = 4;
		}
		else // ���� �� �����ϰ��
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
int checkEnd() { //����Ȯ�� �Լ�
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
// ���� ���� ���� �Լ� 
int is_empty(keep* s)
{
	return (s->top == -1);
}
// ��ȭ ���� ���� �Լ� 
int is_full(keep* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
// �����Լ� 
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
// �����Լ� 
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
// ��ũ�Լ� 
element peek(keep* s)
{
	if (is_empty(s)) {
		walk = 0;
		return;
	}
	else
		return s->stack[s->top];
}
void dos() // â ������
{
	system("mode con: cols=46 lines=17");
	system("title ���ڹ�(Ǫ��Ǫ��) by Mingyu Kim");
}
void removeCursor(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void Rull() {
	CLEAR;
	gotoxy(8, 6);
	textcolor(WHITE);
	printf("�� ������ ���ڸ� �ű�� �����Դϴ�.");
	gotoxy(8, 8);
	textcolor(YELLOW2);	printf("P");
	textcolor(WHITE);	printf("�� �̿��ؼ� ");
	textcolor(GREEN1); printf("��");
	textcolor(WHITE);	printf("��  �ű�� �����Դϴ�");
	gotoxy(8, 10);
	textcolor(CYAN1); printf("��");
	textcolor(WHITE); printf("�� ��ġ�� ");
	textcolor(GREEN1); printf("��");
	textcolor(WHITE); printf("�� �ű�� �˴ϴ�.");
};

