#define _CRT_SECURE_NO_WARNINGS
#include"Skill.h"
#include"Player.h"
#include"Monster.h"

SkillSlot g_SkillTable[ALL_SKILL_COUNT] = {
    {SKILL_STR_DAMAGE,   2, 0, 2},
    {SKILL_DEX_DAMAGE,   2, 0, 2},
    {SKILL_WIS_DAMAGE,   2, 0, 2},
    {SKILL_HEAL_DEX,     1, 0, 5},
    {SKILL_HEAL_SPELL,   1, 0, 5},
    {SKILL_THROUGH_DAMAGE,   5, 0, 1}
};

const wchar_t* GetSkillName(SkillType id) {
    switch (id) {
    case SKILL_STR_DAMAGE:      return L"강타";
    case SKILL_DEX_DAMAGE:      return L"암살";
    case SKILL_WIS_DAMAGE:      return L"마력 강화";
    case SKILL_HEAL_DEX:        return L"응급처치";
    case SKILL_HEAL_SPELL:      return L"치유마법";
    case SKILL_THROUGH_DAMAGE:  return L"관통 공격";
    default:                    return L"-";
    }
}
void Skill_Effect(Using_Player* p,Monster* m, SkillType id) {
    SkillSlot* s = &g_SkillTable[id];

    int Damage, Heal;

    switch (id) {
    case SKILL_STR_DAMAGE:
        m->hp -= (p->stat.str * s->value) - m->def; break;
    case SKILL_DEX_DAMAGE:
        m->hp -= (p->stat.dex * s->value) - m->def; break;
    case SKILL_WIS_DAMAGE:
        m->hp -= (p->stat.wis * s->value) - m->def; break;
    case SKILL_THROUGH_DAMAGE:
        m->hp -= p->stat.str; break;

    case SKILL_HEAL_DEX:
        p->stat.hp += p->stat.dex; break;
    case SKILL_HEAL_SPELL:
        p->stat.hp += p->stat.wis; break;
    }

}
