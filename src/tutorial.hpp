#pragma once
#include "button.hpp"
#include "textureManager.hpp"
#include <raylib.h>

class TutorialState {
  public:
	TutorialState() = default;
	~TutorialState() = default;

	void Init();
	void Reset();

	void UpdateFramePos();

	void Update(Vector2 mousePos);
	void Draw(TextureManager &textureManager);

	[[nodiscard]] bool ShouldReturnToMenu() const { return returnToMenu; }

  private:
	void DrawTitle();

	Button buttonForward;
	Button buttonBack;
	Button buttonReturn;

	int frameW;
	int frameH;
	Rectangle imageDsc;

	int currentImage = 0;
	int imageCount = 3;

	bool returnToMenu = false;

	const std::vector<TextureId> images = {
		TextureId::Tutorial1, TextureId::Tutorial2, TextureId::Tutorial3,
		TextureId::Tutorial4};
};
