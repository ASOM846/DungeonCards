#pragma once

#include "ui.hpp"
#include <raylib.h>

enum class MenuUpdateResoult {
	NONE,
	PLAY,
};

struct Button {
	Rectangle rect;
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

	void Draw() const { Ui::DrawMessageRect(rect, text); }
};

class Menu {
  public:
	void Init();
	void UpdateButtonsPos();

	MenuUpdateResoult Update();
	void Draw() const;

  private:
	Button playButton;

	const char *gameTitle;
};
