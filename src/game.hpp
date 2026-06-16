#pragma once

#include "gameState.hpp"
#include "pile.hpp"
#include "types.hpp"
#include "ui.hpp"
#include <raylib.h>

class Game {
  public:
	Game() = default;
	~Game() = default;

	void Init();
	void Reset();

	void Run();

	void Update();
	void Draw();

	void HandleInteraction(Pile *&selected, Pile &target, int &score);
	Card GenerateRandomCard();
	Card GenerateCard(const CardType type);

  private:
	Ui ui;

	int score;
	bool gameOver;

	GameState state;

	Pile *selectedPile = nullptr;

	int pileWidth = 150;
	int pileHeight = 250;
};
