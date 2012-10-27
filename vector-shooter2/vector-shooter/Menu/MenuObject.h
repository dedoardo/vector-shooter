/**
==========================================================================
						Menu Object - Header only
==========================================================================
**/

/**=============================
Includes
=============================**/

#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


#ifndef MENU_OBJECT_H_INCLUDED
#define MENU_OBJECT_H_INCLUDED

class MenuObject
{
public :
	MenuObject(){};
	void					Init(sf::Texture& tON,sf::Texture& tOFF,sf::Vector2f& pos,std::string text,int pointTo)
	{
		Position_ = pos;
		TextureOn_ = &tON;
		TextureOff_ = &tOFF;
		ActionOnEnter_ = pointTo;

		SpriteOn_.setTexture(*TextureOn_);
		SpriteOff_.setTexture(*TextureOff_);
		Width_ = TextureOn_->getSize().x;
		Height_ = TextureOn_->getSize().y;

		Font_.loadFromFile("data\\fonts\\sub.ttf");
		Text_.setString(text);
		Text_.setColor(sf::Color(0,255,0));
		Text_.setFont(Font_);
		Text_.setPosition(Position_.x + Width_/2 - Text_.getLocalBounds().width/2,
					  Position_.y + Height_/2 - Text_.getLocalBounds().height);
		SpriteOn_.setPosition(Position_);
		SpriteOff_.setPosition(Position_);

	}
	void					Draw(sf::RenderWindow& window)
	{
		if (IsActive_)
			window.draw(SpriteOn_);
		else 
			window.draw(SpriteOff_);
		Text_.setFont(Font_);
		window.draw(Text_);
	}

	bool					IsActive_;
	int						ActionOnEnter_;
private :
	sf::Vector2f			Position_;
	sf::Texture*			TextureOn_;
	sf::Texture*			TextureOff_;
	sf::Sprite				SpriteOn_;
	sf::Sprite				SpriteOff_;

	int						Width_;
	int						Height_;
	sf::Text				Text_;
	sf::Font				Font_;
	sf::Color				Color_;


};

#endif 