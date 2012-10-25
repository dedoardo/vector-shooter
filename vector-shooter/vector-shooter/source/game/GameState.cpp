///////////////////////////////////////////////////////
//				GAME STATE CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#include "GameState.h"
#include "../utility/Data.h"

#include <cstdlib>
#include <iostream>

#include <SFML/Window/Event.hpp>

///////////////////////////////////////////////////////
// MAIN MENU
///////////////////////////////////////////////////////
MainMenuState::MainMenuState()
{
	// It's the first time so the state is null
	classState_ = NEW_GAME_STATE;
	stateChanged_ = false;
}

// Handling methods
void MainMenuState::Init(sf::RenderWindow& window,Game& game)
{
	pWindow_ = &window;
	pGame_   = &game;

	mainMenu_.Init(window,*this);
}

void MainMenuState::HandleEvents()
{
	sf::Event event;
	while (pWindow_->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed :
			pWindow_->close();
			exit(0);
			break;
		default :
			break;
		}
	}
}

// Main menu loop
void MainMenuState::Loop()
{
	while (pWindow_->isOpen())
	{
		HandleEvents();
		pWindow_->clear(sf::Color(0,0,0));

		if (stateChanged_) 
		{
			stateChanged_ = false;
			return;
		}

		mainMenu_.Update();

		pWindow_->display();
	}
}

//////////////////// GAME RUN LOOP/////////////////////////////////////
GameRunState::GameRunState()
{
	classState_ = NEW_GAME_STATE;
}

void GameRunState::Init(sf::RenderWindow& window,Game& game)
{
	pWindow_ = &window;
	pGame_   = &game;

	stateChanged_ = false;

	// Creating the game classes
	player_.Init(window);
	mapHandler_.Init(window);
	mapHandler_.newMap("source\\data\\maps\\map1.map");
	collisionHandler_.Init(player_,mapHandler_);
}

void GameRunState::HandleEvents()
{
	sf::Event event;
	while (pWindow_->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed :
			pWindow_->close();
			exit(0);
			break;
		default :
			break;
		}
	}
}

void GameRunState::Loop()
{

	sf::Clock clock;
	unsigned int count = 0;


	while (pWindow_->isOpen())
	{
		HandleEvents();
		pWindow_->clear(sf::Color(255,255,255));

		if (stateChanged_) 
		{
			stateChanged_ = false;
			return;
		}

		sf::Time dt = clock.restart();

		count += 1;
		if (count >= WINDOW_FPS)
		{
			std::cout << 1.f / dt.asSeconds() << std::endl;
			count = 0;
		}


		// Updating and drawing
		player_.Update(dt);
		mapHandler_.Update();
		collisionHandler_.CheckCollision();

		

		pWindow_->display();
	}
}