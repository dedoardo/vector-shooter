#include "GameHighscoreState.h"

#include <fstream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../Game.h"
#include "../../Utility/Enum.h"
#include "../../Utility/WindowInfo.h"
#include "../../Globals.h"

void GameHighscoreState::Init(sf::RenderWindow& window,Game& game)
{
	Game_ = &game;
	Window_ = &window;

	// hard-coding filename
	Filename_ = "data//new-assets//cfg//highscores.data";

	// pre-loading background atleast
	BackgroundTexture_ = Globals::Assets->GetTexture("highscore_bg");

	StateChanged_ = false;

}

void GameHighscoreState::HandleEvents()
{
	sf::Event event;
	while (Window_->pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::Closed :
			{
				Window_->close();
				exit(0);
			}
		case sf::Event::KeyPressed :
			{
				if (event.key.code == sf::Keyboard::BackSpace)
				{
					setState(MAIN_MENU_STATE);

				}

			}
		}
	}
}

void GameHighscoreState::Loop()
{
	// loading font
	sf::Font* f;
	f = Globals::Assets->GetFont("capture");

	// setting background
	sf::Sprite bg_sprite;
	bg_sprite.setTexture(*BackgroundTexture_);

	std::vector<sf::Text> texts;// texts for the highscores

	// reading data from file
	std::ifstream file_stream;
	file_stream.open(Filename_);

	std::string line;
	sf::Text t; // loading it once, since it will be later copied inside the vector i'll have to just setString()
	t.setFont(*f);
	t.setColor(sf::Color(255,255,255,200));

	// pre-calculating position
	// the y position will change
	int y_count = 0; 

	while (std::getline(file_stream,line))
	{
		int divider_index = line.find_first_of("#");
		t.setString(
			line.substr(0,divider_index) + " : " + line.substr(divider_index +1)
			);
		t.setPosition(WINDOW_WIDTH /2 - t.getLocalBounds().width /2,
			100 + (t.getLocalBounds().height * 2 * y_count));

		texts.push_back(t);

		++y_count;
	}

	// closing file stream
	file_stream.close();

	// calculating size
	std::size_t s = texts.size();

	while (Window_->isOpen())
	{
		if (StateChanged_)
		{
			StateChanged_ = false;
			return;
		}

		HandleEvents();
		Window_->draw(bg_sprite);

		for (int i = 0; i < s;++i)
		{
			Window_->draw(texts[i]);
		}

		Window_->display();
	}
}