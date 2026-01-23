#ifndef ITEM_H
#define ITEM_H

#include <wchar.h>

typedef enum {
    ITEM_EQUIP,
    ITEM_CONSUME,
    ITEM_MISC
} ItemCategory;

typedef enum {
    SLOT_WEAPON,
    SLOT_ARMOR,
    SLOT_ACCESSORY,
    SLOT_NONE
} EquipSlot;

typedef enum {
    STAT_HP,
    STAT_STR,
    STAT_DEF,
    STAT_WIS,
} StatType;

#define ITEM_NAME_LEN 32

typedef struct {
    wchar_t name[ITEM_NAME_LEN];
    ItemCategory category;
    EquipSlot slot;
    StatType stat;
    int value;
} Item;

/* 아이템 생성 함수들 */
// 무기
Item Sword(void);
Item Axe();
Item Dagger();
Item CrossBow();
Item Wand();

// 그외
Item MakePotion(void);

#endif
