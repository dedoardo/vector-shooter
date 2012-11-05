/**
==========================================================================
						Entity file
==========================================================================
**/


#ifndef BASE_ENTITY_H_INCLUDED
#define BASE_ENTITY_H_INCLUDED

#include <SFML/System/Vector2.hpp>

// abstract class
class Entity
{
public :
	Entity(){};
	Entity(sf::Vector2f pos,float width,float height,unsigned int type):
		Position_(pos),
		Width_(width),
		Height_(height),
		Type_(type){};

	static unsigned int								NextValidID_;


	sf::Vector2f									Position_;
	sf::Vector2f									PreviousPosition_;
	sf::Vector2f									Heading_;
	sf::Vector2f									FireVertex_;
	float											Angle_;
	float											Width_;
	float											Height_;

	// Points of visibility for walls
	bool											TopVisible_;
	bool											RightVisible_;
	bool											DownVisible_;
	bool											LeftVisible_;
	int												MapIndex_;

protected :
	unsigned int									Type_;


};
#endif // base entity includeed