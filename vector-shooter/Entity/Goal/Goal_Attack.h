/**
==========================================================================
							Goal Attack Header
==========================================================================
**/


#ifndef GOAL_ATTACK_H_INCLUDED
#define GOAL_ATTACK_H_INCLUDED

#include "Goal.h"

class Enemy;

class Goal_Attack : public Composite_Goal
{
public :
	Goal_Attack(Enemy&);

	int				Process();
	void			Activate();
	void			Terminate();

private :
	Entity*			LastTarget_;
	int				StrafeTick_;
	float			StrafeTime_;
};

#endif