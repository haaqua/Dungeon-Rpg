#ifndef MARKET_H
#define MARKET_H
#include<wchar.h>
#include"item.h"
#include"Skill.h"

typedef struct {
	Item item[2];
	SkillType skill;
	int HasSkill;
}StorySell;

StorySell currentsell;


#endif MARKET_H