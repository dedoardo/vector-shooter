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
#include "../Entity/Entity.h"

class Bullet : public Entity
{
public :
	// Defining all methods
	Bullet::Bullet(sf::Texture& texture,sf::Vector2f& position,sf::Vector2f& heading,float speed,float damage,Entity& entity,float radius)
	{
		Position_ = position;
		PreviousPosition_ = position;
		Heading_.x = -heading.x;
		Heading_.y = heading.y;
		Texture_ = &texture;
		Speed_ = speed;
		Damage_ = damage;
		Owner_ = &entity;
		Radius_ = radius;

		Width_ = Texture_->getSize().x;
		Height_ = Texture_->getSize().y;

		Sprite_.setTexture(*Texture_);
		Sprite_.setOrigin(Texture_->getSize().x /2,Texture_->getSize().y /2);
		Origin_ = Sprite_.getOrigin();

		// faking a timestep
		Position_.x += Heading_.x * 20;
		Position_.y += Heading_.y * 20;

		Sprite_.setPosition(Position_);
	}

private :

	sf::Texture*				Texture_;
	sf::Sprite					Sprite_;
	sf::Vector2f				Origin_;

public :

	inline float GetSpeed(){return Speed_;};
	inline sf::Vector2f GetPosition(){return Position_;};
	inline sf::Vector2f GetPreviousPosition(){return PreviousPosition_;};
	inline float GetAngle(){return Angle_;};
	inline sf::Vector2f GetOrigin(){return Origin_;};
	inline float GetWidth(){return Width_;};
	inline float GetHeight(){return Height_;};
	inline float GetDamage(){return Damage_;};
	inline Entity* GetOwner(){return Owner_;};

	inline void	SetPosition(sf::Vector2f newpos){Position_ = newpos;};

	// easy functions for wall collision
	inline void ReverseYHeading()
	{
		Heading_.y = - Heading_.y; 
	}

	inline void ReverseXHeading()
	{
		Heading_.x = - Heading_.x;
	}

	void Draw(sf::RenderWindow& window,const sf::Time& deltaTime)
	{
		PreviousPosition_ = Position_;
		Position_.x += Heading_.x * Speed_ * deltaTime.asSeconds();
		Position_.y += Heading_.y * Speed_ * deltaTime.asSeconds();

		Angle_ = atan2(Heading_.y,Heading_.x) * 180 / 3.1415926;
		Sprite_.setRotation(Angle_);
		  
		Sprite_.setPosition(Position_);
		window.draw(Sprite_);
	}
};

#endif