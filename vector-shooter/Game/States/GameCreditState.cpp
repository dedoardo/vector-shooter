#include "GameCreditState.h"
#include "../../Utility/Enum.h"

#include <SFML/Window/Event.hpp>

void GameCreditState::Init(sf::RenderWindow& window,Game& game)
{
	Window_ = &window;
	Game_ = &game;
}

void GameCreditState::HandleEvents()
{
	sf::Event event;

	while (Window_->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed :
			Window_->close();
			exit(1);
		}
	}

}

void GameCreditState::Loop()
{
	while (Window_->isOpen())
	{
		HandleEvents();

		setState(MAIN_MENU_STATE);
		return;

		Window_->display();
	}
}