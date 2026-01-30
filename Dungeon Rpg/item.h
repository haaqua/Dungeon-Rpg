#ifndef ITEM_H
#define ITEM_H

#include <wchar.h>

void ApplyItemStat(Player_Stats* stat, Item* item, int sign);
void EquipWeapon(Using_Player* p, int invIdx);
void EquipArmor(Using_Player* p, int invIdx);
void EquipAcce(Using_Player* p, int invIdx);
int GetAttckStat(Using_Player* p);
int GetDefense(Using_Player* p);

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

/* 아이템 생성 함수들 */
// 무기
Item Sword(void);
Item Axe();
Item Saint_Saver();

Item Dagger();
Item CrossBow();
Item Death_Reaper();

Item Wand();
Item Artifact();
Item Gambanteinn();

// 방어구
Item Armor();
Item Theif_Robe();
Item Magic_Robe();

// 장신구
Item Ring_Str();
Item Ring_Dex();
Item Ring_Wis();

// 그외
Item HpPotion(void);
Item StrPotion();
Item DexPotion();
Item WisPotion();
Item LukPotion();
#endif
