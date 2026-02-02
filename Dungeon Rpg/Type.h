#ifndef TYPES_H
#define TYPES_H

#include<wchar.h>

// 플레이어 스탯
typedef struct {
    int hp;
    int level;
    int str;
    int dex;
    int wis;
    int luk;
    int def;
} Player_Stats;

typedef enum {
    ITEM_EQUIP,
    ITEM_CONSUME,
    ITEM_MISC
} ItemCategory;

typedef enum {
    SLOT_WEAPON,
    SLOT_ARMOR,
    SLOT_ACCESSORY,
} EquipSlot;

typedef enum {
    STAT_HP,
    STAT_STR,
    STAT_DEX,
    STAT_WIS,
    STAT_LUK
} ITEMStat;

typedef enum {
    ITEM_WEAPON,
    ITEM_ARMOR,
    ITEM_ACCESSORY,
    ITEM_NONE
}ItemType;

#define ITEM_NAME_LEN 32

typedef struct {
    wchar_t name[ITEM_NAME_LEN];
    ItemType type;
    ItemCategory category;
    EquipSlot slot;
    ITEMStat stat;
    int value;
    int cost;

    int usable;
} Item;

// 인벤토리
#define Inventory_Size 20
typedef struct {
    Item item[Inventory_Size];
    int count;

    int WeaponIdx;
    int ArmorIdx;
    int AcceIdx;
}Inventory;

#endif