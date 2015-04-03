/**
==========================================================================
						Goal Explore Header
==========================================================================
**/ 

#ifndef GOAL_EXPLORE_POSITION_H_INCLUDED
#define GOAL_EXPLORE_POSITION_H_INCLUDED

#include "Goal.h"
#include "../Enemy.h"

#include <vector>

class Goal_ExplorePosition : public  Atomic_Goal
{
public :
	Goal_ExplorePosition(Enemy&);
	
	virtual int Process();
	virtual void Activate();
	virtual void Terminate();
};


#endif