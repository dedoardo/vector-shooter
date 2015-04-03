/**
==========================================================================
						Goal Explore Source
==========================================================================
**/


#include "Goal_Explore.h"
#include "Goal_Seek.h"
#include "Goal_GetExplorePosition.h"

/**=============================
Goal_Explore::Goal_Explore
=============================**/
Goal_Explore::Goal_Explore(Enemy& e)
{
	Owner_ = &e;
	Goal_ExplorePosition_ =  new Goal_ExplorePosition(e);
	Goal_Seek_			  =  new Goal_Seek(e);
}

/**=============================
Goal_Explore::~Goal_Explore
=============================**/
Goal_Explore::~Goal_Explore()
{
	delete Goal_ExplorePosition_;
	delete Goal_Seek_;
}

/**=============================
Goal_Explore::Process
=============================**/
int Goal_Explore::Process()
{
	// Removing completed or failed goals and then process the next one
	while (!SubGoals_.empty() && (SubGoals_.front()->IsCompleted() || SubGoals_.front()->HasFailed()))
	{
		SubGoals_.front()->Terminate();
		SubGoals_.pop();
	}

	if (!SubGoals_.empty())
	{
		int r = SubGoals_.front()->Process();

		// removing if it's completed or failed
		if (r == GOAL_STATUS_COMPLETED || r == GOAL_STATUS_FAILED)
			SubGoals_.pop();
		return GOAL_STATUS_ACTIVE;
	}
	else
	{
		// Goal Explore will never end until the Goal_Think does not change the priority
		// i'm thus reactivating the goal
		Activate();
		return GOAL_STATUS_ACTIVE;
	}
}

/**=============================
Goal_Explore::Activate
=============================**/
void Goal_Explore::Activate()
{
	Status_ = GOAL_STATUS_ACTIVE;

	// Adding subgoals to process I'm splitting all the subgoals for perfomance reasons
	// FindRandomPosition
	// Go To Position
	// Find Path to position
	// Seek position 
	SubGoals_.push(Goal_ExplorePosition_);
	SubGoals_.push(Goal_Seek_);
}

/**=============================
Goal_Explore::Terminate
=============================**/
void Goal_Explore::Terminate()
{
	while (!SubGoals_.empty())
	{
		SubGoals_.front()->Terminate();
		SubGoals_.pop();
	}
}