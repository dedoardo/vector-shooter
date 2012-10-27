/**
==========================================================================
						MainMenu
==========================================================================
**/

// defining enumerator
enum{
	MAINMENU_PLAY,
	MAINMENU_HIGHSCORES,
	MAINMENU_OPTIONS,
	MAINMENU_QUIT
};


#include "MainMenu.h"
#include "..\Game\States\GameMainMenuState.h"
#include "..\Utility\WindowInfo.h"
#include "..\Utility\Enum.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

/**=============================
MainMenu::MainMenu
=============================**/
MainMenu::MainMenu()
{
}

/**=============================
MainMenu::Init
=============================**/
void MainMenu::Init(sf::RenderWindow& window,MainMenuState& state)
{
	Window_ = &window;
	
	// Loading texture
	ButtonOn_.loadFromFile("data\\assets\\grunge\\standard-button-on.png");
	ButtonOff_.loadFromFile("data\\assets\\grunge\\standard-button-off.png");
	DialogBox_.loadFromFile("data\\assets\\grunge\\dialog-box.png");
	
	GameState_ = &state;

	// Calculating the position based on the screen size
	float offSetX = WINDOW_WIDTH/2 - ButtonOn_.getSize().x /2;
	float offSetY = ButtonOn_.getSize().y * 1.5;
	
	Objects_.push_back(MenuObject());
	Objects_[MAINMENU_PLAY].Init(ButtonOn_,ButtonOff_,sf::Vector2f(offSetX,offSetY),"Play",GAME_RUN_STATE);
	Objects_[MAINMENU_PLAY].IsActive_ = true;
	Objects_.push_back(MenuObject());
	Objects_[MAINMENU_HIGHSCORES].Init(ButtonOn_,ButtonOff_,sf::Vector2f(offSetX,offSetY *2),"HighScores",HIGHSCORE_STATE);
	Objects_[MAINMENU_HIGHSCORES].IsActive_ = false;
	Objects_.push_back(MenuObject());
	Objects_[MAINMENU_OPTIONS].Init(ButtonOn_,ButtonOff_,sf::Vector2f(offSetX,offSetY *3),"Options",OPTIONS_STATE);
	Objects_[MAINMENU_OPTIONS].IsActive_ = false;
	Objects_.push_back(MenuObject());
	Objects_[MAINMENU_QUIT].Init(ButtonOn_,ButtonOff_,sf::Vector2f(offSetX,offSetY *4),"Quit",-1);
	Objects_[MAINMENU_QUIT].IsActive_ = false;

	ActiveObject_ = MAINMENU_PLAY;
}

/**=============================
MainMenu::Update
=============================**/

void MainMenu::Update()
{
	std::size_t s = Objects_.size();
	for (std::size_t i = 0; i < s; ++i)
	{
		// Handling events
		sf::Event event;
		while (Window_->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed :
				exit(0);
			case sf::Event::KeyPressed :
				{
					if (event.key.code == sf::Keyboard::Down )
						if ((ActiveObject_ +1) <= 3)
						{
							++ActiveObject_;
							Objects_[ActiveObject_-1].IsActive_ = false;
							Objects_[ActiveObject_].IsActive_ = true;
						}
					if (event.key.code == sf::Keyboard::Up )
						if ((ActiveObject_ -1) >= 0)
						{
							--ActiveObject_;
							Objects_[ActiveObject_+1].IsActive_ = false;
							Objects_[ActiveObject_].IsActive_ = true;
						}
				}
			}
		}

		// checking if enter is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			int state = Objects_[ActiveObject_].ActionOnEnter_;
			if (state != -1)
				GameState_->setState(state);
			else
				exit(0);
		}

		Objects_[i].Draw(*Window_);	
	}
}