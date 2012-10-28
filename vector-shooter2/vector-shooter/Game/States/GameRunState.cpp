/**
==========================================================================
								Game Run State
==========================================================================
**/

#include "GameRunState.h"
#include "../../Graph/Map.h"

#include <iostream>
#include <SFML/Window/Event.hpp>

/**=============================
GameRunState::Init
=============================**/

void GameRunState::Init(sf::RenderWindow& window,Game& game)
{
	Window_ = &window;
	Game_	= &game;

	StateChanged_ = false;
}

/**=============================
GameRunState::HandleEvents
=============================**/

void GameRunState::HandleEvents()
{
	sf::Event event;
	while (Window_->pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::Closed :
			exit(0);
		}
	}
}

/**=============================
GameRunState::Loop
=============================**/

void GameRunState::Loop()
{
	// Loading a new Map
	MapHandler NewMap;
	NewMap.Init(*Window_);
	NewMap.NewMap("data//maps//MAP1");

	sf::Clock c;
	int count = 0; 

	while (Window_->isOpen())
	{
		HandleEvents();

		Window_->clear(sf::Color(0,0,0));

		NewMap.Update();
		float dt = c.restart().asSeconds();
		if (count > 60)
		{
			std::cout << 1.f / dt << std::endl;
			count = 0;
		}
		++count;

		Window_->display();
	}
}