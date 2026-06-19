#pragma once
#include "textureManager.hpp"
#include "ui.hpp"
#include <raylib.h>

struct Button {
	Rectangle rect;
	float shadowOffset;
	const char *text;

	Button() = default;
	Button(Rectangle rectangle, const char *p_text) {
		rect = rectangle;
		text = p_text;
	}

	[[nodiscard]] bool IsClicked(const Vector2 mousePos) const {
		return (CheckCollisionPointRec(mousePos, rect) &&
				IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
	}

	[[nodiscard]] bool IsHovered(const Vector2 mousePos) const {
		return (CheckCollisionPointRec(mousePos, rect));
	}

	void Update(Vector2 mousePos) {
		shadowOffset = 4.0f;
		if (IsHovered(mousePos)) {
			rect.y -= 12.0f;
			shadowOffset = 14.0f;
		}
	}

	void Draw(TextureManager &tm) const {
		Ui::DrawMessageRect(rect, text, shadowOffset, tm.getCustonFont());
	}
};
