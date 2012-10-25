///////////////////////////////////////////////////////
//				PLAYER CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>

#include "Entity.h"
#include "../utility/Data.h"

class Player : BaseEntity
{
public :
	//////////////////////////////////
	// CTORS
	//////////////////////////////////

	Player();
	void				Init(sf::RenderWindow&);

	/////////////////////////////////
	// METHODS
	/////////////////////////////////
	void				Fire();
	void				Update(const sf::Time&);

	void				addWeapon();
	void				removeWeapon(int);

	/////////////////////////////////
	// ACCESSORS & MUTATORS
	/////////////////////////////////

	// accessors
	sf::Vector2f							getPosition();
	sf::Vector2f							getHeading();
	float									getHealt();
	sf::Vector2f							getSpeed();
	inline float							getRotationAngle(){return static_cast<float>(atan2(heading_.y,heading_.x) * 180 / MATH_PI);}
	inline float							getRotationRadians(){return static_cast<float>(atan2(heading_.y,heading_.x));};
	std::vector<sf::Vector2f>*				getBounds()
	{
		bounds_[0] = position_;
		bounds_[1] = sf::Vector2f(position_.x + width_,position_.y);
		bounds_[2] = sf::Vector2f(position_.x + width_,position_.y + height_);
		bounds_[3] = sf::Vector2f(position_.x,position_.y + height_);

		return &bounds_;
	}

	// mutators
	void				setPosition(const sf::Vector2f&);
	void				setSpeed(const sf::Vector2f&);
	void				setHeading(const sf::Vector2f&);
	void				goBack();


private :
	// utility functions
	void				AdjustSpeed();

	// Statistics
	float				healt_;
	int					kill_;
	int					kill_streak_;
	
	// Position etc..
	sf::Vector2f		position_;
	sf::Vector2f		heading_;
	sf::Vector2f		speed_;
	sf::Vector2f		acceleration_;
	sf::Vector2f		max_speed_;

	// Interface stuff
	// WeaponSystem

	// Graphics
	sf::Sprite			sprite_;
	sf::Texture			texture_;
	sf::RenderWindow*	pWindow_;
	int					width_;
	int					height_;
	std::vector<sf::Vector2f> bounds_;

};

#endif