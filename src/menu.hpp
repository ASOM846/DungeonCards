#pragma once

#include "button.hpp"
#include "textureManager.hpp"
#include <raylib.h>

enum class MenuUpdateResoult {
	NONE,
	PLAY,
};

class Menu {
  public:
	void Init();
	void UpdateButtonsPos();

	MenuUpdateResoult Update();
	void Draw(TextureManager &tm) const;

  private:
	Button playButton;

	const char *gameTitle;

	void DrawTitle(TextureManager &tm) const;
};
