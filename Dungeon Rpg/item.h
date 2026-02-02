#ifndef ITEM_H
#define ITEM_H

#include "Type.h"
#include <wchar.h>


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
