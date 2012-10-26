
#include "Menu.h"
#include "../utility/Data.h"
#include "../game/GameState.h"
#include <SFML/Window/Mouse.hpp>

// Menu object
MenuObject::MenuObject()
{
}

void MenuObject::Init(sf::Texture& textureOn,sf::Texture& textureOff,sf::Vector2f& position,std::string text)
{
	position_ = position;
	textureOff_ = &textureOff;
	textureOn_ = &textureOn;
	spriteOn_.setTexture(*textureOn_);
	spriteOff_.setTexture(*textureOff_);
	width_ = static_cast<float>(textureOn_->getSize().x);
	height_ = static_cast<float>(textureOn_->getSize().y);
	
		// Loading text
	font_.loadFromFile("../data/fonts/sub.ttf");
	text_.setString(text);
	text_.setColor(sf::Color(0,255,0));
	text_.setFont(font_);
	text_.setPosition(position.x + width_/2 - text_.getLocalBounds().width/2,
					  position.y + height_/2 - text_.getLocalBounds().height);

}

void MenuObject::Draw(sf::RenderWindow& window)
{
	text_.setFont(font_);
	state_ = MENU_OFF;
	isOn(window);
	if (state_ == MENU_ON)
	{
		spriteOn_.setPosition(position_);
		window.draw(spriteOn_);
	}
	else 
	{
		spriteOff_.setPosition(position_);
		window.draw(spriteOff_);
	};
	window.draw(text_);
}

bool MenuObject::isOn(const sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	if (mousePos.x > position_.x && mousePos.x < (position_.x + width_ )
		&& mousePos.y > position_.y && mousePos.y < (position_.y + height_))
	{
		state_ = MENU_ON;
		return true;
	}
	return false;

}

bool MenuObject::isClicked(const sf::RenderWindow& window)
{
	if (isOn(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
		return true;
	return false;
}

///////////////////////////////////////////////////
//	MAIN MENU CLASS
////////////////////////////////////////////////////
MainMenu::MainMenu()
{
}

void MainMenu::Init(sf::RenderWindow& window,MainMenuState& state)
{
	pWindow_ = &window;
	
	// Loading texture
	buttonOn_.loadFromFile("data/assets/grunge/standard-button-on.png");
	buttonOff_.loadFromFile("data/assets/grunge/standard-button-off.png");
	dialogBox_.loadFromFile("data/assets/grunge/dialog-box.png");
	
	pGameState_ = &state;

	// Calculating the position based on the screen size
	float offSetX = WINDOW_WIDTH/2 - buttonOn_.getSize().x /2;
	float offSetY = buttonOn_.getSize().y * 1.5;
	
	menuObjects_.push_back(MenuObject());
	menuObjects_[MAINMENU_PLAY].Init(buttonOn_,buttonOff_,sf::Vector2f(offSetX,offSetY),"Play");
	menuObjects_.push_back(MenuObject());
	menuObjects_[MAINMENU_HIGHSCORES].Init(buttonOn_,buttonOff_,sf::Vector2f(offSetX,offSetY *2),"HighScores");
	menuObjects_.push_back(MenuObject());
	menuObjects_[MAINMENU_OPTIONS].Init(buttonOn_,buttonOff_,sf::Vector2f(offSetX,offSetY *3),"Options");
	menuObjects_.push_back(MenuObject());
	menuObjects_[MAINMENU_QUIT].Init(buttonOn_,buttonOff_,sf::Vector2f(offSetX,offSetY *4),"Quit");
}

void MainMenu::Update()
{
	for (int i = 0; i < menuObjects_.size(); i++)
	{
		menuObjects_[i].Draw(*pWindow_);
		if (menuObjects_[i].isClicked(*pWindow_) && i == MAINMENU_QUIT)
			exit(0);
		else if (menuObjects_[i].isClicked(*pWindow_))
			pGameState_->setState(i);
	}
}