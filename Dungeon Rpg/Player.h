#ifndef PLAYER_H
#define PLAYER_H
#define SKILL_SLOT_MAX 3

#include "Type.h"
#include"Skill.h"

typedef enum {
    PSTAT_STR,
    PSTAT_DEX,
    PSTAT_WIS,
    PSTAT_LUK
} PlayerStatType;

typedef struct {
    Player_Stats stat;
    int Money;
    Inventory inv;
    SkillSlot skills[SKILL_SLOT_MAX];
}Using_Player;

// 함수 선언
void ApplyItemStat(Player_Stats* stat, Item* item, int sign);
void EquipWeapon(Using_Player* p, int invIdx);
void EquipArmor(Using_Player* p, int invIdx);
void EquipAcce(Using_Player* p, int invIdx);
int GetAttckStat(Using_Player* p);
int GetDefense(Using_Player* p);
#endif // PLAYER_H
