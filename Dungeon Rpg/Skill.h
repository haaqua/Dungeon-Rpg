#ifndef SKILL_H
#define SKILL_H
#include<wchar.h>
#define ALL_SKILL_COUNT 6

typedef struct {
    int SkillId;
    int cooldown;
    int curCool;
    int value;
}SkillSlot;
typedef enum {
    SKILL_NONE = -1,
    SKILL_STR_DAMAGE,
    SKILL_DEX_DAMAGE,
    SKILL_WIS_DAMAGE,
    SKILL_HEAL_DEX,
    SKILL_HEAL_SPELL,
    SKILL_THROUGH_DAMAGE
}SkillType;


extern SkillSlot g_SkillTable[ALL_SKILL_COUNT];

const wchar_t* GetSkillName(SkillType id);

#endif 