/**
==========================================================================
								Basic Goal Header
==========================================================================
**/
#ifndef GOAL_BASE_H_INCLUDED
#define GOAL_BASE_H_INCLUDED

#include <queue>
#include "../Enemy.h"
#include "../../Utility/Enum.h"

class Atomic_Goal
{
public :
	virtual int		Process(){return GOAL_STATUS_ACTIVE;};
	virtual void	Activate(){};
	virtual void	Terminate(){};

	inline bool IsCompleted(){return Status_ == GOAL_STATUS_COMPLETED;};
	inline bool HasFailed(){return Status_ == GOAL_STATUS_FAILED;};

protected : 
	int			Status_;
	Enemy*		Owner_;

};

class Composite_Goal
{
public :
	~Composite_Goal()
	{
		while (!SubGoals_.empty())
		{
			delete SubGoals_.back();
			SubGoals_.pop();

		}
	}
	virtual int		Process()=0;
	virtual void	Activate()=0;
	virtual void	Terminate()=0;

	inline bool IsCompleted(){return Status_ == GOAL_STATUS_COMPLETED;};
	inline bool HasFailed(){return Status_ == GOAL_STATUS_FAILED;};

protected : 
	std::queue<Atomic_Goal*>			SubGoals_;
	int								Status_;
	Enemy*							Owner_;
};

#endif