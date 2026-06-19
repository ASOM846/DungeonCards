#pragma once

#include <raylib.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

enum class TextureId {
	Coin,
	HandL,
	HandR,
	Knight1,
	Enemy1,
	Enemy2,
	Enemy3,
	Enemy4,
	Enemy5,
	Enemy6,
	Enemy7,
	Enemy8,
	EnemyFire,
	EnemyIce,
	ItemShield,
	FlaskBlue,
	FlaskGreen,
	FlaskRed,
	FlaskYellow,
	WeaponAxe,
	WeaponDoubleAxe,
	WeaponGoldenSword,
	WeaponHammer,
	WeaponSword,
	Background,
	Vinete,
};

class TextureManager {
  public:
	TextureManager() = default;
	~TextureManager() { unloadAll(); };

	const Texture2D &get(TextureId id) const {
		auto it = textures.find(id);
		if (it == textures.end()) {
			throw std::runtime_error("No texture was found");
		}
		return it->second;
	}

	void loadAll() {
		load(TextureId::Coin, "assets/coin.png");
		load(TextureId::HandL, "assets/handL.png");
		load(TextureId::HandR, "assets/handR.png");
		load(TextureId::Knight1, "assets/knight1.png");
		load(TextureId::Enemy1, "assets/enemy1.png");
		load(TextureId::Enemy2, "assets/enemy2.png");
		load(TextureId::Enemy3, "assets/enemy3.png");
		load(TextureId::Enemy4, "assets/enemy4.png");
		load(TextureId::Enemy5, "assets/enemy5.png");
		load(TextureId::Enemy6, "assets/enemy6.png");
		load(TextureId::Enemy7, "assets/enemy7.png");
		load(TextureId::Enemy8, "assets/enemy8.png");
		load(TextureId::EnemyFire, "assets/enemyFire.png");
		load(TextureId::EnemyIce, "assets/enemyIce.png");
		load(TextureId::ItemShield, "assets/itemShield.png");
		load(TextureId::FlaskBlue, "assets/flaskBlue.png");
		load(TextureId::FlaskGreen, "assets/flaskGreen.png");
		load(TextureId::FlaskRed, "assets/flaskRed.png");
		load(TextureId::FlaskYellow, "assets/flaskYellow.png");
		load(TextureId::WeaponAxe, "assets/weaponAxe.png");
		load(TextureId::WeaponDoubleAxe, "assets/weaponDoubleAxe.png");
		load(TextureId::WeaponGoldenSword, "assets/weaponGoldenSword.png");
		load(TextureId::WeaponHammer, "assets/weaponHammer.png");
		load(TextureId::WeaponSword, "assets/weaponSword.png");
		load(TextureId::Background, "assets/bcg4.png");
		load(TextureId::Vinete, "assets/vinete2.png");
	}

	void load(TextureId id, const char *path) {
		Texture2D tex = LoadTexture(path);
		if (tex.id == 0) {
			throw std::runtime_error(std::string("Unable to load: ") + path);
		}
		textures.emplace(id, tex);
	}

	void unloadAll() {
		for (auto &pair : textures) {
			UnloadTexture(pair.second);
		}
		textures.clear();
	}

	std::unordered_map<TextureId, Texture2D> textures;
};
