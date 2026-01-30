#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
#include "item.h"
#include "Monster.h"
#include "Player.h"

// 플레이어.
int GetPlayerStat(Using_Player* p, PlayerStatType stat) {
	switch (stat) {
	case PSTAT_STR: return p->stat.str;
	case PSTAT_DEX: return p->stat.dex;
	case PSTAT_WIS: return p->stat.wis;
	case PSTAT_LUK: return p->stat.luk;
	default: return 0;
	}
}
Using_Player player;

typedef Item(*ItemFactory)(void);

// 랜덤 아이템 풀
ItemFactory ItemTable[] = {
	Sword,
	Dagger,
	Wand,
	Armor,
	Ring_Str,
	Ring_Dex,
	HpPotion,
	StrPotion,
	DexPotion,
	LukPotion
};

#define ITEM_TABLE_SIZE (sizeof(ItemTable) / sizeof(ItemTable[0]))

// 버퍼링 관련
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
void DrawStringW(HANDLE hbuffer, int x, int y, const wchar_t* str) {

	if (hbuffer == NULL || hbuffer == INVALID_HANDLE_VALUE)
		return;

	if (str == NULL)
		return;

	DWORD written;
	COORD pos = { x * 2, y };
	WriteConsoleOutputCharacterW( 
		hbuffer,
		str,
		wcslen(str),
		pos,
		&written
	);
}
void ClearBuffer(HANDLE hBuffer) {
	DWORD written;
	COORD pos = { 0, 0 };
	FillConsoleOutputCharacterW(
		hBuffer,
		L' ',
		80 * 25,
		pos,
		&written
	);
}
void ClearLine(HANDLE hBuffer, int y) {
	DWORD written;
	COORD pos = { 0, y };
	FillConsoleOutputCharacterW(
		hBuffer,
		L' ',
		80,   
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
#define Market L'◎'
#define Player L'@'
#define Exit   L'▽'
int plX = 1;
int plY = 1;

int MazeLv = 1;

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
void ItemMarket() {
	while (1) {
		int x = rand() % MAZEWIDTH;
		int y = rand() % MAZEHEIGHT;
		if (map[y][x] == Empty && !(x == plX && y == plY)) {
			map[y][x] = Market;
			break;
		}
	}
}
void ExitPlace() {

	while (1) {
		int x = rand() % MAZEWIDTH;
		int y = rand() % MAZEHEIGHT;
		if (map[y][x] == Empty && !(x == plX && y == plY)) {
			map[y][x] = Exit;
			break;
		}
	}
}

void MapFunction() {
	plX = 1;
	plY = 1;

	MakeMap();
	ItemPlace(5);
	ItemMarket();
	ExitPlace();
	RenderMap();
	SetConsoleActiveScreenBuffer(screen[Field]);
}

void BattleFunction(Using_Player* p, Monster* m);
// 플레이어 이동 함수
Monster MonsterEncounter(Using_Player* p, int isBoss);
void InventoryMenu(Using_Player* p);
int AddItem(Using_Player* p, Item item);
Item GetRandItem();

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

	case 'E':
	case'e':
		InventoryMenu(&player);
		ChangeMode(Field);
		SetConsoleActiveScreenBuffer(screen[Field]);
		RenderMap();
		break;
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
		Item it = GetRandItem();
		AddItem(&player, it);
		map[ny][nx] = Empty;
	}
	if (map[ny][nx] == Exit) {
		Monster boss = MonsterEncounter(&player, 1);
		BattleFunction(&player, &boss);

		MazeLv++;
		MapFunction();
		nx = 1;
		ny = 1;
	}

	plX = nx;
	plY = ny;

	DrawCharToBufferW(hfield, plX, plY, Player);

	if(BattleEncounter(10, &player, PSTAT_LUK)){
		Monster m = MonsterEncounter(&player, 0);
		BattleFunction(&player, &m);
	}
}
// 상자까는 함수
int AddItem(Using_Player* p, Item item) {
	if (p->inv.count >= Inventory_Size) {
		DrawStringW(screen[Field], 18, 3,L"가방이 가득하다");
		Sleep(100);
		DrawStringW(screen[Field], 18, 3, L"        ");
		return 0;
	}
	p->inv.item[p->inv.count++] = item;
	return 1;
}
Item GetRandItem() {
	int idx = rand() % ITEM_TABLE_SIZE;
	return ItemTable[idx]();
}

// 플레이어 생성 함수
void PlayerCreat(Using_Player* p) {
	p->stat.hp = 50;
	p->stat.str = 10;
	p->stat.dex = 10;
	p->stat.wis = 10;
	p->stat.luk = 5;

	p->inv.count = 0;

	p->inv.WeaponIdx = -1;
	p->inv.ArmorIdx = -1;
	p->inv.AcceIdx = -1;
}
// 인벤토리
#define INV_START_X  2
#define INV_START_Y  4
#define INV_VISIBLE  10 
void DrawInventoryUi(Using_Player* p) {
	HANDLE h = screen[Menu];
	ClearBuffer(h);

	DrawStringW(h, 1, 1, L"[인벤토리]");
	DrawStringW(h, 1, 2, L"ENTER : 사용,  ESC : 나가기");
	DrawStringW(h, 1, 3, L"돈 : ");
	wchar_t moneyBuf[32];
	swprintf(moneyBuf, 32, L"%d", p->Money);
	DrawStringW(h, 6, 3, moneyBuf);

	for (int i = 0; i < INV_VISIBLE; i++) {
		DrawStringW(h, INV_START_X, INV_START_Y+1, L"                    ");
	}
}
int UseItemList(Using_Player* p, int* list) {
	int count = 0;
	for (int i = 0; i < p->inv.count; i++) {
		list[count++] = i;
		
		return count;
	}
}
void DrawInvList(Using_Player* p, int* list, int ListCount, int cursor) {
	HANDLE h = screen[Menu];
	for (int i = 0; i < INV_VISIBLE; i++) {
		ClearLine(h, INV_START_Y + 1);

		int idx = i;
		if (idx >= ListCount)continue;

		Item* it = &p->inv.item[list[idx]];

		if (idx == cursor)
			DrawCharToBufferW(h, INV_START_X, INV_START_Y + 1, L">");
		DrawStringW(h, INV_START_X, INV_START_Y + 1, it->name);
	}
}
void UseItem(Using_Player* p, int inidx) {
	Item* it = &p->inv.item[inidx];

	switch (it->stat) {
	case STAT_STR:
		p->stat.str += it->value;
		break;
	case STAT_DEX:
		p->stat.dex += it->value;
		break;
	case STAT_WIS:
		p->stat.wis += it->value;
		break;
	case STAT_LUK:
		p->stat.luk += it->value;
		break;
	}
	for (int i = inidx; i < p->inv.count - 1; i++) {
		p->inv.item[i] = p->inv.item[i + 1];
	}
	p->inv.count--;
}
int InventoryCursor(int* cursor) {
	if (!_kbhit())return -1;

	char key = _getch();
	if (key == -32 || key == 0)
		key = _getch();
	switch (key) {
	case 72:
		if (*cursor > 0)(*cursor)--;
		break;
	case 80:
		if (*cursor < 4)(*cursor)++;
		break;
	case 13:
		return *cursor;
	case 27:
		return -2;
	}
	return -1;
}
void InventoryMenu(Using_Player* p) {
	ChangeMode(Menu);
	SetConsoleActiveScreenBuffer(screen[Menu]);

	int list[Inventory_Size];
	int listCount = UseItemList(p, list);
	int cursor = 0;

	DrawInventoryUi(p);
	DrawInvList(p, list, listCount, cursor);

	while (1) {
		int prev = cursor;
		int sel = InventoryCursor(&cursor);

		if (prev != cursor)
			DrawInvList(p, list, listCount, cursor);
		if (sel == -2) break;

		if (sel >= 0) {
			Item* it = &p->inv.item[list[sel]];
			if (it->category == ITEM_CONSUME) {
				UseItem(p, list[sel]);
			}
			else if (it->category == ITEM_EQUIP) {
				switch (it->slot) {
				case SLOT_WEAPON:
					EquipWeapon(p, list[sel]);
				}
			}
			listCount = UseItemList(p, list);
			if (cursor >= listCount)cursor = listCount - 1;
			DrawInvList(p, list, listCount, cursor);
		}
		Sleep(50);
	}
	return;
}
void ShowStatus(Using_Player* p, Monster* m) {
	int xL = 1;
	int xR = 25;
	int y = 5;

	DrawStringW(screen[Battle], xL, y, L"몬스터 스탯");
	DrawStringW(screen[Battle], xR, y, L"플레이어 스탯");

	DrawStringW(screen[Battle], xL, y + 1, L"----------------------|------------------------");

	// 몬스터
	DrawStringW(screen[Battle], xL, y + 2, L"이름   : ");
	DrawStringW(screen[Battle], xL + 9, y + 2, m->name);

	wchar_t buf[64];

	swprintf(buf, 64, L"HP     : %d", m->hp);
	DrawStringW(screen[Battle], xL, y + 3, buf);

	swprintf(buf, 64, L"ATK    : %d", m->atk);
	DrawStringW(screen[Battle], xL, y + 4, buf);

	swprintf(buf, 64, L"DEF    : %d", m->def);
	DrawStringW(screen[Battle], xL, y + 5, buf);

	swprintf(buf, 64, L"LV     : %d", m->lv);
	DrawStringW(screen[Battle], xL, y + 6, buf);

	// 플레이어
	DrawStringW(screen[Battle], xR, y + 2, L"이름   : 플레이어");

	swprintf(buf, 64, L"HP     : %d", p->stat.hp);
	DrawStringW(screen[Battle], xR, y + 3, buf);

	swprintf(buf, 64, L"STR    : %d", p->stat.str);
	DrawStringW(screen[Battle], xR, y + 4, buf);

	swprintf(buf, 64, L"DEX    : %d", p->stat.dex);
	DrawStringW(screen[Battle], xR, y + 5, buf);

	swprintf(buf, 64, L"WIS    : %d", p->stat.wis);
	DrawStringW(screen[Battle], xR, y + 6, buf);

	swprintf(buf, 64, L"LUK    : %d", p->stat.luk);
	DrawStringW(screen[Battle], xR, y + 7, buf);
}

// 전투 함수
int calculate(int atk, int def) {
	int dmg = atk - def;
	if (dmg < 1) {
		dmg = 1;
	}
	return dmg;
}
int RollPercent(int need, int percent, Using_Player* p, PlayerStatType stat) {
	int statValue = GetPlayerStat(p, stat);
	int roll = rand() % 100;

	return roll < (percent + statValue - need);
}
void BattleUI(Monster* m, Using_Player* p, int cursor) {
	HANDLE hBattle = screen[Battle];
	ClearBuffer(hBattle);

	DrawStringW(hBattle, 10, 2, m->name);
	// 메뉴
	const wchar_t* menu[5] = {
		L"공격",
		L"아이템",
		L"스킬",
		L"상태",
		L"도망"
	};
	for (int i = 0; i < 5; i++) {
		DrawStringW(hBattle, 6 + i * 6, 15, menu[i]);
	}

}
void DrawCursor(int prev, int cur) {
	HANDLE hBattle = screen[Battle];
	if (prev >= 0) {
		DrawStringW(hBattle, 5 + prev * 6, 15, L" ");
	}
	DrawStringW(hBattle, 5 + cur * 6, 15, L">");
}
void UpdateHP(Monster* m, Using_Player* p) {
	HANDLE hBattle = screen[Battle];

	wchar_t hpBuf[50];
	swprintf(hpBuf, 50, L"HP : %d", m->hp);
	DrawStringW(hBattle, 7, 5, hpBuf);

	ClearLine(hBattle, 12);
	swprintf(hpBuf, 50, L"플레이어 HP : %d", p->stat.hp);
	DrawStringW(hBattle, 1, 12, hpBuf);
}

int BattleInput(int* cursor) {
	if (!_kbhit())return -1;

	char key = _getch();
	if (key == -32 || key == 0)
		key = _getch();

	switch (key) {
	case 75:
		if (*cursor > 0)(*cursor)--;
		break;
	case 77:
		if (*cursor < 4)(*cursor)++;
		break;
	case 13:
		return *cursor;
	}
	return - 1;
}
void BattleFunction(Using_Player* p, Monster* m) {
	ChangeMode(Battle);
	SetConsoleActiveScreenBuffer(screen[Battle]);

	int cursor = 0;

	BattleUI(m, p, cursor);
	DrawCursor(-1, cursor);
	while (1) {
		int AtkStat = GetAttckStat(p);
		UpdateHP(m, p);
		int prevCursor = cursor;
		int select = BattleInput(&cursor);
		if (prevCursor != cursor) {
			DrawCursor(prevCursor, cursor);
		}
		if (select == -1) {
			Sleep(50);
			continue;
		}
		// 플레이어 행동
		// 공격
		ClearLine(screen[Battle], 17);
		ClearLine(screen[Battle], 18);
		ClearLine(screen[Battle], 19);
		if (select == 0) {
			DrawStringW(screen[Battle], 1, 17, L"공격");
			if (RollPercent(m->lv * 10, 50, p, AtkStat)) {
				DrawStringW(screen[Battle], 1, 18, L"당신의 공격이 성공했습니다.");
				int dmg = calculate(AtkStat, m->def);	
				m->hp -= dmg;
				UpdateHP(m, p);
			}
			else {
				DrawStringW(screen[Battle], 1, 18, L"당신의 공격이 빗나갔습니다.");
			}
		}
		// 아이템
		else if (select == 1) {
			InventoryMenu(p);
			SetConsoleActiveScreenBuffer(screen[Battle]);
			BattleUI(m, p, cursor);
			DrawCursor(-1, cursor);
		}
		// 스킬
		else if (select == 2) {

		}
		// 상태
		else if (select == 3) {
			ClearLine(screen[Battle], 5);
			ClearLine(screen[Battle], 6);
			ClearLine(screen[Battle], 7);
			ClearLine(screen[Battle], 8);
			ClearLine(screen[Battle], 9);
			ClearLine(screen[Battle], 10);
			ClearLine(screen[Battle], 11);
			ClearLine(screen[Battle], 12);

			ShowStatus(p, m);

			while (_kbhit() == 0) {
				Sleep(50);
			}
			_getch();
			for (int i = 5; i <= 12; i++)
				ClearLine(screen[Battle], i);
			BattleUI(m, p, cursor);
			DrawCursor(-1, cursor);
		}
		// 도망
		else if (select == 4) {
			DrawStringW(screen[Battle], 1, 17, L"도망친다");
			if (RollPercent(m->lv * 10, 50, p, PSTAT_DEX)) {
				DrawStringW(screen[Battle], 1, 18, L"당신은 도망쳤다.");
				break;
			}
			else {
				DrawStringW(screen[Battle], 1, 18, L"당신은 도망칠 수 없었다.");
			}
		}
		Sleep(700);

		if (m->hp <= 0) {
			DrawStringW(screen[Battle], 1, 19, L"몬스터를 처치했다.");
			if (m->type == common) {
				p->Money += m->lv;
			}
			if (m->type == rare) {
				p->Money += m->lv * 2;
			}
			if (m->type == Boss) {
				p->Money += m->lv * 5;
			}
			Sleep(800);
			break;
		}

		/*몬스터의 턴*/
		ClearLine(screen[Battle], 17);
		ClearLine(screen[Battle], 18);
		int Player_Defence = GetDefense(p);

			int AorS = rand() % 2;
			if (AorS == 0) {
				DrawStringW(screen[Battle], 1, 17, L"몬스터가 공격해온다!");
				int avoid = rand() % p->stat.dex;
				if (m->atk < avoid) {
					DrawStringW(screen[Battle], 1, 18, L"당신은 회피했다!");
				}
				else {
					DrawStringW(screen[Battle], 1, 18, L"공격이 명중했다!");
					int dmg = m->atk - Player_Defence;
					if (dmg < 0) {
						DrawStringW(screen[Battle], 1, 19, L"몬스터의 공격은 당신의 방어구를 뚫지 못했다!");
					}
					p->stat.hp -= dmg;
					UpdateHP(m, p);
				}
			}
			else {
				int idx = rand() % m->skill_count;
				const wchar_t* msg = m->skill[idx](m,p);
				m->skill[idx](m,p);
				DrawStringW(screen[Battle], 1, 18, L"몬스터가 스킬을 사용했다!");
				DrawStringW(screen[Battle], 1, 18, msg);
			}
			Sleep(700);

		if (p->stat.hp <= 0) {
			DrawStringW(screen[Battle], 1, 17, L"당신은 쓰러졌다...");
			Sleep(1000);
			break;
		}
		UpdateHP(m, p);
	}
	ChangeMode(Field);
	SetConsoleActiveScreenBuffer(screen[Field]);
}
Monster MonsterEncounter(Using_Player* p, int isBoss);

int BattleEncounter(int base, Using_Player* p, PlayerStatType stat) {
	int statvalue = GetPlayerStat(p, stat);

	int chance = base - statvalue;
	if (chance < 1)chance = 1;

	return (rand() % 100) < chance;
}
mon_type MonsterGrade(Using_Player* p) {
	int luk = p->stat.luk;
	int roll = rand() % 100;

	int commonchance = 50 + luk;
	if (commonchance > 95) commonchance = 95;

	if (roll < commonchance) {
		return common;
	}
	else {
		return rare;
	}
}
Monster(*commontable[])(int) = {
	Slime,
	Goblin,
	Imp
};
Monster(*raretable[])(int) = {
	Orc,
	Vampire
};
Monster MonsterEncounter(Using_Player* p, int isBoss) {
	mon_type type = MonsterGrade(p);
	int level = MazeLv;

	if (isBoss) {
		return Dragon(level);
	}


	if (type == common) {
		int count = sizeof(commontable) / sizeof(commontable[0]);
		int idx = rand() % count;
		return commontable[idx](level);
	}
	else {
		int count = sizeof(raretable) / sizeof(raretable[0]);
		int idx = rand() % count;
		return raretable[idx](level);
	}
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	Initialize();
	srand(time(NULL));

	PlayerCreat(&player);
	MapFunction();

	while (1) {
		MoveInput();
		Sleep(30);
		if (player.stat.hp <= 0) {
			break;
		}
	}
	printf("GAME OVER");

	return 0;
}