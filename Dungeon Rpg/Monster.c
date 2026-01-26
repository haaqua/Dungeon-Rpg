#include"Monster.h"
#include<wchar.h>
#include<stdio.h>

Monster Slime(int Level) {
	Monster m;
	wcspy(m.name, L"슬라임");
	m.type = common;
	m.lv = Level;
	m.hp = 7 + Level;
	m.atk = 1 + Level;
	m.def = 0 + Level;

	m.skill_count = 0;
	m.skill[0] = SlimeSkill;

	return m;
}
Monster Goblin(int Level) {
	Monster m;
	wcspy(m.name, L"고블린");
	m.type = common;
	m.lv = Level;
	m.hp = 5 + Level;
	m.atk = 5 + Level;
	m.def = 3 + Level;

	m.skill_count = 0;
	m.skill[0] = GoblinSkill;

	return m;
}
Monster Imp(int Level) {
	Monster m;
	wcspy(m.name, L"임프");
	m.type = common;
	m.lv = Level;
	m.hp = 3 + Level;
	m.atk = 5 + Level;
	m.def = 1 + Level;

	m.skill_count = 0;
	m.skill[0] = ImpSkill;

	return m;
}
Monster Orc(int Level) {
	Monster m;
	wcspy(m.name, L"오크");
	m.type = rare;
	m.lv = Level;
	m.hp = 10 + Level*2;
	m.atk = 5 + Level*2;
	m.def = 5 + Level*2;

	m.skill_count = 0;
	m.skill[0] = OrcSmash;
	m.skill[1] = OrcRush;

	return m;
}
Monster Vampire(int Level) {
	Monster m;
	wcspy(m.name, L"뱀파이어");
	m.type = rare;
	m.lv = Level;
	m.hp = 5 * Level;
	m.atk = 3 * Level;
	m.def = 3 * Level;

	m.skill_count = 0;
	m.skill[0] = VampireBloodAttack;
	m.skill[1] = VampireBlooddrain;

	return m;
}
Monster Dragon(int Level) {
	Monster m;
	wcspy(m.name, L"드래곤");
	m.type = Boss;
	m.lv = Level;
	m.hp = 10 * Level * 2;
	m.atk = 5 * Level * 2;
	m.def = 5 * Level * 2;

	m.skill_count = 0;
	m.skill[0] = DragonBreath;
	m.skill[1] = DragonArmor;

	return m;
}

void SlimeSkill(Monster* self) {
	int heal = self->lv;
	self->hp += heal;
	wprintf(L"%ls이 몸을 분열하여 체력을 %d 회복했다!\n", self->name, heal);
}
void GoblinSkill(Monster* self) {
	int swap = self->atk;
	self->atk = self->def;
	self->def = swap;
	wprintf(L"%ls이 장비를 바꿔쓴다!\n", self->name);
}
void ImpSkill(Monster* self) {
	int bonus = self->atk + self->lv;
	wprintf(L"%ls의 기습 공격! 추가 피해 %d\n", self->name, bonus);
}

void OrcSmash(Monster* self) {
	int bonus = self->atk + self->lv * 2;
	wprintf(L"%ls의 강타! 추가 피해 %d\n", self->name, bonus);
}
void OrcRush(Monster* self) {
	int bonus = self->def + self->lv * 2;
	wprintf(L"%ls가 방패를 들고 돌진! 추가 피해 %d\n", self->name, bonus);
}
void VampireBlooddrain(Monster* self) {
	int drain = self->atk / 2;
	self->hp += drain;
	wprintf(L"%ls가 흡혈했다! HP +%d\n", self->name, drain);
}	 
void VampireBloodAttack(Monster* self) {
	int attack = self->hp / 2;
	self->hp -= attack;
	wprintf(L"%ls가 피로 공격했다! 추가 피해 %d\n", self->name, attack);
}

void DragonBreath(Monster* self) {
	int bonus = self->atk * self->lv;
	wprintf(L"%ls의 브래스! 추가 피해 %d\n", self->name, bonus);
}
void DragonArmor(Monster* self) {
	int bonus = self->def * self->lv;
	wprintf(L"&ls이 자신의 비늘로 몸을 감싼다!\n", self->name, bonus);
}
void DragonMagic(Monster* self) {
	int bonus = self->hp * self->lv;
	wprintf(L"&ls이 용의 마법으로 자신의 몸을 회복한다!\n", self->name, bonus);
}