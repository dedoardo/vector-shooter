/**
==========================================================================
						Goal Seek Target Source
==========================================================================
**/

#include "Goal_SeekTarget.h"
#include "Goal_Seek.h"
#include "../../Entity/Enemy.h"

/**=============================
Goal_SeekTarget::Goal_SeekTarget
=============================**/
Goal_SeekTarget::Goal_SeekTarget(Enemy& enemy)
{
	Owner_ = &enemy;

	Goal_Seek_ = new Goal_Seek(enemy);

}

/**=============================
Goal_SeekTarget::~Goal_SeekTarget
=============================**/
Goal_SeekTarget::~Goal_SeekTarget()
{
	delete Goal_Seek_;
}

/**=============================
Goal_SeekTarget::Process
=============================**/
int Goal_SeekTarget::Process()
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
Goal_SeekTarget::Activate
=============================**/
void Goal_SeekTarget::Activate()
{

}

/**=============================
Goal_SeekTarget::Terminate
=============================**/
void Goal_SeekTarget::Terminate()
{

}