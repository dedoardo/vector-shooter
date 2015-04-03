/**
==========================================================================
								UI Element Header
==========================================================================
**/

#ifndef UI_ELEMENT_H_INCLUDED
#define UI_ELEMENT_H_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class UIElement
{
private :
	sf::Texture*			Texture_;
	sf::Sprite				Sprite_;
	sf::Vector2f			Position_;

public :
	UIElement(sf::Texture& texture,sf::IntRect rect,sf::Vector2f position)
	{
		Position_ = position;
		Texture_ = &texture;
		Sprite_.setTexture(*Texture_);
		Sprite_.setTextureRect(rect);
	}
	void Draw(sf::RenderWindow& window)
	{
		Sprite_.setPosition(Position_);
		window.draw(Sprite_);
	}


};

#endif