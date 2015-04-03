/**
==========================================================================
						Goal Explore Header
==========================================================================
**/ 

#ifndef GOAL_EXPLORE_H_INCLUDED
#define GOAL_EXPLORE_H_INCLUDED

#include "Goal.h"
#include "../Enemy.h"
#include "Goal_Seek.h"
#include "Goal_GetExplorePosition.h"

class Goal_Explore : public  Composite_Goal
{
public :
	Goal_Explore(Enemy&);
	~Goal_Explore();

	virtual int Process();
	virtual void Activate();
	virtual void Terminate();

private :
	Goal_Seek*				Goal_Seek_;
	Goal_ExplorePosition*	Goal_ExplorePosition_;

};


#endif