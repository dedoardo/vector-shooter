/**
==========================================================================
							Goal Think source
==========================================================================
**/


#include "Goal_Think.h"
#include "Goal_Explore.h"
#include "Goal_Attack.h"
#include "Goal_SeekTarget.h"

#include "../../Utility/Enum.h"
#include "../../Weapon/WeaponSystem.h"
#include "../Enemy.h"
#include "../EnemyFactory.h"
#include "../../Game/States/GameRunState.h"

/**=============================
Goal_Think::Goal_Think
=============================**/
Goal_Think::Goal_Think(Enemy& e)
{
	Owner_ = &e;
	// On this initialization all the subgoals will be initialized
	StrategicGoals_.push_back(new Goal_Explore(*Owner_));
	StrategicGoals_.push_back(new Goal_Attack(*Owner_));
	StrategicGoals_.push_back(new Goal_SeekTarget(*Owner_));
	LastActiveGoal_ = STRATEGIC_GOAL_EXPLORE;

	ThinkTime_ = 3;
	ThinkTick_ = ThinkTime_;


}

/**=============================
Goal_Think::Evaluate
=============================**/
void Goal_Think::Evaluate()
{
	// ticking the AI once every 3 renders
	++ThinkTick_;
	if (ThinkTick_ >= ThinkTime_)
	{
		ThinkTick_ = 0;
		// Chooses between all the strategic goals
		float ExploreValue = EvaluateExplore();
		float AttackValue = EvaluateAttack();
		float HealthValue = EvaluateGetHealth();
		float WeaponValue = EvaluateGetWeapon();
	
		// checking if last goal was attack
		// if it was and the target is not dead yet, i'm using a goal_seek
		if (ExploreValue > AttackValue && ExploreValue > HealthValue && ExploreValue > WeaponValue)
		{
			StrategicGoals_[STRATEGIC_GOAL_EXPLORE]->Process();
			LastActiveGoal_ = STRATEGIC_GOAL_EXPLORE;
		}
		else if (AttackValue > ExploreValue && AttackValue > HealthValue && AttackValue > WeaponValue)
		{	
			StrategicGoals_[STRATEGIC_GOAL_ATTACK]->Process();
			LastActiveGoal_ = STRATEGIC_GOAL_ATTACK;
		}
		else if (HealthValue > ExploreValue && HealthValue > AttackValue && HealthValue > WeaponValue)
		{
			StrategicGoals_[STRATEGIC_GOAL_GET_HEALTH]->Process();
			LastActiveGoal_ = STRATEGIC_GOAL_GET_HEALTH;
		}
		else if (WeaponValue > ExploreValue && WeaponValue > AttackValue && WeaponValue > HealthValue)
		{
			StrategicGoals_[STRATEGIC_GOAL_GET_WEAPON]->Process();
			LastActiveGoal_ = STRATEGIC_GOAL_GET_WEAPON;
		}
	}
}

/**=============================
Goal_Think::EvaluateExplore
=============================**/
float Goal_Think::EvaluateExplore()
{
	return 0.1; // fixed value for exploration
}

/**=============================
Goal_Think::EvaluateAttack
=============================**/
float Goal_Think::EvaluateAttack()
{
	if (Owner_->IsDead_)
		return 0;
	std::size_t s = Owner_->EnemyFactory_->GetEntities()->size();
	int nearestEnemyIndex = -1;
	
	float distance = std::numeric_limits<int>::max();

	for (int i = 0; i < s;++i)
	{
		Enemy* e = (*Owner_->EnemyFactory_->GetEntities())[i];

		sf::Vector2f d = e->Position_ - Owner_->Position_;
		if (Owner_->Position_ != (*Owner_->EnemyFactory_->GetEntities())[i]->Position_)
		{
			float newdistance = sqrt(d.x * d.x + d.y * d.y);
			if (newdistance < distance && !e->IsDead_ && !Owner_->IsPathObstructed(Owner_->Position_,(*Owner_->EnemyFactory_->GetEntities())[i]->Position_) )
			{
				distance = newdistance;
				nearestEnemyIndex = i;
			}
		}
	}

	// checking distance
	if (nearestEnemyIndex > -1 && distance <= 100)
	{
		return 0.9;
	}

	// distance from player
	sf::Vector2f d = Owner_->GetGameState()->Player_.Position_ - Owner_->Position_;
	float playerDistance = sqrt(d.x * d.x + d.y * d.y);

	if (playerDistance <= 100 && !Owner_->IsPathObstructed(Owner_->Position_,Owner_->GetGameState()->Player_.Position_))
	{
		return 0.9;
	}

	return 0;
}

/**=============================
Goal_Think::EvaluateGetHealth
=============================**/
float Goal_Think::EvaluateGetHealth()
{
	return 0;
}

/**=============================
Goal_Think::EvaluateGetWeapon
=============================**/
float Goal_Think::EvaluateGetWeapon()
{
	int ammoLeft = Owner_->WeaponSystem_->GetTotalRemainingAmmo();
	return 0;
}

/**=============================
Goal_Think::~Goal_Think
=============================**/
Goal_Think::~Goal_Think()
{
	while (!StrategicGoals_.empty())
	{
		delete StrategicGoals_.back();
		StrategicGoals_.pop_back();
	}
}