/**
==========================================================================
								Game File
==========================================================================
**/

#include "Game.h"
#include "States\BaseGameState.h"
#include "..\Utility\Enum.h"
#include "..\Utility\WindowInfo.h"
#include "..\Controller\ControllerXBox.h"
#include "States\GameMainMenuState.h"
#include "States/GameRunState.h"

#include <iostream>

/**=============================
Game::Game
=============================**/
Game::Game()
{
} 

/**=============================
Game::Run
=============================**/
void Game::Run()
{
	// Initializing the window
	sf::VideoMode videoMode(WINDOW_WIDTH,WINDOW_HEIGHT,32);
	sf::RenderWindow window(videoMode,WINDOW_CAPTION);

	Window_ = &window;
	VideoMode_ = &videoMode;
	Window_->setFramerateLimit(WINDOW_FPS);

	// Initializing all the gamestates
	InitStates();

	// Initializing the controller
	ControllerXBox_ = new ControllerXBox(1);

	if (ControllerXBox_->IsConnected())
		std::cout << "connected !\n"; 

	// Running the current state
	while (true)
	{
		ActiveGameState_->Loop();
	}

}

/**=============================
Game::InitStates
=============================**/
void Game::InitStates()
{
	States_.reserve(STATES_NUMBER);
	States_.resize(STATES_NUMBER);

	MainMenuState* mmstate = new MainMenuState();
	mmstate->Init(*Window_,*this);
	States_[MAIN_MENU_STATE] = mmstate;

	GameRunState* gmstate = new GameRunState();
	gmstate->Init(*Window_,*this);
	States_[GAME_RUN_STATE] = gmstate;

	ActiveGameState_ = States_[MAIN_MENU_STATE];

}

/**=============================
Game::~Game
=============================**/
Game::~Game()
{
	std::size_t stateSize = States_.size();
	for (int i = 0; i < stateSize; ++i)
	{
		delete States_[i];
	}
	delete ControllerXBox_;
}