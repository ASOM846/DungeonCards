#include "cardGenerator.hpp"
#include "textureManager.hpp"
#include "types.hpp"
#include <cstdlib>
#include <random>
#include <raylib.h>
#include <vector>

struct WeaponTemplate {
	TextureId textureId;
	const char *name;
	int minValue;
	int maxValue;
	int maxDurability;
	int weight;
};

Card CardGenerator::GetRandomWeapon() {
	static std::vector<WeaponTemplate> pool = {
		{TextureId::WeaponRustySword, "RUSTY SWORD", 1, 3, 2, 50},
		{TextureId::WeaponSword, "SWORD", 2, 6, 3, 30},
		{TextureId::WeaponAxe, "AXE", 4, 8, 2, 15},
		{TextureId::WeaponDoubleAxe, "DOUBLE AXE", 6, 10, 2, 10},
		{TextureId::WeaponHammer, "HAMMER", 4, 10, 3, 10},
		{TextureId::WeaponMace, "MACE", 10, 12, 1, 5}};

	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::vector<int> weights;
	weights.reserve(pool.size());
	for (const auto &weapon : pool) {
		weights.push_back(weapon.weight);
	}

	std::discrete_distribution<size_t> dist(weights.begin(), weights.end());

	size_t randomIndex = dist(gen);
	const auto &templateWeapon = pool[randomIndex];

	Card card;
	card.type = CardType::WEAPON;
	card.textureId = templateWeapon.textureId;
	card.name = templateWeapon.name;

	card.minValue = templateWeapon.minValue;
	card.maxValue = templateWeapon.maxValue;

	card.durability = templateWeapon.maxDurability;
	card.maxDurability = templateWeapon.maxDurability;

	card.hp = -1;

	card.description = TextFormat("%s - STANDARD WEAPON", card.name.c_str());

	return card;
}

struct EnemyTemplate {
	TextureId textureId;
	const char *name;
	int hp;
};

Card CardGenerator::GetRandomEnemy(Element enemyElement) {
	EnemyTemplate templateEnemy;

	static const std::vector<TextureId> textures = {
		TextureId::Enemy1, TextureId::Enemy2, TextureId::Enemy3,
		TextureId::Enemy4, TextureId::Enemy5, TextureId::Enemy6,
		TextureId::Enemy7, TextureId::Enemy8,
	};

	int seed = GetRandomValue(0, textures.size() - 1);

	templateEnemy.textureId = textures[seed];

	templateEnemy.hp = GetRandomValue(2, 10);
	templateEnemy.name = "ENEMY";

	Card card;
	card.type = CardType::ENEMY;
	card.textureId = templateEnemy.textureId;
	card.minValue = -1;
	card.maxValue = -1;
	card.durability = 0;
	card.maxDurability = 0;
	card.hp = templateEnemy.hp;
	card.element = Element::NONE;
	card.name = templateEnemy.name;
	card.description = "ENEMY - STANDARD ENEMY";

	if (enemyElement == Element::ICE) {
		card.name = "FROST";
		card.description = "FROST - ICE ENEMY";
		card.element = Element::ICE;
		card.textureId = TextureId::EnemyIce;
	}

	if (enemyElement == Element::FIRE) {
		card.name = "BLAZE";
		card.description = "BLAZE - FIRE ENEMY";
		card.element = Element::FIRE;
		card.textureId = TextureId::EnemyFire;
	}

	return card;
}

Card CardGenerator::GetRandomSpell() {
	std::vector<Element> elements;
	elements.push_back(Element::LIFESTEAL);
	elements.push_back(Element::LIFESTEAL);

	elements.push_back(Element::WARHAMMER);
	elements.push_back(Element::ESCAPE);

	Element selectedElement;

	int seed = GetRandomValue(0, elements.size() - 1);

	selectedElement = elements[seed];

	Card c;
	c.Clear();

	c.name = "SPELL";
	c.description = "SPELL - UNKNOW MAGIC";
	c.hp = 1;

	c.type = CardType::SPELL;

	switch (selectedElement) {
	case (Element::LIFESTEAL): {
		c.name = "LIFESTEAL";
		c.description = "LIFESTEAL - HEALS YOU WITH DAMAGE IT DEALS";
		c.textureId = TextureId::WeaponGoldenSword;
		break;
	}
	case (Element::WARHAMMER): {
		c.name = "WARHAMMER";
		c.description = "WARHAMMER - SEND CARD BACK TO THE MASTER DECK";
		c.textureId = TextureId::WeaponHammer;
		break;
	}
	case (Element::ESCAPE): {
		c.name = "ESCAPE";
		c.description = "ESCAPE - REDRAW 4 CARDS";
		c.textureId = TextureId::FlaskBlue;
		break;
	}
	}

	switch (selectedElement) {
	case Element::LIFESTEAL:
		c.minValue = GetRandomValue(6, 12);
		c.maxValue = c.minValue;
		c.durability = 2;
		c.maxDurability = 2;
		c.element = Element::LIFESTEAL;
		break;
	case Element::WARHAMMER:
		c.maxValue = 1;
		c.minValue = 1;
		c.element = Element::WARHAMMER;
		break;
	case Element::ESCAPE:
		c.minValue = 1;
		c.maxValue = 1;
		c.element = Element::ESCAPE;
		break;
	}

	return c;
}

Card CardGenerator::GetItemParms(const CardType type, const Element element) {
	Card c;
	c.type = type;
	c.element = element;

	c.durability = -1;
	c.maxDurability = -1;
	c.hp = 0;

	switch (type) {
	case CardType::SHIELD:
		c.name = "SHIELD";
		c.description = "SHIELD - DAMAGE ABSORPTION ITEM";
		c.textureId = TextureId::ItemShield;
		break;
	case CardType::POTION:
		c.name = "HP Elixir";
		c.description = "POTION - HEALS YOU";
		c.textureId = TextureId::FlaskRed;
		break;
	case CardType::COIN:
		c.name = "COIN";
		c.description = "COIN - COLLECT GOLD TO SCORE POINTS";
		c.textureId = TextureId::Coin;
		break;
	case CardType::WAND:
		if (c.element == Element::ICE) {
			c.name = "ICE WAND";
			c.description = "ICE WAND - DOUBLE DAMAE TO FIRE";
			c.textureId = TextureId::WeaponWandIce;
		}
		if (c.element == Element::FIRE) {
			c.name = "FIRE WAND";
			c.description = "ICE FIRE - DOUBLE DAMAE TO ICE";
			c.textureId = TextureId::WeaponWandFire;
		}
	}
	return c;
}
