#ifndef MONSTER_H
#define MONSTER_H

#include<wchar.h>

typedef enum {
	common,
	rare,
	Boss
}mon_type;

struct Monster;

typedef const wchar_t* (*Monster_Skill)(struct Monster* self);

typedef struct {
	wchar_t name[20];
	mon_type type;
	int hp;
	int lv;
	int atk;
	int def;

	Monster_Skill skill[3];
	int skill_count;
}Monster;

typedef struct {
	const wchar_t* msg;
	int damage;     
	int self_heal;   
} SkillResult;


Monster Slime(int Level);
Monster Goblin(int Level);
Monster Imp(int Level);
Monster Orc(int Level);
Monster Vampire(int Level);
Monster Dragon(int Level);
#endif 
