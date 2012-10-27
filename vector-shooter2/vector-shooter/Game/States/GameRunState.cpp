/**
==========================================================================
								Game Run State
==========================================================================
**/

#include "GameRunState.h"

#include <SFML\Window\Event.hpp>

/**=============================
GameRunState::GameRunState
=============================**/

GameRunState::GameRunState()
{
}

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
	while (Window_->isOpen())
	{
		HandleEvents();

		Window_->clear(sf::Color(0,0,0));

		Window_->display();
	}
}