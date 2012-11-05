/**
==========================================================================
								Bullet Header
==========================================================================
**/


#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Bullet
{
public :
	// Defining all methods
	Bullet::Bullet(sf::Texture& texture,sf::Vector2f& position,sf::Vector2f& heading,float speed)
	{
		Position_ = position;
		Heading_.x = -heading.x;
		Heading_.y = heading.y;
		Texture_ = &texture;
		Speed_ = speed;

		Sprite_.setTexture(*Texture_);
		Sprite_.setOrigin(Texture_->getSize().x /2,Texture_->getSize().y /2);
	}

private :

	sf::Texture*				Texture_;
	sf::Sprite					Sprite_;
	sf::Vector2f				Position_;
	sf::Vector2f				Heading_;
	float						Speed_; // it decreases over time
										// it is used also as "dead" check
public :

	inline float GetSpeed(){return Speed_;};
	inline sf::Vector2f GetPosition(){return Position_;};

	void Draw(sf::RenderWindow& window,const sf::Time& deltaTime)
	{

		Position_.x += Heading_.x * Speed_ * deltaTime.asSeconds();
		Position_.y += Heading_.y * Speed_ * deltaTime.asSeconds();

		Sprite_.setRotation(atan2(Heading_.y,Heading_.x) * 180 / 3.1415926);

		Sprite_.setPosition(Position_);
		window.draw(Sprite_);
	}
};

#endif