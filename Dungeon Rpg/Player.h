#ifndef PLAYER_H
#define PLAYER_H
#define SKILL_SLOT_MAX 4

#include "item.h"

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
    PSTAT_STR,
    PSTAT_DEX,
    PSTAT_WIS,
    PSTAT_LUK
} PlayerStatType;

typedef struct {
    int SkillId;
}SkillSlot;

typedef struct {
    Player_Stats stat;
    int Money;
    Inventory inv;
    SkillSlot skills[SKILL_SLOT_MAX];
}Using_Player;


#endif // PLAYER_H
