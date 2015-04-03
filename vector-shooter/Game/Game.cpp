/**
==========================================================================
								Game File
==========================================================================
**/

#include "Game.h"
#include "States/BaseGameState.h"
#include "../Utility/Enum.h"
#include "../Utility/WindowInfo.h"
#include "../Controller/ControllerXBox.h"
#include "States/GameMainMenuState.h"
#include "States/GameRunState.h"
#include "States/GameHighscoreState.h"
#include "States/GameCreditState.h"
#include "../Globals.h"

#include <iostream>
#include <random>
#include <ctime>
#include <typeinfo>

namespace Globals
{
	AssetManager* Assets;
}

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

	// Seeding the random number generator
	srand(time(NULL));

	// Setting asset manager
	Globals::Assets = new AssetManager("asset_info.xml");

	Window_ = &window;
	VideoMode_ = &videoMode;

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
		//if(typeid(ActiveGameState_) == typeid(States_[GAME_RUN_STATE]))
		//{
			//delete States_[GAME_RUN_STATE];
			//States_[GAME_RUN_STATE] = new GameRunState();
		//	States_[GAME_RUN_STATE]->Init(*Window_,*this);
		//}
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

	GameHighscoreState* ghstate = new GameHighscoreState();
	ghstate->Init(*Window_,*this);
	States_[HIGHSCORE_STATE] = ghstate;

	GameCreditState* cstate = new GameCreditState();
	cstate->Init(*Window_,*this);
	States_[CREDITS_STATE] = cstate;

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
	delete Globals::Assets;
}