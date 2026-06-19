#include "interactionManager.hpp"
#include "types.hpp"
#include <raylib.h>

bool InteractionManager::ShouldHighlight(Pile *selected, Pile *target) {
	if (selected == nullptr || selected == target) {
		return false;
	}
	if (selected->IsEmpty()) {
		return false;
	}

	CardType selType = selected->Back().type;

	if (target->IsEmpty()) {
		if (target->isLeftHand || target->isRightHand || target->isBackpack) {
			return (selType == CardType::WEAPON || selType == CardType::WAND ||
					selType == CardType::POTION ||
					selType == CardType::SHIELD || selType == CardType::SPELL);
		}
		return false;
	}

	CardType tarType = target->Back().type;

	if (selected->isBackpack) {
		return (target->isRightHand || target->isLeftHand);
	}

	if (selType == CardType::ENEMY) {
		return (tarType == CardType::PLAYER || tarType == CardType::SHIELD);
	}

	if ((selType == CardType::WEAPON && !selected->isBackpack) ||
		selType == CardType::WAND) {
		return (tarType == CardType::ENEMY);
	}

	if (selType == CardType::POTION) {
		return (tarType == CardType::PLAYER);
	}

	if (selType == CardType::COIN) {
		return (tarType == CardType::PLAYER);
	}

	Element selELement = selected->Back().element;

	if (selType == CardType::SPELL) {
		return (tarType == CardType::ENEMY);
	}
	return false;
}

void InteractionManager::Handle(Pile *&selected, Pile *target, int &score,
								Pile *playerPile, Pile *dungeonPiles,
								std::vector<Card> &masterDeck) {
	if (selected == nullptr) {
		if (!target->cards.empty() && !target->isDiscardPile) {
			selected = target;
		}
		return;
	}

	if (selected == target) {
		selected = nullptr;
		return;
	}

	if (selected->isDungeonPile && target->isDungeonPile) {
		selected = nullptr;
		return;
	}

	if (target->isDiscardPile) {
		ResolveCardVsDiscardPile(selected, target, score);
		selected = nullptr;
		return;
	}

	if (target->cards.empty()) {
		if (target->isDungeonPile) {
			selected = nullptr;
			return;
		}

		if (selected->cards.back().type == CardType::ENEMY) {
			selected = nullptr;
			return;
		}

		HandleEmptyTargetMove(selected, target);
		selected = nullptr;
		return;
	}

	ResolveCardInteraction(selected, target, score, playerPile, masterDeck,
						   dungeonPiles);
	selected = nullptr;
}

void InteractionManager::HandleEmptyTargetMove(Pile *selected, Pile *target) {
	Card &sel = selected->cards.back();

	if (sel.type == CardType::PLAYER) {
		selected = nullptr;
		return;
	}

	target->cards.push_back(selected->cards.back());
	selected->cards.pop_back();
}

void InteractionManager::ResolveCardInteraction(Pile *selected, Pile *target,
												int &score, Pile *playerPile,
												std::vector<Card> &masterDeck,
												Pile *dungeonPiles) {
	Card &sel = selected->cards.back();
	Card &tar = target->cards.back();

	if (sel.type == CardType::WEAPON && tar.type == CardType::ENEMY) {
		ResolveWeaponVsEnemy(selected, target, sel, tar, score, playerPile,
							 masterDeck);
	} else if (sel.type == CardType::ENEMY && tar.type == CardType::PLAYER) {
		ResolvePlayerVsEnemy(selected, tar, sel, score);
	} else if (sel.type == CardType::ENEMY && tar.type == CardType::SHIELD) {
		ResolveEnemyVsShield(selected, target, sel, tar, score, playerPile);
	} else if (sel.type == CardType::POTION && tar.type == CardType::PLAYER) {
		ResolvePotionVsPlayer(selected, target, sel, tar, score);
	} else if (sel.type == CardType::WAND && tar.type == CardType::ENEMY) {
		ResolveWandVsEnemy(selected, target, sel, tar, score);
	} else if (sel.type == CardType::COIN && tar.type == CardType::PLAYER) {
		ResolveCoinVsPlayer(selected, target, sel, tar, score);
	} else if (sel.type == CardType::SPELL && tar.type == CardType::ENEMY) {
		ResolveSpellVsEnemy(selected, target, sel, tar, score, playerPile,
							masterDeck, dungeonPiles);
	} else {
	}
}

void InteractionManager::ResolveCardVsDiscardPile(Pile *selected, Pile *target,
												  int &score) {
	Card &topCard = selected->Back();

	if (selected->Back().type == CardType::PLAYER ||
		selected->Back().type == CardType::ENEMY) {
		return;
	}
	score += selected->Back().value;
	selected->cards.pop_back();
}

void InteractionManager::ResolveWeaponVsEnemy(Pile *selected, Pile *target,
											  Card &sel, Card &tar, int &score,
											  Pile *playerPile,
											  std::vector<Card> &masterDeck) {
	if (selected->isBackpack)
		return;

	if (sel.value >= tar.value) {
		score += tar.value;

		sel.value -= tar.value;

		target->cards.pop_back();

		if (sel.value <= 0) {
			selected->cards.pop_back();
		}
	} else {
		tar.value -= sel.value;
		selected->cards.pop_back();
	}
}

void InteractionManager::ResolvePlayerVsEnemy(Pile *target, Card &sel,
											  Card &tar, int &score) {
	score += tar.value;

	sel.value -= tar.value;
	target->cards.pop_back();
}

void InteractionManager::ResolveEnemyVsShield(Pile *selected, Pile *target,
											  Card &sel, Card &tar, int &score,
											  Pile *playerPile) {
	if (tar.value >= sel.value) {
		tar.value -= sel.value;
		selected->cards.pop_back();
		if (tar.value <= 0) {
			target->cards.pop_back();
		}
	} else if (sel.value > tar.value) {
		int remainingDmg = sel.value - tar.value;
		target->cards.pop_back();

		if ((playerPile != nullptr) && !playerPile->IsEmpty()) {
			playerPile->Back().value -= remainingDmg;
		}

		selected->cards.pop_back();
	}

	score += sel.value;
}

void InteractionManager::ResolvePotionVsPlayer(Pile *selected, Pile *target,
											   Card &sel, Card &tar,
											   int &score) {
	tar.IncreaseVal(sel.value);
	selected->cards.pop_back();
}

void InteractionManager::ResolveWandVsEnemy(Pile *selected, Pile *target,
											Card &sel, Card &tar, int &score) {
	if (sel.element == tar.element) {
		return;
	}

	int dmgMultiplier = 1;

	if (((sel.element == Element::ICE) && (tar.element == Element::FIRE)) ||
		((sel.element == Element::FIRE) && (tar.element == Element::ICE))) {

		dmgMultiplier = 2;
	}

	int totalDamage = dmgMultiplier * sel.value;

	if (totalDamage >= tar.value) {
		score += tar.value;
		target->cards.pop_back();
	} else {
		tar.value -= totalDamage;
	}

	sel.value -= 1;

	if (sel.value <= 0) {
		selected->cards.pop_back();
	}
}

void InteractionManager::ResolveCoinVsPlayer(Pile *selected, Pile *target,
											 Card &sel, Card &tar, int &score) {
	score += sel.value;
	selected->cards.pop_back();
}

void InteractionManager::ResolveSpellVsEnemy(Pile *selected, Pile *target,
											 Card &sel, Card &tar, int &score,
											 Pile *playerPile,
											 std::vector<Card> &masterDeck,
											 Pile *dungeonPiles) {
	switch (sel.element) {
	case Element::LIFESTEAL: {
		int dmgDealt = 0;
		if (sel.value >= tar.value) {
			dmgDealt = tar.value;
			score += tar.value;

			sel.value -= tar.value;

			target->cards.pop_back();

			if (sel.value <= 0) {
				selected->cards.pop_back();
			}
		} else {
			dmgDealt = sel.value;
			tar.value -= sel.value;
			selected->cards.pop_back();
		}

		if (playerPile != nullptr && !playerPile->IsEmpty()) {
			playerPile->Back().IncreaseVal(dmgDealt);
		}
		break;
	}
	case Element::WARHAMMER: {
		masterDeck.insert(masterDeck.begin(), tar);

		TraceLog(LOG_INFO, "KARTA WYSLANA NA TYL DECKU");
		target->cards.pop_back();

		sel.value -= 1;
		if (sel.value <= 0) {
			selected->cards.pop_back();
		}
		return;
	}
	case Element::ESCAPE: {
		TraceLog(LOG_INFO, "ESCAPE USED RESET");

		if (selected != nullptr && !selected->IsEmpty()) {
			selected->cards.pop_back();
		}
		for (int i = 0; i < D_COUNT; i++) {
			while (!dungeonPiles[i].IsEmpty()) {
				masterDeck.insert(masterDeck.begin(), dungeonPiles[i].Back());
				dungeonPiles[i].cards.pop_back();
			}
		}

		for (int i = 0; i < D_COUNT; i++) {
			if (!masterDeck.empty()) {
				dungeonPiles[i].cards.push_back(masterDeck.back());
				masterDeck.pop_back();
			}
		}
		return;
	}
	}
}
