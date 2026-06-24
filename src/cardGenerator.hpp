#pragma once

#include "types.hpp"

class CardGenerator {
  public:
	static Card GetRandomWeapon();
	static Card GetRandomSpell();
	static Card GetRandomEnemy(Element enemyElement = Element::NONE);
	static Card GetItemParms(const CardType type,
							 const Element element = Element::NONE);
};
