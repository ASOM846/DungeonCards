#include "interactionManager.hpp"
#include "screenShake.hpp"
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

	if (selected->isDungeonPile && target->isDungeonPile)
		return false;

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
								std::vector<Card> &masterDeck,
								int &cardsDefeated, ScreenShake &screenShake) {
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
		ResolveCardVsDiscardPile(selected, target, score, cardsDefeated);
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
						   dungeonPiles, cardsDefeated, screenShake);
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
												Pile *dungeonPiles,
												int &cardsDefeated,
												ScreenShake &screenShake) {
	Card &sel = selected->cards.back();
	Card &tar = target->cards.back();

	if (sel.type == CardType::WEAPON && tar.type == CardType::ENEMY) {
		ResolveWeaponVsEnemy(selected, target, sel, tar, score, playerPile,
							 masterDeck, cardsDefeated);
	} else if (sel.type == CardType::ENEMY && tar.type == CardType::PLAYER) {
		ResolvePlayerVsEnemy(selected, tar, sel, score, cardsDefeated);
	} else if (sel.type == CardType::ENEMY && tar.type == CardType::SHIELD) {
		ResolveEnemyVsShield(selected, target, sel, tar, score, playerPile,
							 cardsDefeated);
	} else if (sel.type == CardType::POTION && tar.type == CardType::PLAYER) {
		ResolvePotionVsPlayer(selected, target, sel, tar, score, cardsDefeated);
	} else if (sel.type == CardType::WAND && tar.type == CardType::ENEMY) {
		ResolveWandVsEnemy(selected, target, sel, tar, score, cardsDefeated);
	} else if (sel.type == CardType::COIN && tar.type == CardType::PLAYER) {
		ResolveCoinVsPlayer(selected, target, sel, tar, score, cardsDefeated);
	} else if (sel.type == CardType::SPELL && tar.type == CardType::ENEMY) {
		ResolveSpellVsEnemy(selected, target, sel, tar, score, playerPile,
							masterDeck, dungeonPiles, cardsDefeated);
	} else {
	}

	if (sel.type == CardType::ENEMY && tar.type == CardType::PLAYER) {
		screenShake.trigger(2.0f, 0.2f);
	}

	if (sel.type == CardType::WEAPON && tar.type == CardType::ENEMY) {
		screenShake.trigger(2.0f, 0.2f);
	}
}

void InteractionManager::ResolveCardVsDiscardPile(Pile *selected, Pile *target,
												  int &score,
												  int &cardsDefeated) {
	Card &topCard = selected->Back();

	if (selected->Back().type == CardType::PLAYER ||
		selected->Back().type == CardType::ENEMY) {
		return;
	}
	score += selected->Back().value;
	selected->cards.pop_back();
	cardsDefeated++;
}

void InteractionManager::ResolveWeaponVsEnemy(Pile *selected, Pile *target,
											  Card &sel, Card &tar, int &score,
											  Pile *playerPile,
											  std::vector<Card> &masterDeck,
											  int &cardsDefeated) {

	if (selected->isBackpack)
		return;

	if (sel.value >= tar.value) {
		score += tar.value;

		sel.value -= tar.value;

		target->cards.pop_back();
		cardsDefeated++;

		if (sel.value <= 0) {
			selected->cards.pop_back();
		}
	} else {
		tar.value -= sel.value;
		selected->cards.pop_back();
		cardsDefeated++;
	}
}

void InteractionManager::ResolvePlayerVsEnemy(Pile *target, Card &sel,
											  Card &tar, int &score,
											  int &cardsDefeated) {
	score += tar.value;

	sel.value -= tar.value;
	target->cards.pop_back();
	cardsDefeated++;
}

void InteractionManager::ResolveEnemyVsShield(Pile *selected, Pile *target,
											  Card &sel, Card &tar, int &score,
											  Pile *playerPile,
											  int &cardsDefeated) {
	if (tar.value >= sel.value) {
		tar.value -= sel.value;
		selected->cards.pop_back();
		cardsDefeated++;
		if (tar.value <= 0) {
			target->cards.pop_back();
			cardsDefeated++;
		}
	} else if (sel.value > tar.value) {
		int remainingDmg = sel.value - tar.value;
		target->cards.pop_back();

		if ((playerPile != nullptr) && !playerPile->IsEmpty()) {
			playerPile->Back().value -= remainingDmg;
		}

		selected->cards.pop_back();
		cardsDefeated++;
	}
	score += sel.value;
}

void InteractionManager::ResolvePotionVsPlayer(Pile *selected, Pile *target,
											   Card &sel, Card &tar, int &score,
											   int &cardsDefeated) {
	tar.IncreaseVal(sel.value);
	selected->cards.pop_back();
	cardsDefeated++;
}

void InteractionManager::ResolveWandVsEnemy(Pile *selected, Pile *target,
											Card &sel, Card &tar, int &score,
											int &cardsDefeated) {
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
		cardsDefeated++;
	} else {
		tar.value -= totalDamage;
		cardsDefeated++;
	}

	sel.value -= 1;

	if (sel.value <= 0) {
		selected->cards.pop_back();
		cardsDefeated++;
	}
}

void InteractionManager::ResolveCoinVsPlayer(Pile *selected, Pile *target,
											 Card &sel, Card &tar, int &score,
											 int &cardsDefeated) {
	score += sel.value;
	selected->cards.pop_back();
	cardsDefeated++;
}

void InteractionManager::ResolveSpellVsEnemy(Pile *selected, Pile *target,
											 Card &sel, Card &tar, int &score,
											 Pile *playerPile,
											 std::vector<Card> &masterDeck,
											 Pile *dungeonPiles,
											 int &cardsDefeated) {
	switch (sel.element) {
	case Element::LIFESTEAL: {
		int dmgDealt = 0;
		if (sel.value >= tar.value) {
			dmgDealt = tar.value;
			score += tar.value;

			sel.value -= tar.value;

			target->cards.pop_back();
			cardsDefeated++;

			if (sel.value <= 0) {
				selected->cards.pop_back();
				cardsDefeated++;
			}
		} else {
			dmgDealt = sel.value;
			tar.value -= sel.value;
			selected->cards.pop_back();
			cardsDefeated++;
		}

		if (playerPile != nullptr && !playerPile->IsEmpty()) {
			playerPile->Back().IncreaseVal(dmgDealt);
		}
		break;
	}
	case Element::WARHAMMER: {
		masterDeck.insert(masterDeck.begin(), tar);

		target->cards.pop_back();

		sel.value -= 1;
		if (sel.value <= 0) {
			selected->cards.pop_back();
		}
		return;
	}
	case Element::ESCAPE: {

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
