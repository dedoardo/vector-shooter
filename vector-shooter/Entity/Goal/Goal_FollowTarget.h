/**
==========================================================================
						Goal Follow Target Header
==========================================================================
**/


#ifndef GOAL_FOLLOW_TARGET_H_INCLUDED
#define GOAL_FOLLOW_TARGET_H_INCLUDED

#include "Goal.h"

class Enemy;

class Goal_FollowTarget : public Composite_Goal
{
	Goal_FollowTarget(Enemy&);

	int Process();
	void Activate();
	void Terminate();

};

#endif // goal follow target