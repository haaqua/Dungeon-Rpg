#define _CRT_SECURE_NO_WARNINGS
#include"item.h"
#include<wchar.h>
#include"Player.h"
void ApplyItemStat(Player_Stats* stat, Item* item, int sign) {
	switch (item->stat) {
	case STAT_STR:
		stat->str += item->value * sign;
		break;
	case STAT_DEX:
		stat->dex += item->value * sign;
		break;
	case STAT_WIS:
		stat->wis += item->value * sign;
		break;
	case STAT_LUK:
		stat->luk += item->value * sign;
		break;
	case STAT_HP:
		stat->hp += item->value * sign;
		break;
	}
}
void EquipWeapon(Using_Player* p, int invIdx) {
	if (p->inv.WeaponIdx == 1) {
		ApplyItemStat(&p->stat, &p->inv.item[p->inv.WeaponIdx], -1);
	}
	p->inv.WeaponIdx = invIdx;
	ApplyItemStat(&p->stat, &p->inv.item[invIdx], +1);
}
void EquipArmor(Using_Player* p, int invIdx) {
	if (p->inv.ArmorIdx == 1) {
		ApplyItemStat(&p->stat, &p->inv.item[p->inv.ArmorIdx], -1);
	}
	p->inv.ArmorIdx = invIdx;
	ApplyItemStat(&p->stat, &p->inv.item[invIdx], +1);
}
void EquipAcce(Using_Player* p, int invIdx) {
	if (p->inv.AcceIdx == 1) {
		ApplyItemStat(&p->stat, &p->inv.item[p->inv.AcceIdx], -1);
	}
	p->inv.AcceIdx = invIdx;
	ApplyItemStat(&p->stat, &p->inv.item[invIdx], +1);
}

int GetAttckStat(Using_Player* p) {
	if (p->inv.WeaponIdx == -1)
		return p->stat.str;
	Item* w = &p->inv.item[p->inv.WeaponIdx];
	switch (w->stat) {
	case STAT_STR: return p->stat.str;
	case STAT_DEX: return p->stat.dex;
	case STAT_WIS: return p->stat.wis;
	}
	return p->stat.str;
}
int GetDefense(Using_Player* p) {
	int def = 0;

	if (p->inv.ArmorIdx == -1)
		return def;

	Item* a = &p->inv.item[p->inv.ArmorIdx];

	switch (a->stat) {
	case STAT_STR: def = p->stat.str / 2; break;
	case STAT_DEX: def = p->stat.dex / 2; break;
	case STAT_WIS: def = p->stat.wis / 2; break;
	}
	return def;
}


// 무기
Item Sword(void) {
	Item item;
	wcscpy(item.name, L"대검");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_STR;
	item.value = 3;
	item.cost = 5;
	item.usable = 0;
	return item;
}
Item Axe() {
	Item item;
	wcscpy(item.name, L"도끼");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_STR;
	item.value = 5;
	item.cost = 10;
	item.usable = 0;
	return item;
}
Item Saint_Saver() {
	Item item;
	wcscpy(item.name, L"성검");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_STR;
	item.value = 10;
	item.cost = 20;
	item.usable = 0;
	return item;
}

Item Dagger() {
	Item item;
	wcscpy(item.name, L"단검");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_DEX;
	item.value = 3;
	item.cost = 5;
	item.usable = 0;
	return item;
}
Item CrossBow() {
	Item item;
	wcscpy(item.name, L"석궁");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_DEX;
	item.value = 5;
	item.cost = 10;
	item.usable = 0;
	return item;
}
Item Death_Reaper() {
	Item item;
	wcscpy(item.name, L"데스리퍼");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_DEX;
	item.value = 10;
	item.cost = 20;
	item.usable = 0;
	return item;
}

Item Wand() {
	Item item;
	wcscpy(item.name, L"지팡이");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_WIS;
	item.value = 3;
	item.cost = 5;
	item.usable = 0;
	return item;
}
Item Artifact() {
	Item item;
	wcscpy(item.name, L"아티펙트");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_WIS;
	item.value = 5;
	item.cost = 10;
	item.usable = 0;
	return item;
}
Item Gambanteinn() {
	Item item;
	wcscpy(item.name, L"감반테인");
	item.type = ITEM_WEAPON;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_WIS;
	item.value = 10;
	item.cost = 20;
	item.usable = 0;
	return item;
}

// 방어구
Item Armor() {
	Item item;
	wcscpy(item.name, L"갑옷");
	item.type = ITEM_ARMOR;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ARMOR;
	item.stat = STAT_STR;
	item.value = 3;
	item.cost = 10;
	item.usable = 0;
	return item;
}
Item Theif_Robe() {
	Item item;
	wcscpy(item.name, L"도적망토");
	item.type = ITEM_ARMOR;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ARMOR;
	item.stat = STAT_DEX;
	item.value = 3;
	item.cost = 10;
	item.usable = 0;
	return item;
}
Item Magic_Robe() {
	Item item;
	wcscpy(item.name, L"마법망토");
	item.type = ITEM_ARMOR;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ARMOR;
	item.stat = STAT_WIS;
	item.value = 3;
	item.cost = 10;
	item.usable = 0;
	return item;
}

// 장신구
Item Ring_Str() {
	Item item;
	wcscpy(item.name, L"힘의 반지");
	item.type = ITEM_ACCESSORY;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ACCESSORY;
	item.stat = STAT_STR;
	item.value = 5;
	item.cost = 15;
	item.usable = 0;
	return item;
}

Item Ring_Dex() {
	Item item;
	wcscpy(item.name, L"신속의 장화");
	item.type = ITEM_ACCESSORY;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ACCESSORY;
	item.stat = STAT_DEX;
	item.value = 5;
	item.cost = 15;
	item.usable = 0;
	return item;
}

Item Ring_Wis() {
	Item item;
	wcscpy(item.name, L"지식의 목걸이");
	item.type = ITEM_ACCESSORY;
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ACCESSORY;
	item.stat = STAT_WIS;
	item.value = 5;
	item.cost = 15;
	item.usable = 0;
	return item;
}
// 사용템
Item HpPotion(void) {
	Item item;
	wcscpy(item.name, L"체력 포션");
	item.type = ITEM_NONE;
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_HP;
	item.value = 5;
	item.cost = 5;
	item.usable = 1;
	return item;
}

Item StrPotion() {
	Item item;
	wcscpy(item.name, L"힘의 비약");
	item.type = ITEM_NONE;
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_STR;
	item.value = 1;
	item.cost = 20;
	item.usable = 1;
	return item;
}

Item DexPotion() {
	Item item;
	wcscpy(item.name, L"암살자의 성장약");
	item.type = ITEM_NONE;
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_DEX;
	item.value = 1;
	item.cost = 20;
	item.usable = 1;
	return item;
}

Item WisPotion() {
	Item item;
	wcscpy(item.name, L"지식의 샘물");
	item.type = ITEM_NONE;
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_WIS;
	item.value = 1;
	item.cost = 20;
	item.usable = 1;
	return item;
}

Item LukPotion() {
	Item item;
	wcscpy(item.name, L"운이 좋은 포션");
	item.type = ITEM_NONE;
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_LUK;
	item.value = 1;
	item.cost = 30;
	item.usable = 1;
	return item;
}