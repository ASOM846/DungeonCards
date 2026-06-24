#pragma once

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
};

class EffectManager {
  private:
	std::vector<FloatingText> activeTexts;

  public:
	EffectManager() = default;

	void SpawnText(Vector2 startPos, const std::string &text, Color color,
				   float lifetime = 1.0f) {
		FloatingText ft;
		ft.text = text;
		ft.position = {.x = startPos.x + GetRandomValue(-10, 10),
					   .y = startPos.y - 20};
		ft.velocity = {.x = static_cast<float>(GetRandomValue(-20, 20)) / 10.0F,
					   .y = -60.0F};

		ft.color = color;
		ft.alpha = 1.0f;
		ft.lifetime = lifetime;

		activeTexts.push_back(ft);
	}

	void Update(float deltaTime) {
		for (auto it = activeTexts.begin(); it != activeTexts.end();) {
			it->position.x += it->velocity.x * deltaTime;
			it->position.y += it->velocity.y * deltaTime;

			it->lifetime -= deltaTime;
			it->alpha = it->lifetime;

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
					 static_cast<int>(ft.position.y), 24, finalColor);
		}
	}
};
