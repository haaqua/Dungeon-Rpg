#define _CRT_SECURE_NO_WARNINGS
#include"Market.h"

void SetUpStore() {
	for (int i = 0; i < 2; i++) {
		int randPotion = rand() % 5;
		if (randPotion == 0) {
			currentsell.item[i] = HpPotion();
		}
		else if (randPotion == 1) {
			currentsell.item[i] = StrPotion();
		}
		else if (randPotion == 2) {
			currentsell.item[i] = DexPotion();
		}
		else if (randPotion == 3) {
			currentsell.item[i] = WisPotion();
		}
		else if (randPotion == 4) {
			currentsell.item[i] = LukPotion();
		}
	}
	currentsell.skill = (SkillType)(rand() % ALL_SKILL_COUNT);
	currentsell.HasSkill = 1;
}
