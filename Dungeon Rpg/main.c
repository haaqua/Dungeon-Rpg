#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
#include "item.h"
#include "Monster.h"
#include "Player.h"
#include "Skill.h"
#include "Market.h"

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
	// 무기
Sword,
Axe,
Saint_Saver,
Dagger,
CrossBow,
Death_Reaper,
Wand,
Artifact,
Gambanteinn,

// 방어구
Armor,
Theif_Robe,
Magic_Robe,

// 장신구
Ring_Str,
Ring_Dex,
Ring_Wis,

// 소비
HpPotion,
StrPotion,
DexPotion,
WisPotion,
LukPotion
};

#define ITEM_TABLE_SIZE (sizeof(ItemTable) / sizeof(ItemTable[0]))

int RollPercent(int need, int percent, Using_Player* p, PlayerStatType stat) {
	int statValue = GetPlayerStat(p, stat);
	int roll = rand() % 100;

	return roll < (percent + statValue - need);
}

// 버퍼링 관련
int screenIndex;
HANDLE screen[4];
enum Mode{
	Field,
	Battle,
	Menu,
	Store
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

#define STATUS_X 22
#define STATUS_Y 1
void DrawFieldStatus(Using_Player* p)
{
	HANDLE h = screen[Field];
	wchar_t buf[64];
	int x = STATUS_X;
	int y = STATUS_Y;

	for (int i = 0; i < 12; i++) {
		DrawStringW(h, x, y + i, L"                    ");
	}

	DrawStringW(h, x, y + 0, L"[상태]");

	swprintf(buf, 64, L"HP  : %d", p->stat.hp);
	DrawStringW(h, x, y + 1, buf);

	swprintf(buf, 64, L"STR : %d", p->stat.str);
	DrawStringW(h, x, y + 2, buf);

	swprintf(buf, 64, L"DEX : %d", p->stat.dex);
	DrawStringW(h, x, y + 3, buf);

	swprintf(buf, 64, L"WIS : %d", p->stat.wis);
	DrawStringW(h, x, y + 4, buf);

	swprintf(buf, 64, L"LUK : %d", p->stat.luk);
	DrawStringW(h, x, y + 5, buf);

	DrawStringW(h, x, y + 7, L"[장비]");

	// 무기
	if (p->inv.WeaponIdx >= 0)
		DrawStringW(h, x, y + 8, p->inv.item[p->inv.WeaponIdx].name);
	else
		DrawStringW(h, x, y + 8, L"무기 : 없음");

	// 방어구
	if (p->inv.ArmorIdx >= 0)
		DrawStringW(h, x, y + 9, p->inv.item[p->inv.ArmorIdx].name);
	else
		DrawStringW(h, x, y + 9, L"방어구 : 없음");

	// 장신구
	if (p->inv.AcceIdx >= 0)
		DrawStringW(h, x, y + 10, p->inv.item[p->inv.AcceIdx].name);
	else
		DrawStringW(h, x, y + 10, L"장신구 : 없음");
}
void DrawFieldHelp() {
	HANDLE h = screen[Field];

	int y = MAZEHEIGHT + 1;

	DrawStringW(h, 0, y, L"[조작]");
	DrawStringW(h, 0, y + 1, L"이동 : ↑ ↓ ← →");
	DrawStringW(h, 0, y + 2, L"E : 인벤토리");
}
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
	DrawFieldHelp();
	DrawFieldStatus(&player);
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
void MarketMenu(Using_Player* p);

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
	if (map[ny][nx] == Market) {
		MarketMenu(&player);
		SetConsoleActiveScreenBuffer(screen[Field]);
		RenderMap();
		return;
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

	for (int i = 0; i < SKILL_SLOT_MAX; i++) {
		p->skills[i].SkillId = SKILL_NONE;
	}
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
		DrawStringW(h, INV_START_X, INV_START_Y+i, L"                    ");
	}
}
int UseItemList(Using_Player* p, int* list) {
	int count = 0;
	for (int i = 0; i < p->inv.count; i++) {
		list[count++] = i;
	}
	return count;
}   
void DrawInvList(Using_Player* p, int* list, int ListCount, int cursor) {
	HANDLE h = screen[Menu];
	for (int i = 0; i < INV_VISIBLE; i++) {
		ClearLine(h, INV_START_Y + i);

		if (i >= ListCount)continue;

		Item* it = &p->inv.item[list[i]];

		if (i == cursor)
			DrawCharToBufferW(h, INV_START_X - 1, INV_START_Y + i, L'>');

		DrawStringW(h, INV_START_X, INV_START_Y + i, it->name);

		if (list[i] == p->inv.WeaponIdx ||
			list[i] == p->inv.ArmorIdx ||
			list[i] == p->inv.AcceIdx) {
			DrawStringW(h, INV_START_X + 20, INV_START_Y + i, L"[E]");
		}
	}
}
void DrawItemDetail(Using_Player* p, int invIdx) {
	HANDLE h = screen[Menu];
	Item* it = &p->inv.item[invIdx];

	int x = 30;
	int y = 6;
	wchar_t buf[64];

	for (int i = 0; i < 6; i++) {
		DrawStringW(h, x, y + i, L"                                    ");
	}

	swprintf(buf, 64, L"이름 : %ls", it->name);
	DrawStringW(h, x, y + 0, buf);

	const wchar_t* type =
		(it->category == ITEM_EQUIP) ? L"장비" :
		(it->category == ITEM_CONSUME) ? L"소비" : L"기타";

	swprintf(buf, 64, L"종류 : %ls", type);
	DrawStringW(h, x, y + 1, buf);

	const wchar_t* stat =
		(it->stat == STAT_STR) ? L"STR" :
		(it->stat == STAT_DEX) ? L"DEX" :
		(it->stat == STAT_WIS) ? L"WIS" :
		(it->stat == STAT_LUK) ? L"LUK" :
		(it->stat == STAT_HP) ? L"HP" : L"-";

	swprintf(buf, 64, L"효과 : %ls +%d", stat, it->value);
	DrawStringW(h, x, y + 2, buf);

	swprintf(buf, 64, L"가격 : %d", it->cost);
	DrawStringW(h, x, y + 3, buf);

	if (it->category == ITEM_EQUIP) {
		DrawStringW(h, x, y + 4, L"ENTER : 장착");
	}
}
void UseItem(Using_Player* p, int inidx) {
	Item* it = &p->inv.item[inidx];

	switch (it->stat) {
	case STAT_HP:
		p->stat.hp += it->value;
		break;
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
int InventoryCursor(int* cursor, int ListCount) {
	if (!_kbhit())return -1;

	char key = _getch();
	if (key == -32 || key == 0)
		key = _getch();
	switch (key) {
	case 72:
		if (*cursor > 0)(*cursor)--;
		break;
	case 80:
		if (*cursor < ListCount - 1)(*cursor)++;
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
		int sel = InventoryCursor(&cursor, listCount);
		if (listCount > 0) {
			DrawItemDetail(p, list[cursor]);
			if (prev != cursor)
				DrawInvList(p, list, listCount, cursor);
		}
		else {
			DrawStringW(screen[Menu], INV_START_X, INV_START_Y, L"비어 있음");
		}

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
					break;
				case SLOT_ARMOR:
					EquipArmor(p, list[sel]);
					break;
				case SLOT_ACCESSORY:
					EquipAcce(p, list[sel]);
					break;
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

// 상태창
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

// 상점
void SetUpStore();
void DrawStoreUi(Using_Player* p, int cursor) {
	HANDLE h = screen[Store];
	ClearBuffer(h);

	DrawStringW(h, 1, 1, L"던전 상점");
	DrawStringW(h, 1, 2, L"보유금액 : ");
	wchar_t MoneyBuffer[32];
	swprintf(MoneyBuffer, 32, L"%d G", p->Money);
	DrawStringW(h, 8, 2, MoneyBuffer);
	DrawStringW(h, 1, 4, L"[판매목록]");
	// 물약 1
	swprintf(MoneyBuffer, 32, L"1. %-10ls : %d G", currentsell.item[0].name, currentsell.item[0].cost);
	DrawStringW(h, 2, 6, MoneyBuffer);

	// 물약 2
	swprintf(MoneyBuffer, 32, L"2. %-10ls : %d G", currentsell.item[1].name, currentsell.item[1].cost);
	DrawStringW(h, 2, 7, MoneyBuffer);

	// 스킬
	if (currentsell.HasSkill) {
		swprintf(MoneyBuffer, 32, L"3. 스킬 [%ls] : 10 G", GetSkillName(currentsell.skill));
		DrawStringW(h, 2, 8, MoneyBuffer);
	}
	else {
		DrawStringW(h, 2, 8, L"3. [품절]");
	}
	if (cursor == 3) DrawStringW(h, 1, 10, L">");
	DrawStringW(h, 2, 10, L"4. 아이템 판매하기");

	int cursorY[] = { 6,7,8,10 };
	DrawStringW(h, 1, cursorY[cursor], L">");

	DrawStringW(h, 1, 12, L"ESC : 상점 나가기 / ENTER : 구매");
}
void BuyItem(Using_Player* p, int cursor) {
	if (cursor < 2) {
		Item* it = &currentsell.item[cursor];
		if (p->Money >= it->cost) {
			if (AddItem(p, *it)) {
				p->Money -= it->cost;	
			}
		}
	}
	else if (cursor == 2 && currentsell.HasSkill) {
		if (p->Money >= 10) {
			int slot = -1;
			for (int i = 0; i < SKILL_SLOT_MAX; i++) {
				if (p->skills[i].SkillId == SKILL_NONE) {
					slot = i;
					break;
				}
			}
			if (slot != -1) {
				p->skills[slot].SkillId = currentsell.skill;
				p->Money -= 10;
				currentsell.HasSkill = 0;
			}
		}
	}
}
void SellMenu(Using_Player* p) {
	ChangeMode(Menu); // 인벤토리 UI를 재활용하기 위해 Menu 모드 사용
	int list[Inventory_Size];
	int listCount = UseItemList(p, list);
	int cursor = 0;

	int needUpdate = 1;

	while (1) {
		if (needUpdate) {
			DrawInventoryUi(p); // 상단 금액 등 기본 틀
			DrawStringW(screen[Menu], 1, 1, L"[판매 모드] - ENTER:판매, ESC:돌아가기");

			if (listCount > 0) {
				DrawInvList(p, list, listCount, cursor);
				DrawItemDetail(p, list[cursor]);

				wchar_t priceBuf[32];
				swprintf(priceBuf, 32, L"판매가 : %d G", p->inv.item[list[cursor]].cost / 5);
				DrawStringW(screen[Menu], 30, 11, priceBuf);
			}
			else {
				// 아이템이 없을 때의 화면 처리
				for (int i = 0; i < INV_VISIBLE; i++) ClearLine(screen[Menu], INV_START_Y + i);
				DrawStringW(screen[Menu], INV_START_X, INV_START_Y, L"판매할 아이템이 없습니다.");
			}
			needUpdate = 0; // 그리기가 끝났으므로 플래그를 내립니다.
		}

		int prevCursor = cursor;
		int sel = InventoryCursor(&cursor, listCount);

		if (sel == -2) break; // ESC로 나가기

		// 커서가 움직였을 때만 다음 프레임에 그리도록 설정
		if (prevCursor != cursor) {
			needUpdate = 1;
		}

		if (sel >= 0 && listCount > 0) {
			// 판매 로직
			int sellPrice = p->inv.item[list[sel]].cost / 5;
			p->Money += sellPrice;

			// 아이템 제거
			for (int i = list[sel]; i < p->inv.count - 1; i++) {
				p->inv.item[i] = p->inv.item[i + 1];
			}
			p->inv.count--;

			// 판매 후 리스트 갱신
			listCount = UseItemList(p, list);
			if (cursor >= listCount) cursor = (listCount > 0) ? listCount - 1 : 0;

			needUpdate = 1; // 아이템이 사라졌으니 화면을 다시 그려야 함
		}
		Sleep(30);
	}
	ChangeMode(Store); // 판매 종료 후 다시 상점 모드로
}
void MarketMenu(Using_Player* p) {
	ChangeMode(Store);
	SetUpStore();
	int cursor = 0;
	int prevCursor = -1;

	while (1) {
		if (prevCursor != cursor) {
			DrawStoreUi(p, cursor);
			SetConsoleActiveScreenBuffer(screen[Store]);
			prevCursor = cursor;
		}

		if (_kbhit()) {
			int key = _getch();
			if (key == 27) break;
			if (key == 224 || key == 0) {
				key = _getch();
				if (key == 72 && cursor > 0) cursor--;
				if (key == 80 && cursor < 3) cursor++;
			}
			if (key == 13) {
				if (cursor < 3) {
					BuyItem(p, cursor);
					prevCursor = -1;
				}
				else if (cursor == 3) {
					SellMenu(p);	
					prevCursor = -1;
				}
			}
		}
		Sleep(50);
	}
	ChangeMode(Field);
}

// 스킬 관련 함수
void SkillUi(Using_Player* p) {
	ChangeMode(Menu);
	HANDLE h = screen[Menu];
	ClearBuffer(h);
	DrawStringW(h, 1, 1, L"스킬");
	DrawStringW(h, 1, 2, L"Esc : 나가기");

	int y = 4;
	for (int i = 0; i < SKILL_SLOT_MAX; i++) {
		if (p->skills[i].SkillId == SKILL_NONE) continue;

		const wchar_t* name = GetSkillName(p->skills[i].SkillId);
		DrawStringW(h, 2, y++, name);
	}
	while (1) {
		if (_kbhit()) {
			int k = _getch();
			if (k == 27)break;
		}
		Sleep(50);
	}
}
int SkillCursor(Using_Player* p,int* cursor) {
	if (!_kbhit())return -1;
	char key = _getch();
	if (key == -32 || key == 0)
		key = _getch();
	int count = 0;
	for (int i = 0; i < SKILL_SLOT_MAX; i++) {
		if (p->skills[i].SkillId != SKILL_NONE)
			count++;
	}
	switch (key) {
	case 72:
		if (*cursor > 0)(*cursor)--;
		break;
	case 80:
		if (*cursor < count -1)(*cursor)++;
		break;
	case 13:
		return *cursor;
	case 27:
		return -2;
	}
	return -1;
}
void DrawSkillUiB(Using_Player* p, int cursor) {
	HANDLE h = screen[Battle];
	ClearLine(h, 16);
	ClearLine(h, 17);
	ClearLine(h, 18);
	ClearLine(h, 19);
	DrawStringW(h, 1, 16, L"[스킬 선택] Esc : 나가기");

	int y = 17;
	int idx = 0;
	for (int i = 0; i < SKILL_SLOT_MAX; i++) {
		if (p->skills[i].SkillId == SKILL_NONE) continue;
		if (idx == cursor)
			DrawStringW(h, 0, y, L">");
		DrawStringW(h, 2, y, GetSkillName(p->skills[i].SkillId));
		idx++;
		y++;
	}
}
void Skill_Effect(Using_Player* p, Monster* m, SkillType id);

// 전투 함수
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

	ClearLine(hBattle, 5);
	swprintf(hpBuf, 50, L"HP : %-5d", m->hp);
	DrawStringW(hBattle, 7, 5, hpBuf);

	ClearLine(hBattle, 12);
	swprintf(hpBuf, 50, L"플레이어 HP : %-5d", p->stat.hp);
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
		ClearLine(screen[Battle], 17);
		ClearLine(screen[Battle], 18);
		ClearLine(screen[Battle], 19);
		// 일반공격
		if (select == 0) {
			DrawStringW(screen[Battle], 1, 17, L"공격");
			if (RollPercent(m->lv * 10, 90, p, AtkStat))// 양쪽 다 1렙 기준 : 100분율 < 90 + 10(공격스탯) - 10). 즉, 약 90%의 확률로 공격
			{
				DrawStringW(screen[Battle], 1, 18, L"당신의 공격이 성공했습니다.");
				int dmg = rand() % AtkStat + 1 - m->def;
				if (dmg < 0){
					dmg = 0;
					DrawStringW(screen[Battle], 1, 19, L"당신의 공격이 너무 약했습니다.");
				}
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
			int skillCursor = 0;
			int prevCursor = -1;

			while (1) {
				DrawSkillUiB(p, skillCursor);
				if (prevCursor != skillCursor) {
					DrawSkillUiB(p, skillCursor);
					prevCursor = skillCursor;
				}
				int sel = SkillCursor(p, &skillCursor);
				if (sel == -1) {
					Sleep(50);
					continue;
				}
				if (sel == -2) {
					BattleUI(m, p, cursor);
					DrawCursor(-1, cursor);
					break;
				}
				int idx = 0;
				SkillType useSkill = SKILL_NONE;

				for (int i = 0; i < SKILL_SLOT_MAX; i++) {
					if (p->skills[i].SkillId == SKILL_NONE)
						continue;
					if (idx == sel) {
						useSkill = p->skills[i].SkillId;
						break;
					}
					idx++;
				}
				if (useSkill != SKILL_NONE) {
					ClearLine(screen[Battle], 17);
					ClearLine(screen[Battle], 18);

					DrawStringW(screen[Battle], 1, 17, L"스킬을 사용했다!");
					
					Skill_Effect(p, m, useSkill);
					UpdateHP(m, p);
				}
				Sleep(700);
				break;
			}
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
			continue;
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
				p->Money += m->lv * 10;
			}
			if (m->type == rare) {
				p->Money += m->lv * 20;
			}
			if (m->type == Boss) {
				p->Money += m->lv * 50;
			}
			Sleep(800);
			break;
		}

		/*몬스터의 턴*/
		ClearLine(screen[Battle], 16);
		ClearLine(screen[Battle], 17);
		ClearLine(screen[Battle], 18);
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
						dmg = 0;
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
				DrawStringW(screen[Battle], 1, 19, msg);
				UpdateHP(m, p);
			}
			Sleep(700);

		if (p->stat.hp <= 0) {
			DrawStringW(screen[Battle], 1, 17, L"당신은 쓰러졌다...");
			Sleep(1000);
			break;
		}
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