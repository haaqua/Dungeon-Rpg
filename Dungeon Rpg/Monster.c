#define _CRT_SECURE_NO_WARNINGS
#include"Monster.h"
#include<wchar.h>
#include<stdio.h>

const wchar_t* SlimeSkill(Monster* self);
const wchar_t* GoblinSkill(Monster* self);
const wchar_t* ImpSkill(Monster* self);

const wchar_t* OrcSmash(Monster* self);
const wchar_t* OrcRush(Monster* self);

const wchar_t* VampireBloodAttack(Monster* self);
const wchar_t* VampireBlooddrain(Monster* self);

const wchar_t* DragonBreath(Monster* self);
const wchar_t* DragonArmor(Monster* self);
const wchar_t* DragonMagic(Monster* self);


Monster Slime(int Level) {
	Monster m;
	wcscpy(m.name, L"슬라임");
	m.type = common;
	m.lv = Level;
	m.hp = 7 + Level;
	m.atk = 1 + Level;
	m.def = 0 + Level;

	m.skill_count = 1;
	m.skill[0] = SlimeSkill;

	return m;
}
Monster Goblin(int Level) {
	Monster m;
	wcscpy(m.name, L"고블린");
	m.type = common;
	m.lv = Level;
	m.hp = 5 + Level;
	m.atk = 5 + Level;
	m.def = 3 + Level;

	m.skill_count = 1;
	m.skill[0] = GoblinSkill;

	return m;
}
Monster Imp(int Level) {
	Monster m;
	wcscpy(m.name, L"임프");
	m.type = common;
	m.lv = Level;
	m.hp = 3 + Level;
	m.atk = 5 + Level;
	m.def = 1 + Level;

	m.skill_count = 1;
	m.skill[0] = ImpSkill;

	return m;
}
Monster Orc(int Level) {
	Monster m;
	wcscpy(m.name, L"오크");
	m.type = rare;
	m.lv = Level;
	m.hp = 10 + Level*2;
	m.atk = 5 + Level*2;
	m.def = 5 + Level*2;

	m.skill_count = 2;
	m.skill[0] = OrcSmash;
	m.skill[1] = OrcRush;

	return m;
}
Monster Vampire(int Level) {
	Monster m;
	wcscpy(m.name, L"뱀파이어");
	m.type = rare;
	m.lv = Level;
	m.hp = 5 * Level;
	m.atk = 3 * Level;
	m.def = 3 * Level;

	m.skill_count = 2;
	m.skill[0] = VampireBloodAttack;
	m.skill[1] = VampireBlooddrain;

	return m;
}
Monster Dragon(int Level) {
	Monster m;
	wcscpy(m.name, L"드래곤");
	m.type = Boss;
	m.lv = Level;
	m.hp = 10 * Level * 2;
	m.atk = 5 * Level * 2;
	m.def = 5 * Level * 2;

	m.skill_count = 3;
	m.skill[0] = DragonBreath;
	m.skill[1] = DragonArmor;
	m.skill[2] = DragonMagic;

	return m;
}

const wchar_t* SlimeSkill(Monster* self) {
	return L"슬라임이 몸을 분열하여 체력을 회복했다!"; /// 체력 회복
}

const wchar_t* GoblinSkill(Monster* self) {
	return L"고블린이 장비를 바꿔 들었다!공격력과 방어력이 바뀐다 !"; // 공격력, 방어력 바꾸기
}

const wchar_t* ImpSkill(Monster* self) {
	return L"임프의 기습 공격!"; // 공격력 * 2
}

const wchar_t* OrcSmash(Monster* self) {
	return L"오크의 강타!"; // 공격력 + 레밸*2
}

const wchar_t* OrcRush(Monster* self) {
	return L"오크가 방패를 들고 돌진했다!"; // 방어력 만큼의 데미지
}

const wchar_t* VampireBlooddrain(Monster* self) {
	return L"뱀파이어가 흡혈하여 체력을 회복했다!"; // 상대에게 데미지, 준 데미지만큼 회복
}

const wchar_t* VampireBloodAttack(Monster* self) {
	return L"뱀파이어가 자신의 피로 공격했다!";// hp 줄이고 줄인 만큼 상대에게 데미지를 줌
}

const wchar_t* DragonBreath(Monster* self) {
	return L"드래곤이 강력한 브레스를 뿜었다!"; // 데미지 * Lv * 2
}

const wchar_t* DragonArmor(Monster* self) {
	return L"드래곤이 비늘로 몸을 감싸 방어력을 높였다!"; // 방어력 * Lv * 2
}

const wchar_t* DragonMagic(Monster* self) {
	return L"드래곤이 용의 마법으로 체력을 회복했다!"; // hp += Lv
}
