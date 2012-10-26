///////////////////////////////////////////////////////
//				WEAPONS CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#ifndef WEAPONS_H_INCLUDED
#define WEAPONS_H_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

// Abstract base class
class BaseWeapon
{
public :
	BaseWeapon();
	BaseWeapon(int,int);
	void				Update(const sf::Time&);

	// accessors
	inline int getType(){return type_;};

protected :
	float				damage_;
	float				spread_;
	sf::Vector2f		speed_;
	int					ammo_;
	int					type_;
	float				fire_tick;
	float				fire_time;
};

#endif