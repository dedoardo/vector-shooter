/**
==========================================================================
						Goal Seek Position
==========================================================================
**/ 

#ifndef GOAL_SEEK_H_INCLUDED
#define GOAL_SEEK_H_INCLUDED

#include "Goal.h"
#include "../Enemy.h"

class Goal_Seek : public Atomic_Goal
{
public :
	Goal_Seek(Enemy&);
	
	int Process();
	void Activate();
	void Terminate();

	bool HasReached();

private :
	float			ProximityFactor_;

};

#endif