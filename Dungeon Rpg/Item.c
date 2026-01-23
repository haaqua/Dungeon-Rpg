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
	return item;
}
Item Axe() {
	Item item;
	wcscpy(item.name, L"도끼");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_STR;
	item.value = 5;
	return item;
}
Item Dagger() {
	Item item;
	wcscpy(item.name, L"단검");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_DEF;
	item.value = 3;
	return item;
}
Item CrossBow() {
	Item item;
	wcscpy(item.name, L"석궁");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_DEF;
	item.value = 5;
	return item;
}
Item Wand() {
	Item item;
	wcscpy(item.name, L"지팡이");
	item.category = ITEM_EQUIP;
	item.slot = SLOT_WEAPON;
	item.stat = STAT_WIS;
	item.value = 3;
	return item;
}

// 

// 그외
Item MakePotion(void) {
	Item item;
	wcscpy(item.name, L"포션");
	item.category = ITEM_CONSUME;
	item.slot = SLOT_NONE;
	item.stat = STAT_HP;
	item.value = 5;
	return item;
}
