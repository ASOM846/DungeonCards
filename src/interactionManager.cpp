#include "interactionManager.hpp"
#include "types.hpp"

void InteractionManager::Handle(Pile *&selected, Pile *target, int &score) {
	if (selected == nullptr) {
		if (!target->cards.empty()) {
			selected = target;
		}
		return;
	}

	if (selected == target) {
		selected = nullptr;
		return;
	}

	if (target->cards.empty()) {
		HandleEmptyTargetMove(selected, target);
		selected = nullptr;
		return;
	}

	ResolveCardInteraction(selected, target, score);
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
												int &score) {
	Card &sel = selected->cards.back();
	Card &tar = target->cards.back();

	if (sel.type == CardType::WEAPON && tar.type == CardType::ENEMY) {
		ResolveWeaponVsEnemy(selected, target, sel, tar, score);
	} else if (sel.type == CardType::PLAYER && tar.type == CardType::ENEMY) {
		ResolvePlayerVsEnemy(target, sel, tar, score);
	} else if (sel.type == CardType::POTION && tar.type == CardType::PLAYER) {
		ResolvePotionVsPlayer(selected, target, sel, tar, score);
	} else if (sel.type == CardType::WAND && tar.type == CardType::ENEMY) {
		ResolveWandVsEnemy(selected, target, sel, tar, score);
	} else {
	}
}

void InteractionManager::ResolveWeaponVsEnemy(Pile *selected, Pile *target,
											  Card &sel, Card &tar,
											  int &score) {
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

void InteractionManager::ResolvePotionVsPlayer(Pile *selected, Pile *target,
											   Card &sel, Card &tar,
											   int &score) {
	tar.value += sel.value;
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
