
#include "Goal_Seek.h"

/**=============================
Goal_Seek::Goal_Seek
=============================**/
Goal_Seek::Goal_Seek(Enemy& e)
{
	Owner_ = &e;
	ProximityFactor_ = 5.f; // this factor represents the required proximity to the node in order to mark it as reached
}

/**=============================
Goal_Seek::Process
=============================**/
int Goal_Seek::Process()
{
	if (!Owner_->PathToFollow_.empty() &&HasReached())
	{
		// we have reached the destination
		// if it's the last node we are returning completed otherwise next tick another 
		// position will be followed
		Owner_->LastNodePosition_ = Owner_->PathToFollow_.back();
		Owner_->PathToFollow_.pop_back();

		if (Owner_->PathToFollow_.empty())
		{
			return GOAL_STATUS_COMPLETED;
		}

	}
	else if (!Owner_->PathToFollow_.empty())
	{
		// calculating heading to next waypoint
		sf::Vector2f heading = Owner_->PathToFollow_.back() - Owner_->Position_;

		// normaling heading
		float magnitude = sqrt(heading.x * heading.x + heading.y * heading.y);



		heading.x /= magnitude;
		heading.y /= magnitude;

		// making the enemy face the right direction
		Owner_->Angle_ = atan2(heading.y,heading.x) * 180 / 3.1415926;

		// Making him follow the right path
		Owner_->Heading_ = heading;
		return GOAL_STATUS_ACTIVE;
	}
	else
	{
		return GOAL_STATUS_COMPLETED;
	}
}

/**=============================
Goal_Seek::Activate
=============================**/
void Goal_Seek::Activate()
{
	Status_ = GOAL_STATUS_ACTIVE;
}

/**=============================
Goal_Seek::Terminate
=============================**/
void Goal_Seek::Terminate()
{
	// when this goal is terminated the path to follow is cleared
	Owner_->PathToFollow_.clear();
}

/**=============================
Goal_Seek::HasReached
=============================**/
bool Goal_Seek::HasReached()
{
	if (Owner_->Position_.x >= Owner_->PathToFollow_.back().x - ProximityFactor_ &&
		Owner_->Position_.x <= Owner_->PathToFollow_.back().x + ProximityFactor_ &&
		Owner_->Position_.y >= Owner_->PathToFollow_.back().y - ProximityFactor_ &&
		Owner_->Position_.y <= Owner_->PathToFollow_.back().y + ProximityFactor_)
		return true;
	return false;
}