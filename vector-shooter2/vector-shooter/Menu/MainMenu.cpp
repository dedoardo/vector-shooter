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
#include "..\Game\Game.h"
#include "..\Controller\ControllerXBox.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <iostream>

/**=============================
MainMenu::MainMenu
=============================**/
MainMenu::MainMenu()
{
}

/**=============================
MainMenu::Init
=============================**/
void MainMenu::Init(sf::RenderWindow& window,MainMenuState& state,Game& game)
{
	Window_ = &window;
	Game_ = &game;
	
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

	ChangeOptionTick_ = 100;

}

/**=============================
MainMenu::Update
=============================**/

void MainMenu::Update()
{

	sf::Clock clock;

	std::size_t s = Objects_.size();
	Window_->setFramerateLimit(WINDOW_FPS);
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

		float dt = clock.restart().asSeconds();

		// Now checking for xbox 360 controller 
		if (Game_->ControllerXBox_->IsConnected())
		{

			ChangeOptionTick_ += dt;
			if (ChangeOptionTick_ >= 0.01)
			{
				if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					if ((ActiveObject_ +1) <= 3)
					{
						++ActiveObject_;
						Objects_[ActiveObject_-1].IsActive_ = false;
						Objects_[ActiveObject_].IsActive_ = true;
						ChangeOptionTick_ = 0;
					}
				}

				else if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					if ((ActiveObject_ -1) >= 0)
					{
						--ActiveObject_;
						Objects_[ActiveObject_+1].IsActive_ = false;
						Objects_[ActiveObject_].IsActive_ = true;
						ChangeOptionTick_ = 0;
					}
				}

				// checking if A key is pressed
				else if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					int state = Objects_[ActiveObject_].ActionOnEnter_;
					if (state != -1)
					{
						GameState_->setState(state);
					}
					else
						exit(0);
				}
			}
		}
		else 
		{
			ChangeOptionTick_ += dt;
			if (ChangeOptionTick_ > 0.03)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					if ((ActiveObject_ +1) <= 3)
					{
						++ActiveObject_;
						Objects_[ActiveObject_-1].IsActive_ = false;
						Objects_[ActiveObject_].IsActive_ = true;
						ChangeOptionTick_ = 0;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					if ((ActiveObject_ -1) >= 0)
					{
						--ActiveObject_;
						Objects_[ActiveObject_+1].IsActive_ = false;
						Objects_[ActiveObject_].IsActive_ = true;
						ChangeOptionTick_ = 0;
					}
				}
			}
		}

		Objects_[i].Draw(*Window_);
	}
}