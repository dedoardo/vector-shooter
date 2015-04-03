/**
==========================================================================
							Enemy Source
==========================================================================
**/


#include "Enemy.h"
#include "EnemyFactory.h"
#include "Goal/Goal.h"
#include "Goal/Goal_Think.h"
#include "../Weapon/WeaponSystem.h"
#include "../Game/States/GameRunState.h"

/**=============================
Enemy::Enemy
=============================**/
Enemy::Enemy(EnemyInfo info,MapHandler& map,EnemyFactory& factory,GameRunState& gameState)
{
	Position_ = info.position;
	Texture_ = info.texture;
	Aggressivity_ = info.aggressivity;
	Respawn_	= info.respawn;
	RespawnTick_ = 0;
	Speed_ = info.speed;
	Texture_ = info.texture;
	Sprite_.setTexture(*Texture_);
	Heading_ = sf::Vector2f(0,0);
	LastNodePosition_ = Position_;
	HitPoints_ = 100;

	BoundingCircleRadius_ = Texture_->getSize().x /2 * 0.9 ;

	Width_ = Texture_->getSize().x;
	Height_ = Texture_->getSize().y;
	Sprite_.setOrigin(Width_ /2,Height_ /2);

	IsDead_ = false;
	Target_ = nullptr;

	PathFinder_ = new PathFinder(map,*this);
	Goal_Think_ = new Goal_Think(*this);
	EnemyFactory_ = &factory;
	GameState_ = &gameState;

	Kills_ = 0;
}

/**=============================
Enemy::LoadWeaponSystem
=============================**/
void Enemy::LoadWeaponSystem(WeaponSystem& w)
{
	WeaponSystem_ = &w;
}

/**=============================
Enemy::Fire
=============================**/
void Enemy::Fire()
{
	WeaponSystem_->Fire();
}


/**=============================
Enemy::IsPathObstructed
=============================**/
bool Enemy::IsPathObstructed(const sf::Vector2f& from,const sf::Vector2f& to)
{
	return PathFinder_->IsPathObstructed(from,to);
}

/**=============================
Enemy::Update
=============================**/
void Enemy::Update(sf::RenderWindow* window,const sf::Time& deltaTime)
{
	if (HitPoints_ <= 0)
	{
		IsDead_ = true;
		Position_ = PathFinder_->FindRandomValidPosition();
	}
	if (!IsDead_)
	{
		// Updating AI
		Goal_Think_->Evaluate();
		// calculating movement
		PreviousPosition_ = Position_;
		Position_.x += Speed_ * Heading_.x * deltaTime.asSeconds();
		Position_.y += Speed_ * Heading_.y * deltaTime.asSeconds();

		// checking if posiiton is valid
		int result = GameState_->CollisionHandler_.EntityToWall(*this);
		if (result != COLLISION_FALSE)
		{
			switch (result)
			{
			case COLLISION_LEFT :
				{
					Position_.x = PreviousPosition_.x;
					break;
				}
			case COLLISION_RIGHT :
				{
					Position_.x = PreviousPosition_.x;
					break;
				}
			case COLLISION_TOP :
				{
					Position_.y = PreviousPosition_.y;
					break;
				}
			case COLLISION_DOWN :
				{
					Position_.y = PreviousPosition_.y;
					break;
				}
			case COLLISION_TRUE :
				{
					Position_ = PreviousPosition_;
					break;
				}
			}
		}


		// Setting updated sprite position
		Sprite_.setRotation(Angle_);
		Sprite_.setPosition(Position_);
		window->draw(Sprite_);
	}
	else 
	{
		RespawnTick_ += deltaTime.asSeconds();
		if (RespawnTick_ >= Respawn_)
		{
			Respawn();
		}
	}

	WeaponSystem_->Update(deltaTime);

}
/**=============================
Enemy::~Enemy
=============================**/
Enemy::~Enemy()
{
	delete PathFinder_;
	delete Goal_Think_;
}


/**=============================
Enemy::Respawn
=============================**/
void Enemy::Respawn()
{
	// Resetting all stats and position
	HitPoints_ = 100;
	IsDead_ = false;
	Position_ = PathFinder_->FindRandomValidPosition();
	LastNodePosition_ = Position_;
	PathToFollow_.clear();
}

/**=============================
Enemy::IncreaseAmmo
=============================**/
void Enemy::IncreaseAmmo()
{
	WeaponSystem_->IncreaseAmmo();
}