#define _CRT_SECURE_NO_WARNINGS
#include"item.h"
#include<wchar.h>


// 무기
Item Sword(void) {
	Item item;
	wcscpy(item.name, L"대검");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_STR;
	item.value = 3;
	item.cost = 5;
	return item;
}
Item Axe() {
	Item item;
	wcscpy(item.name, L"도끼");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_STR;
	item.value = 5;
	item.cost = 10;
	return item;
}
Item Saint_Saver() {
	Item item;
	wcscpy(item.name, L"성검");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_STR;
	item.value = 10;
	item.cost = 20;
	return item;
}

Item Dagger() {
	Item item;
	wcscpy(item.name, L"단검");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_DEX;
	item.value = 3;
	item.cost = 5;

	return item;
}
Item CrossBow() {
	Item item;
	wcscpy(item.name, L"석궁");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_DEX;
	item.value = 5;
	return item;
	item.cost = 10;
}
Item Death_Reaper() {
	Item item;
	wcscpy(item.name, L"데스리퍼");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_DEX;
	item.value = 10;
	item.cost = 20;
	return item;
}

Item Wand() {
	Item item;
	wcscpy(item.name, L"지팡이");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_WIS;
	item.value = 3;
	item.cost = 5;

	return item;
}
Item Artifact() {
	Item item;
	wcscpy(item.name, L"아티펙트");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_WIS;
	item.value = 5;
	item.cost = 10;
	return item;
}
Item Gambanteinn() {
	Item item;
	wcscpy(item.name, L"감반테인");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_WIS;
	item.value = 10;
	item.cost = 20;
	return item;
}

// 방어구
Item Armor() {
	Item item;
	wcscpy(item.name, L"갑옷");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ARMOR;
	item.stat = STAT_STR;
	item.value = 3;
	item.cost = 10;
	return item;
}
Item Theif_Robe() {
	Item item;
	wcscpy(item.name, L"도적망토");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ARMOR;
	item.stat = STAT_DEX;
	item.value = 3;
	item.cost = 10;
	return item;
}
Item Magic_Robe() {
	Item item;
	wcscpy(item.name, L"마법망토");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ARMOR;
	item.stat = STAT_WIS;
	item.value = 3;
	item.cost = 10;
	return item;
}

// 장신구
Item Ring_Str() {
	Item item;
	wcscpy(item.name, L"힘의 반지");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ACCESSORY;
	item.stat = STAT_STR;
	item.value = 5;
	item.cost = 15;
	return item;
}
Item Ring_Dex() {
	Item item;
	wcscpy(item.name, L"신속의 장화");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ACCESSORY;
	item.stat = STAT_DEX;
	item.value = 5;
	item.cost = 15;
	return item;
}
Item Ring_Wis() {
	Item item;
	wcscpy(item.name, L"지식의 목걸이");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_ACCESSORY;
	item.stat = STAT_WIS;
	item.value = 5;
	item.cost = 15;
	return item;
}

// 그외
Item HpPotion(void) {
	Item item;
	wcscpy(item.name, L"체력 포션");
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_HP;
	item.value = 5;
	item.cost = 5;
	return item;
}
Item StrPotion() {
	Item item;
	wcscpy(item.name, L"힘의 비약");
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_STR;
	item.value = 1;
	item.cost = 20;
	return item;
}
Item DexPotion() {
	Item item;
	wcscpy(item.name, L"암살자의 성장약");
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_DEX;
	item.value = 1;
	item.cost = 20;
	return item;
}
Item WisPotion() {
	Item item;
	wcscpy(item.name, L"지식의 샘물");
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_WIS;
	item.value = 1;
	item.cost = 20;
	return item;
}
Item LukPotion() {
	Item item;
	wcscpy(item.name, L"운이 좋은 포션");
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_LUK;
	item.value = 1;
	item.cost = 30;
	return item;
}
