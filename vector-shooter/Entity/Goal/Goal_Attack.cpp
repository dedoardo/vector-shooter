/**
==========================================================================
							Goal Attack source
==========================================================================
**/

#include "Goal_Attack.h"
#include "../Enemy.h"
#include "../../Utility/Enum.h"
#include "../../Weapon/WeaponSystem.h"
#include "../EnemyFactory.h"
#include "../../Game/States/GameRunState.h"

#include <limits>
#include <cassert>


/**=============================
Goal_Attack::Goal_Attack
=============================**/
Goal_Attack::Goal_Attack(Enemy& e)
{
	Owner_ = &e;
	StrafeTime_ = rand() % 1000;
	StrafeTick_ = StrafeTime_;
}


/**=============================
Goal_Attack::Process
=============================**/
int Goal_Attack::Process()
{
	// if there are some ammo left it'll fire 
	if (Owner_->GetWeaponSystem()->GetTotalRemainingAmmo() > 0)
	{
		// choosing the nearest enemy
		int index = -1;
		sf::Vector2f deltaDistance;
		float d = std::numeric_limits<int>::max();
		std::size_t s = Owner_->GetEnemyFactory()->GetEntities()->size();
		for (int i = 0; i < s;++i)
		{
			if ((*Owner_->GetEnemyFactory()->GetEntities())[i]->Position_ == Owner_->Position_)
				continue;
			// calculating displacement
			deltaDistance = (*Owner_->GetEnemyFactory()->GetEntities())[i]->Position_ - Owner_->Position_;
			float newd = deltaDistance.x * deltaDistance.x + deltaDistance.y * deltaDistance.y;
			if ( newd < d)
			{
				index = i;
				d = newd;
			}
		}
		if (index == -1)
			return GOAL_STATUS_FAILED;

		// checking if the Player distance is less than the other enemies
		sf::Vector2f dPlayer = Owner_->GetGameState()->Player_.Position_ - Owner_->Position_;
		float playerDistance = dPlayer.x * dPlayer.x + dPlayer.y * dPlayer.y;

		float angle; // angle for bullets avoidance

		if (playerDistance < d && !Owner_->GetGameState()->Player_.IsDead_)
		{
			// rotating the player
			angle = Owner_->GetGameState()->Player_.Angle_;

			// predicting enemy position based on heading and speed and distance
			sf::Vector2f futureTargetPosition;
			futureTargetPosition.x = Owner_->GetGameState()->Player_.Position_.x + Owner_->GetGameState()->Player_.LateralHeading_.x * 0;
			futureTargetPosition.y = Owner_->GetGameState()->Player_.Position_.y + Owner_->GetGameState()->Player_.LateralHeading_.y * 0;

			deltaDistance = futureTargetPosition - Owner_->Position_;

			Owner_->Angle_ = atan2(deltaDistance.y,deltaDistance.x) * 180 / 3.1415926;

			// saving the target
			Owner_->Target_ = &Owner_->GetGameState()->Player_;
		}
		else 
		{
			// rotating the player
			angle = (*Owner_->GetEnemyFactory()->GetEntities())[index]->Angle_;
			// predicting enemy position based on heading and speed and distance
			sf::Vector2f futureTargetPosition;

			futureTargetPosition.x = (*Owner_->GetEnemyFactory()->GetEntities())[index]->Position_.x + (*Owner_->GetEnemyFactory()->GetEntities())[index]->Heading_.x * 0;
			futureTargetPosition.y = (*Owner_->GetEnemyFactory()->GetEntities())[index]->Position_.y + (*Owner_->GetEnemyFactory()->GetEntities())[index]->Heading_.x * 0;

			deltaDistance = futureTargetPosition - Owner_->Position_;
			Owner_->Angle_ = atan2(deltaDistance.y,deltaDistance.x) * 180 / 3.1415926;

			// saving the target
			Owner_->Target_ = (*Owner_->GetEnemyFactory()->GetEntities())[index];
		}

		// firing
		Owner_->Fire();


		StrafeTick_ += 1;
		if (StrafeTick_ >= StrafeTime_)
		{
			StrafeTick_ = 0;
			StrafeTime_ = rand() % 1000;
			if (static_cast<int>(StrafeTime_) % 2 == 0)
			{
				// calculating enemy shoot heading
				sf::Vector2f heading = sf::Vector2f(cos(angle),sin(angle));
				
				// calculating normal
				sf::Vector2f normal = sf::Vector2f(-heading.y,heading.x);

				// normalizing
				float magnitude = sqrt(normal.x * normal.x + normal.y * normal.y);
				if (magnitude > 0)
				{
					normal.x /= magnitude;
					normal.y /= magnitude;
				}

				// setting heading
				Owner_->Heading_ = normal;

			}
			else
			{
				// calculating enemy shoot heading
				sf::Vector2f heading = sf::Vector2f(cos(angle),sin(angle));
				
				// calculating normal
				sf::Vector2f normal = sf::Vector2f(heading.y,-heading.x);

				// normalizing
				float magnitude = sqrt(normal.x * normal.x + normal.y * normal.y);
				if (magnitude > 0)
				{
					normal.x /= magnitude;
					normal.y /= magnitude;
				}

				// setting heading
				Owner_->Heading_ = normal;
			}
		}
		
		return GOAL_STATUS_ACTIVE;
	}

	return GOAL_STATUS_COMPLETED;
}

/**=============================
Goal_Attack::Activate
=============================**/
void Goal_Attack::Activate()
{
	Status_ = GOAL_STATUS_ACTIVE;
}

/**=============================
Goal_Attack::Terminate
=============================**/
void Goal_Attack::Terminate()
{

}