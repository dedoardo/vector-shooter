/**
==========================================================================
						Entity file
==========================================================================
**/


#ifndef BASE_ENTITY_H_INCLUDED
#define BASE_ENTITY_H_INCLUDED


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class WeaponSystem;

// abstract class
class Entity
{
public :
	Entity(){};
	Entity(sf::Vector2f pos,float width,float height,unsigned int type):
		Position_(pos),
		Width_(width),
		Height_(height){};

	static unsigned int								NextValidID_;

	float											HitPoints_;

	sf::Vector2f									Position_;
	sf::Vector2f									PreviousPosition_;
	sf::Vector2f									Heading_;
	sf::Vector2f									FireVertex_;
	float											BoundingCircleRadius_;
	float											Angle_;
	float											Width_;
	float											Height_;
	float											Speed_;
	float											Damage_; // only for bullets ( easier for collision detection ) 
	bool											IsDead_;
	Entity*											Owner_;
	float											Radius_;

	// Points of visibility for walls
	bool											TopVisible_;
	bool											RightVisible_;
	bool											DownVisible_;
	bool											LeftVisible_;
	int												MapIndex_;
	int												Kills_;
	std::string										Name_;

	unsigned int									EffectID_;

	WeaponSystem*									WeaponSystem_;

	// few inline useful methods
	inline void AddHP(int amount){HitPoints_ += amount;};
	inline void TakeDamage(float damage){HitPoints_ -= damage;};
	virtual void IncreaseAmmo(){};

protected :
	unsigned int									Id_;


};
#endif // base entity includeed