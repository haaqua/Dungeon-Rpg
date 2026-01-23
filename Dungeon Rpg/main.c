#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
#include "item.h"

// 스탯
typedef struct {
	int hp;
	int str;
	int def;
	int wis;
	int luk;
}Player_Stats;

// 인벤토리
#define Inventory_Size 20
typedef struct {
	Item item[Inventory_Size];
	int count;

	Item* Weapon;
	Item* Armor;
	Item* Accessory;
	Item* None;
}Inventory;

// 플레이어.
typedef struct {
	Player_Stats stat;
	Inventory inv;
}Using_Player;


int screenIndex;
HANDLE screen[4];
enum Mode{
	Field,
	Battle,
	Menu
};
enum Mode NowMode = Field;

void Initialize()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = 1;

	for (int i = 0; i < 4; i++) {
		screen[i] = CreateConsoleScreenBuffer
		(
			GENERIC_READ | GENERIC_WRITE,
			0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL
		);
		SetConsoleCursorInfo(screen[i], &cursor);
	}

	SetConsoleActiveScreenBuffer(screen[0]);
}
void DrawCharToBufferW(HANDLE hBuffer, int x, int y, wchar_t ch)
{
	DWORD written;
	COORD pos = { x * 2, y }; // ■□가 2칸이므로 x*2
	WriteConsoleOutputCharacterW(
		hBuffer,
		&ch,
		1,
		pos,
		&written
	);
}

void ChangeMode(enum Mode mode) {
	NowMode = mode;
	screenIndex = mode;

	SetConsoleActiveScreenBuffer(screen[screenIndex]);
}

// 맵 생성 함수들
#define MAZEWIDTH  20
#define MAZEHEIGHT 20
wchar_t map[MAZEHEIGHT][MAZEWIDTH];

#define Wall   L'■'
#define Empty  L'□'
#define Chest  L'★'
#define Player L'@'
#define Exit   L'▽'
int plX = 1;
int plY = 1;

void MakeMap(){
	
	for (int y = 0; y < MAZEHEIGHT; y++) {
		for (int x = 0; x < MAZEWIDTH; x++) {
			map[y][x] = Wall;
		}
	}
	int x = 1, y = 1;
	map[y][x] = Empty;

	for (int i = 0; i < 500; i++) {
		int dir = rand() % 4;

		int nx = x, ny = y;

		if (dir == 0)nx++;
		if (dir == 1) nx--;
		if (dir == 2) ny++;
		if (dir == 3) ny--;

		if (nx > 0 && nx < MAZEWIDTH - 1 && ny>0 && ny < MAZEHEIGHT - 1) {
			x = nx;
			y = ny;
			map[y][x] = Empty;
		}
	}
}
void RenderMap() {
	HANDLE hField = screen[Field];
	for (int y = 0; y < MAZEHEIGHT; y++) {
		for (int x = 0; x < MAZEWIDTH; x++) {
			DrawCharToBufferW(hField, x, y, map[y][x]);
		}
	}
	DrawCharToBufferW(hField, plX, plY, Player);
}
void ItemPlace(int percent) {
	for (int y = 0; y < MAZEHEIGHT; y++) {
		for (int x = 0; x < MAZEWIDTH; x++) {
			if (map[y][x] == Empty && x != plX && y != plY) {
				int I = rand() % 100;
				if (I < percent)
					map[y][x] = Chest;
			}
		}
	}
}
void ExitPlace() {
	
	while (1) {
		int x = rand() % MAZEWIDTH;
		int y = rand() % MAZEHEIGHT;
		if (map[y][x] == Empty &&!(x == plX && y == plY)) {
			map[y][x] = Exit;
			break;
		}
	}
}

void MapFunction() {
	Initialize();
	srand(time(NULL));

	plX = 1;
	plY = 1;

	MakeMap();
	ItemPlace(5);
	ExitPlace();
	RenderMap();
	SetConsoleActiveScreenBuffer(screen[Field]);
}

// 플레이어 이동 함수
void PlayerMove(int nx, int ny);
void MoveInput() {
	if (NowMode != Field) 
		return;
	if (!_kbhit()) 
		return;
	char key = _getch();
	if (key == -32 || key == 0)
	{
		key = _getch();
	}
	int nx = plX, ny = plY;

	switch (key) {
	case 72: ny--;	break;
	case 75: nx--;	break;
	case 77: nx++;	break;
	case 80: ny++;	break;
	default: return;
	}
	PlayerMove(nx, ny);
}
void PlayerMove(int nx, int ny) {
	if (nx < 0 || nx >= MAZEWIDTH || ny < 0 || ny >= MAZEHEIGHT)
		return;
	wchar_t target = map[ny][nx];
	if (target == Wall)
		return;
	HANDLE hfield = screen[Field];

	DrawCharToBufferW(hfield, plX, plY, map[plY][plX]);

	if (map[ny][nx] == Chest) {
		map[ny][nx] = Empty;
	}
	if (map[ny][nx] == Exit) {
		MapFunction();
		nx = 1;
		ny = 1;
	}

	plX = nx;
	plY = ny;

	DrawCharToBufferW(hfield, plX, plY, Player);
}

// 플레이어 생성 함수
void PlayerCreat(Using_Player* p) {
	p->stat.hp = 10;
	p->stat.str = 5;
	p->stat.def = 5;
	p->stat.wis = 5;
	p->stat.luk = 5;
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	MapFunction();

	while (1) {
		MoveInput();
		Sleep(30);
	}

	return 0;
}