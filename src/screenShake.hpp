#pragma once

#include <algorithm>
#include <raylib.h>
#include <stdexcept>

struct ScreenShake {
	Vector2 offset{0.0f, 0.0f};
	float intensity = 0.0f;
	float totalTime = 0.0f;
	float timeLeft = 0.0f;

	void reset() {
		offset = {.x = 0, .y = 0};
		intensity = 0.0F;
		totalTime = 0.0F;
		timeLeft = 0.0F;
	}

	void trigger(float inten = 1.0, float duration = 0.4) {
		intensity = std::max(intensity, inten);
		totalTime = std::max(totalTime, duration);
		timeLeft = std::max(timeLeft, duration);
	}

	void update(float dt) {
		if (timeLeft <= 0.0f) {
			offset = {0.0f, 0.0f};
			intensity = 0.0f;
			return;
		}
		timeLeft -= dt;
		float t = (totalTime > 0.0f) ? (timeLeft / totalTime) : 0.0f;
		float cur = intensity * t;
		float rx =
			(static_cast<float>(GetRandomValue(-100, 100)) / 100.0f) * cur;
		float ry =
			(static_cast<float>(GetRandomValue(-100, 100)) / 100.0f) * cur;
		offset = {.x = rx, .y = ry};
		if (timeLeft <= 0.0f)
			offset = {0.0f, 0.0f};
	}
};
