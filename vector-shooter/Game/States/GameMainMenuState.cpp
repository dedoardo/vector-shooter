/**
==========================================================================
								MainMenuState
==========================================================================
**/

#include "GameMainMenuState.h"
#include <SFML/Window/Event.hpp>

/**=============================
MainMenuState::Init
=============================**/
void MainMenuState::Init(sf::RenderWindow& window,Game& game)
{
	Window_ = &window;
	Game_   = &game;

	StateChanged_ = false;

	MainMenu_.Init(window,*this,*Game_);
}

/**=============================
MainMenuState::HandleEvents
=============================**/
void MainMenuState::HandleEvents()
{
	sf::Event event;
	while (Window_->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed :
			Window_->close();
			exit(0);
			break;
		default :
			break;
		}
	}
}

/**=============================
MainMenuState::Loop
=============================**/
void MainMenuState::Loop()
{
	while (Window_->isOpen())
	{
		HandleEvents();
		Window_->clear(sf::Color(0,0,0));

		if (StateChanged_) 
		{
			StateChanged_ = false;
			return;
		}

		MainMenu_.Update();

		Window_->display();
	}
}
