///////////////////////////////////////////////////////
//				GAME CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#include "Game.h"
#include "GameState.h"
#include "../utility/Data.h"

// Declaring
void Game::run()
{
	// Creating the window
	sf::VideoMode videoMode(WINDOW_WIDTH,WINDOW_HEIGHT,32);
	pVideoMode_ = &videoMode;
	sf::RenderWindow window(*pVideoMode_,WINDOW_CAPTION);
	pWindow_    = &window;
	pWindow_->setFramerateLimit(WINDOW_FPS);

	// Initializing the state and pushing them back in the right order
	MainMenuState mMState = MainMenuState(); 
	states_[state::MAIN_MENU] = &mMState;
	states_[state::MAIN_MENU]->Init(*pWindow_,*this);
	GameRunState gRState = GameRunState();
	states_[state::GAME_RUN] = & gRState;
	states_[state::GAME_RUN]->Init(*pWindow_,*this);


	// Setting the first state to main menu
	pActiveGameState_ = states_[state::MAIN_MENU];
	pPreviousGameState_ = states_[state::MAIN_MENU];

	// Main loop
	while (true)
	{
		pActiveGameState_->Loop();
	}

}