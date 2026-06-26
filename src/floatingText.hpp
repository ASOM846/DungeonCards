#pragma once

#include <algorithm>
#include <cctype>
#include <raylib.h>
#include <string>
#include <vector>

struct FloatingText {
	std::string text;
	Vector2 position;
	Vector2 velocity;
	Color color;
	float alpha;
	float lifetime;
	float maxLifetime;
	float startSize;
	float currentSize;
};

class EffectManager {
  private:
	std::vector<FloatingText> activeTexts;

  public:
	EffectManager() = default;

	void Reset() { activeTexts.clear(); }

	void SpawnText(Vector2 startPos, std::string text, Color color,
				   float lifetime = 1.0f) {
		FloatingText ft;

		std::string checkStr = text;
		if (!checkStr.empty() && checkStr[0] == '-') {
			checkStr.erase(0, 1);
		}

		bool isNumber =
			!text.empty() &&
			std::all_of(checkStr.begin(), checkStr.end(),
						[](unsigned char c) { return std::isdigit(c); });

		float finalSize = 45.0f;

		if (isNumber) {
			int damageValue = std::stoi(text);

			if (std::abs(damageValue) >= 8) {
				text = text + "!";
				finalSize = 75;
			}
		} else {
			finalSize = 65.0f;
		}

		ft.text = text;
		ft.position = {.x = startPos.x + GetRandomValue(-10, 10),
					   .y = startPos.y - 20};
		ft.velocity = {.x = static_cast<float>(GetRandomValue(-20, 20)) / 10.0F,
					   .y = -60.0F};

		ft.color = color;
		ft.alpha = 1.0f;
		ft.lifetime = lifetime;
		ft.maxLifetime = lifetime;
		ft.startSize = finalSize;
		ft.currentSize = finalSize;

		activeTexts.push_back(ft);
	}

	void Update(float deltaTime) {
		for (auto it = activeTexts.begin(); it != activeTexts.end();) {
			it->position.x += it->velocity.x * deltaTime;
			it->position.y += it->velocity.y * deltaTime;

			it->lifetime -= deltaTime;

			float lifeRatio = it->lifetime / it->maxLifetime;
			lifeRatio = std::max(lifeRatio, 0.0f);

			it->alpha = lifeRatio;

			it->currentSize = it->startSize * (0.8f + 0.2f * lifeRatio);

			if (it->lifetime <= 0.0f) {
				it = activeTexts.erase(it);
			} else {
				++it;
			}
		}
	}

	void Draw() {
		for (const auto &ft : activeTexts) {
			Color finalColor = Fade(ft.color, ft.alpha);

			DrawText(ft.text.c_str(), static_cast<int>(ft.position.x),
					 static_cast<int>(ft.position.y),
					 static_cast<int>(ft.currentSize), finalColor);
		}
	}
};
