/**
==========================================================================
						Goal Explore Position Source
==========================================================================
**/

#include "Goal_GetExplorePosition.h"


/**=============================
Goal_Explore::Goal_Explore
=============================**/
Goal_ExplorePosition::Goal_ExplorePosition(Enemy& e)
{
	Owner_ = &e;
}

/**=============================
Goal_Explore::Process
=============================**/
int Goal_ExplorePosition::Process()
{
	// calculating random position on map & storing it 
	int id = Owner_->GetPathFinder()->FindRandomValidPoint();

	Owner_->GetPathFinder()->FindPathTo(id);

	return GOAL_STATUS_COMPLETED;
}

/**=============================
Goal_Explore::Activate
=============================**/
void Goal_ExplorePosition::Activate()
{
	Status_ = GOAL_STATUS_ACTIVE;
}

/**=============================
Goal_Explore::Terminate
=============================**/
void Goal_ExplorePosition::Terminate()
{

}