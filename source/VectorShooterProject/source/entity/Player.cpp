///////////////////////////////////////////////////////
//				PLAYER CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#include "Player.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Mouse.hpp>

// the constructor is the initializer too
Player::Player()
{
}

void Player::Init(sf::RenderWindow& window)
{
	pWindow_ = &window;

	position_ = sf::Vector2f(0,0);
	heading_  = sf::Vector2f(1,0);
	speed_	  = sf::Vector2f(0,0);
	acceleration_ = sf::Vector2f(3,3);
	max_speed_ = sf::Vector2f(10,10);

	// Loading graphics
	texture_.loadFromFile("../data/Player.png");
	sprite_.setTexture(texture_);
	sprite_.setOrigin(texture_.getSize().x /2,texture_.getSize().y /2);
	sprite_.setPosition(position_);

	width_ = texture_.getSize().x;
	height_ = texture_.getSize().y;
	bounds_.reserve(4);
	bounds_.push_back(sf::Vector2f(position_.x,position_.y));
	bounds_.push_back(sf::Vector2f(position_.x + width_,position_.y));
	bounds_.push_back(sf::Vector2f(position_.x + width_,position_.y + height_));
	bounds_.push_back(sf::Vector2f(position_.x,position_.y + height_));

}

// Update method called every loop to update positon and other stats
void Player::Update(const sf::Time& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		speed_ += (acceleration_ * deltaTime.asSeconds());
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		speed_ -= (acceleration_ * deltaTime.asSeconds());
	}
	else 
	{
		if (speed_.x < 0) speed_.x += acceleration_.x*2 * deltaTime.asSeconds();
		if (speed_.y <0) speed_.y += acceleration_.y*2 * deltaTime.asSeconds();
		if (speed_.x > 0) speed_.x -= acceleration_.x*2 * deltaTime.asSeconds();
		if (speed_.y >0) speed_.y -= acceleration_.y*2 * deltaTime.asSeconds();
	};

	sf::Vector2i mousePosition = sf::Mouse::getPosition(*pWindow_);
	sf::Vector2f disp;
	disp.x = mousePosition.x - position_.x;
	disp.y = mousePosition.y - position_.y;
	float magnitude = getMagnitude(disp);
	if (magnitude != 0)
	{ 
		heading_.x = disp.x / magnitude;
		heading_.y = disp.y / magnitude;
	}
	AdjustSpeed();

	position_.x += speed_.x * heading_.x;
	position_.y += speed_.y * heading_.y;
	sprite_.setRotation(atan2(heading_.y,heading_.x) * 180 / MATH_PI);
	sprite_.setPosition(position_);

	pWindow_->draw(sprite_);
}

// Utility functions
void Player::AdjustSpeed()
{
	speed_.x = speed_.x>max_speed_.x?max_speed_.x:speed_.x;
	speed_.y = speed_.y>max_speed_.y?max_speed_.y:speed_.y;
	speed_.x = speed_.x<-max_speed_.x?-max_speed_.x:speed_.x;
	speed_.y = speed_.y<-max_speed_.y?-max_speed_.y:speed_.y;
}