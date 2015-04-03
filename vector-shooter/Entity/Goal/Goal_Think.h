/**
==========================================================================
							Goal Think Header
==========================================================================
**/

#ifndef GOAL_THINK_H_INCLUDED
#define GOAL_THINK_H_INCLUDED

#include "Goal.h"

class Goal_Think
{
public :
	Goal_Think(Enemy&);
	~Goal_Think();
	void		Evaluate(); // Evaluates and chooses the best action
	void		Update(); // processes all the goals

	float		EvaluateAttack();
	float		EvaluateGetHealth();
	float		EvaluateGetWeapon();
	float		EvaluateExplore();

private : 
	// time variables for AI Update
	int								ThinkTime_;
	int								ThinkTick_;


	Enemy*		Owner_;
	std::vector<Composite_Goal*>	StrategicGoals_;
	Composite_Goal*					ActiveGoal_;
	int								LastActiveGoal_;
};

#endif