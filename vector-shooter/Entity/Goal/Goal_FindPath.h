/**
==========================================================================
						Goal Find Path
==========================================================================
**/ 

#ifndef GOAL_FIND_PATH_H_INCLUDED
#define GOAL_FIND_PATH_H_INCLUDED

#include "Goal.h"
#include "../Enemy.h"
#include <SFML/System/Vector2.hpp>

class Goal_FindPath : public Composite_Goal
{
public :
	Goal_FindPath(Enemy&);
	
	int Process();
	void Activate();
	void Terminate();
	void SetPath(sf::Vector2f&);

};

#endif