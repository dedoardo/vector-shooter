/**
==========================================================================
						Goal Seek Target Header
==========================================================================
**/ 

#ifndef GOAL_SEEKTARGET_H_INCLUDED
#define GOAL_SEEKTARGET_H_INCLUDED

#include "Goal.h"

class Enemy;
class Goal_Seek;

class Goal_SeekTarget : public Composite_Goal
{
public :
	Goal_SeekTarget(Enemy&);
	~Goal_SeekTarget();

	void			Activate();
	int				Process();
	void			Terminate();

private : 

	Goal_Seek*		Goal_Seek_;

};


#endif // goal seek included