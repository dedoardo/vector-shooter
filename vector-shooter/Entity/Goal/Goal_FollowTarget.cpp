/**
==========================================================================
						Goal Follow Target source
==========================================================================
**/

#include "Goal_FollowTarget.h"
#include "../../Entity/Enemy.h"

/**=============================
Goal_FollowTarget::Goal_FollowTarget
=============================**/
Goal_FollowTarget::Goal_FollowTarget(Enemy& enemy)
{
	Owner_ = &enemy;
}

/**=============================
Goal_FollowTarget::Process
=============================**/
int Goal_FollowTarget::Process()
{
	return GOAL_STATUS_COMPLETED;
}


/**=============================
Goal_FollowTarget::Activate
=============================**/
void Goal_FollowTarget::Activate()
{

}


/**=============================
Goal_FollowTarget::Terminate
=============================**/
void Goal_FollowTarget::Terminate()
{

}