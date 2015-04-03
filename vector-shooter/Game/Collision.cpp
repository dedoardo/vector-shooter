/**
==========================================================================
						Collision File
==========================================================================
**/


#include "Collision.h"
#include "../Graph/Map.h"
#include "../Entity/Player.h"
#include "../Entity/Enemy.h"
#include "../Entity/Entity.h"
#include "../Utility/Enum.h"
#include "../Utility/WindowInfo.h"
#include "../Weapon/Bullet.h"
#include "../Weapon/WeaponSystem.h"
#include "../Entity/EnemyFactory.h"
#include "../Game/States/GameRunState.h"
#include "../Game/EffectHandler.h"

#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>

/**=============================
QuadTree::QuadTree
=============================**/
QuadTree::QuadTree(float x,float y,float width,float height,int level,int maxLevel):
	X_(x),
	Y_(y),
	Width_(width),
	Height_(height),
	Level_(level),
	MaxLevel_(maxLevel)
{
	if (Level_ == MaxLevel_)
		return;

	LeftUp_ = new QuadTree(X_,Y_,Width_ / 2.0f , Height_ / 2.0f, Level_ +1,MaxLevel_);
	RightUp_ = new QuadTree(X_ + Width_ / 2.0f,Y_, Width_ / 2.0f,Height_ /2.0f,Level_ +1,MaxLevel_);
	LeftDown_ = new QuadTree(X_, Y_ + Height_ / 2.0f,Width_ / 2.0f,Height_ / 2.0f,Level_ +1,MaxLevel_);
	RightDown_ = new QuadTree(X_ + Width_ / 2.0f,Y_ + Height_ / 2.0f,Width_ / 2.0f,Height_ / 2.0f,Level_ +1,MaxLevel_);

}
/**=============================
QuadTree::~QuadTree
=============================**/
QuadTree::~QuadTree()
{
	if (Level_ != MaxLevel_)
	{
		delete LeftUp_;
		delete LeftDown_;
		delete RightUp_;
		delete RightDown_;
	}
}

/**=============================
QuadTree::AddEntity
=============================**/
void QuadTree::AddEntity(Entity* entity)
{
	if (Level_ == MaxLevel_)
	{
		Entities_.push_back(entity);
		return;
	}

	if (Contains(LeftUp_,entity))
	{
		LeftUp_->AddEntity(entity);
		return;
	}
	else if (Contains(RightUp_,entity))
	{
		RightUp_->AddEntity(entity);
		return;
	}
	else if (Contains(LeftDown_,entity))
	{
		LeftDown_->AddEntity(entity);
		return;
	}
	else if (Contains(RightDown_,entity))
	{
		RightDown_->AddEntity(entity);
		return;
	}

	if (Contains(this,entity))
	{
		Entities_.push_back(entity);
	}

}

/**=============================
QuadTree::AddFixedEntity
=============================**/
void QuadTree::AddFixedEntity(Entity* entity)
{
	if (Level_ == MaxLevel_)
	{
		FixedEntities_.push_back(entity);
		return;
	}

	if (Contains(LeftUp_,entity))
	{
		LeftUp_->AddFixedEntity(entity);
		return;
	}
	else if (Contains(RightUp_,entity))
	{
		RightUp_->AddFixedEntity(entity);
		return;
	}
	else if (Contains(LeftDown_,entity))
	{
		LeftDown_->AddFixedEntity(entity);
		return;
	}
	else if (Contains(RightDown_,entity))
	{
		RightDown_->AddFixedEntity(entity);
		return;
	}

	if (Contains(this,entity))
	{
		FixedEntities_.push_back(entity);
	}

}

/**=============================
QuadTree::GetEntitiesAt
=============================**/
std::vector<Entity*> QuadTree::GetEntitiesAt(float x,float y)
{
	if (Level_ == MaxLevel_)
	{
		return Entities_;
	}

	std::vector<Entity*> r1,r2;
	if (!Entities_.empty())
	{
		r1 = Entities_;
	}

	if (x >= X_ + Width_ / 2.0f && x < X_ + Width_)
	{
		if (y >= Y_ + Height_ / 2.0f && y <= Y_ + Height_)
		{
			r2 = RightDown_->GetEntitiesAt(x,y);
			r1.insert(r1.end(),r2.begin(),r2.end());
			return r1;
		}
		else if (y >= Y_ && y < Y_ + Height_)
		{
			r2 = RightUp_->GetEntitiesAt(x,y);
			r1.insert(r1.end(),r2.begin(),r2.end());
			return r1;
		}
	}
	else if ( x >= X_ && x <= X_ + Width_)
	{
		if (y >= Y_ + Height_ / 2.0f && y <= Y_ + Height_)
		{
			r2 = LeftDown_->GetEntitiesAt(x,y);
			r1.insert(r1.end(),r2.begin(),r2.end());
			return r1;
		}
		else if (y >= Y_ && y < Y_ + Height_)
		{
			r2 = LeftUp_->GetEntitiesAt(x,y);
			r1.insert(r1.end(),r2.begin(),r2.end());
			return r1;
		}
	}
	return r1;

}

/**=============================
QuadTree::GetFixedEntitiesAt
=============================**/

std::vector<Entity*> QuadTree::GetFixedEntitiesAt(float x,float y)
{
	if (Level_ == MaxLevel_)
	{
		return FixedEntities_;
	}

	std::vector<Entity*> r1,r2;
	if (!FixedEntities_.empty())
	{
		r1 = FixedEntities_;
	}

	if (x >= X_ + Width_ / 2.0f && x < X_ + Width_)
	{
		if (y >= Y_ + Height_ / 2.0f && y <= Y_ + Height_)
		{
			r2 = RightDown_->GetFixedEntitiesAt(x,y);
			r1.insert(r1.end(),r2.begin(),r2.end());
			return r1;
		}
		else if (y >= Y_ && y < Y_ + Height_)
		{
			r2 = RightUp_->GetFixedEntitiesAt(x,y);
			r1.insert(r1.end(),r2.begin(),r2.end());
			return r1;
		}
	}
	else if ( x >= X_ && x <= X_ + Width_)
	{
		if (y >= Y_ + Height_ / 2.0f && y <= Y_ + Height_)
		{
			r2 = LeftDown_->GetFixedEntitiesAt(x,y);
			r1.insert(r1.end(),r2.begin(),r2.end());
			return r1;
		}
		else if (y >= Y_ && y < Y_ + Height_)
		{
			r2 = LeftUp_->GetFixedEntitiesAt(x,y);
			r1.insert(r1.end(),r2.begin(),r2.end());
			return r1;
		}
	}
	return r1;
}

/**=============================
QuadTree::Contains
=============================**/
bool QuadTree::Contains(QuadTree* child,Entity* entity)
{
	if (entity->Position_.x >= child->X_ &&
		entity->Position_.y >= child->Y_ &&
		entity->Position_.x + entity->Width_ <= child->X_ + child->Width_ &&
		entity->Position_.y + entity->Height_<= child->Y_ + child->Height_)
		return true;
	return false;
}

/**=============================
QuadTree::Clear
=============================**/
void QuadTree::Clear()
{
	if (!Entities_.empty())
		Entities_.clear();
	if (Level_ != MaxLevel_)
	{
		LeftDown_->Clear();
		LeftUp_->Clear();
		RightUp_->Clear();
		RightDown_->Clear();
	}
	else 
	{
		Entities_.clear();
		return;
	}
}

/**=============================
CollisionHandler::~CollisionHandler()
=============================**/
CollisionHandler::~CollisionHandler()
{
	delete QuadTree_;
}

/**=============================
CollisionHandler::Init
=============================**/
void CollisionHandler::Init(TiledMap& map,Player& player,EnemyFactory& factory,GameRunState& state)
{
	ActiveMap_ = &map;
	Player_ = &player;
	EnemyFactory_ = &factory;
	GameRunState_ = &state;

	// Constructing quadtree
	QuadTree_ = new QuadTree(0,0,ActiveMap_->GetSize().x,ActiveMap_->GetSize().y,0,2);

	// Pushing all the fixed entities in the quadtree
	std::size_t s = ActiveMap_->GetWalls()->size();
	for (std::size_t i = 0; i < s; ++i)
	{
		QuadTree_->AddFixedEntity(&(*ActiveMap_->GetWalls())[i]); // adding the entity
	}
}

/**=============================
CollisionHandler::Update
=============================**/
void CollisionHandler::Update(sf::RenderWindow& window)
{
	// Pushing all bullets in the quadtree
	std::size_t s = EnemyFactory_->GetEntities()->size();
	for (int i = 0; i < s;++i)
	{
		std::size_t bulletSize = (*EnemyFactory_->GetEntities())[i]->GetWeaponSystem()->GetBullets()->size();
		for (int j = 0; j < bulletSize;++j)
		{
			QuadTree_->AddEntity(&(*(*EnemyFactory_->GetEntities())[i]->GetWeaponSystem()->GetBullets())[j]);
		}
	}

	// pushing player bullets
	s = Player_->GetWeaponSystem()->GetBullets()->size();
	for (int i = 0; i < s;++i)
	{
		QuadTree_->AddEntity(&(*Player_->GetWeaponSystem()->GetBullets())[i]);
	}

	// Once all the elements are inserted i'm going to check collision between them
	// for the bullet to enemy collision i'm using a bounding circle around the entity
	// and check the distance with the center of the bullet ( is enough for now ) 
	// maybe i'll enhance this later
	
	// pre-initializing variable to make loop faster
	Entity* e;
	sf::Vector2f distance;
	float magnitude;

	// checking all enemies against bullets
	// to destroy the bullet i'm simply setting the speed to 0 and it'll be destroyed
	// in the next update of the weaponsystem
	s = EnemyFactory_->GetEntities()->size();
	for (int i = 0; i < s;++i)
	{
		// saving pointer to entity for ease of use
		e = (*EnemyFactory_->GetEntities())[i];

		// getting the bullets in his quadtree
		std::vector<Entity*> bullets = QuadTree_->GetEntitiesAt(e->Position_.x,e->Position_.y);

		// checking collision
		std::size_t bulletSize = bullets.size();
		for (int j = 0; j < bulletSize;++j)
		{
			// calculating distance -> keeping it squared to avoid too many calculations
			distance = (bullets[j]->Position_ + sf::Vector2f(bullets[j]->Width_/2,bullets[j]->Height_ /2)) - e->Position_;
			magnitude = distance.x * distance.x + distance.y * distance.y;

			// checking if it's in the range of the bounding radius
			// if so i'm zeroing the speed
			if (magnitude <= (e->BoundingCircleRadius_ * e->BoundingCircleRadius_) && !e->IsDead_)
			{
				// killing the bullet
				bullets[j]->Speed_ = 0;

				// inflicting damage
				e->TakeDamage(bullets[j]->Damage_);

				// adding effect
				GameRunState_->EffectHandler_.AddExplosion(EFFECT_TEXTURE_EXPLOSION_BLUE_SMALL,e->Position_);

				if (e->HitPoints_ <= 0)
				{
					bullets[j]->Owner_->Kills_ += 1;

					// adding effect
					GameRunState_->EffectHandler_.AddExplosion(EFFECT_TEXTURE_EXPLOSION_BLUE_BIG,e->Position_);
				}

				// checking damage to other players
				if (bullets[j]->Radius_ > 1)
				{
					// playing explosion animation
					GameRunState_->EffectHandler_.AddExplosion(EFFECT_TEXTURE_EXPLOSION_ROCKET,e->Position_);
				}

			}
		}
	}

	std::vector<Entity*> bullets = QuadTree_->GetEntitiesAt(Player_->Position_.x,Player_->Position_.y);
	s = bullets.size();

	for (int i = 0; i < s;++i)
	{
		distance = (bullets[i]->Position_) - Player_->PreviousPosition_;
		magnitude = distance.x * distance.x + distance.y * distance.y;

		if (magnitude <= (Player_->BoundingCircleRadius_ * Player_->BoundingCircleRadius_) && !Player_->IsDead_)
		{
			// killing the bullet
			bullets[i]->Speed_ = 0;

			// inflicting damage
			Player_->TakeDamage(bullets[i]->Damage_);

			// adding effect
			GameRunState_->EffectHandler_.AddExplosion(EFFECT_TEXTURE_EXPLOSION_BLUE_SMALL,Player_->Position_);

			if (Player_->HitPoints_ <= 0)
			{
				// giving the kill
				bullets[i]->Owner_->Kills_ += 1;

				// adding effect
				GameRunState_->EffectHandler_.AddExplosion(EFFECT_TEXTURE_EXPLOSION_RED_BIG,Player_->Position_);
			}
		}
	}


	QuadTree_->Clear();
}


/**=============================
CollisionHandler::PlayerToWall
=============================**/
int CollisionHandler::PlayerToWall()
{
	// checking collision for entity on wall ( only when not following a pre-defined path!
	std::vector<Entity*> walls = QuadTree_->GetFixedEntitiesAt(Player_->Position_.x,Player_->Position_.y);
	std::size_t s = walls.size();

	sf::Vector2f endVertex = Player_->Position_;
	sf::Vector2f startVertex = Player_->PreviousPosition_;

	for (int i = 0; i < s;++i)
	{
			if (IsInside(startVertex,walls[i]))
			{
				// Checking for the side of collision
				sf::Vector2f topLeft = walls[i]->Position_;
				sf::Vector2f topRight = sf::Vector2f(walls[i]->Position_.x + walls[i]->Width_,walls[i]->Position_.y); 
				sf::Vector2f downRight = sf::Vector2f(walls[i]->Position_.x + walls[i]->Width_,walls[i]->Position_.y + walls[i]->Height_);
				sf::Vector2f downLeft = sf::Vector2f(walls[i]->Position_.x,walls[i]->Position_.y + walls[i]->Height_);

				if (AreIntersecting(endVertex,startVertex,topLeft,topRight) && walls[i]->TopVisible_)
					return COLLISION_TOP;
				else if (AreIntersecting(endVertex,startVertex,topRight,downRight) && walls[i]->RightVisible_)
					return COLLISION_RIGHT;
				else if (AreIntersecting(endVertex,startVertex,downRight,downLeft) && walls[i]->DownVisible_)
					return COLLISION_DOWN;
				else if (AreIntersecting(endVertex,startVertex,downLeft,topLeft) && walls[i]->LeftVisible_)
					return COLLISION_LEFT;
				else 
					return COLLISION_TRUE;

			}
		}
	// Checking if the player is going outside the map (window)
	if (Player_->PreviousPosition_.x < 0)
		return COLLISION_RIGHT;
	else if (Player_->PreviousPosition_.x > WINDOW_WIDTH)
		return COLLISION_LEFT;
	else if (Player_->PreviousPosition_.y < 0)
		return COLLISION_DOWN;
	else if (Player_->PreviousPosition_.y > WINDOW_HEIGHT)
		return COLLISION_TOP;


	return COLLISION_FALSE;
}

/**=============================
CollisionHandler::EntityToWall
=============================**/
int CollisionHandler::EntityToWall(const Entity& entity)
{
	// checking collision for entity on wall ( only when not following a pre-defined path!
	std::vector<Entity*> walls = QuadTree_->GetFixedEntitiesAt(entity.Position_.x,entity.Position_.y);
	std::size_t s = walls.size();

	sf::Vector2f startVertex = entity.Position_;
	sf::Vector2f endVertex = entity.PreviousPosition_;

	for (int i = 0; i < s;++i)
	{
		if (IsInside(startVertex,walls[i]))
		{
			// Checking for the side of collision
			sf::Vector2f topLeft = walls[i]->Position_;
			sf::Vector2f topRight = sf::Vector2f(walls[i]->Position_.x + walls[i]->Width_,walls[i]->Position_.y); 
			sf::Vector2f downRight = sf::Vector2f(walls[i]->Position_.x + walls[i]->Width_,walls[i]->Position_.y + walls[i]->Height_);
			sf::Vector2f downLeft = sf::Vector2f(walls[i]->Position_.x,walls[i]->Position_.y + walls[i]->Height_);

			if (AreIntersecting(endVertex,startVertex,topLeft,topRight) && walls[i]->TopVisible_)
				return COLLISION_TOP;
			else if (AreIntersecting(endVertex,startVertex,topRight,downRight) && walls[i]->RightVisible_)
				return COLLISION_RIGHT;
			else if (AreIntersecting(endVertex,startVertex,downRight,downLeft) && walls[i]->DownVisible_)
				return COLLISION_DOWN;
			else if (AreIntersecting(endVertex,startVertex,downLeft,topLeft) && walls[i]->LeftVisible_)
				return COLLISION_LEFT;
			else 
				return COLLISION_TRUE;

			}
	}
	
	// Checking if the player is going outside the map (window)
	if (entity.PreviousPosition_.x < 0)
		return COLLISION_RIGHT;
	else if (entity.PreviousPosition_.x > WINDOW_WIDTH)
		return COLLISION_LEFT;
	else if (entity.PreviousPosition_.y < 0)
		return COLLISION_DOWN;
	else if (entity.PreviousPosition_.y > WINDOW_HEIGHT)
		return COLLISION_TOP;

	// otherwise return collisione false
	return COLLISION_FALSE;
}

/**=============================
CollisionHandler::BulletToWall
=============================**/
int CollisionHandler::BulletToWall(Bullet& bullet,sf::RenderWindow& window)
{
	// getting data from quadtree
	std::vector<Entity*> walls = QuadTree_->GetFixedEntitiesAt(bullet.GetPosition().x,bullet.GetPosition().y);

	// creating ray from old position to new one
	sf::Vector2f startVertex = bullet.GetPosition();
	sf::Vector2f endVertex = bullet.GetPreviousPosition();

	std::size_t s = walls.size();
	for (int i = 0; i < s; ++i)
	{
		// checking if it is inside first
		if (IsInside(bullet.GetPosition(),walls[i]))
		{
			// making explosion
			GameRunState_->EffectHandler_.AddExplosion(EFFECT_TEXTURE_EXPLOSION_RED_SMALL,bullet.Position_);

			if (bullet.Radius_ > 1)
			{
				// playing explosion animation
				GameRunState_->EffectHandler_.AddExplosion(EFFECT_TEXTURE_EXPLOSION_ROCKET,bullet.Position_);
			}
			// then removing it 
			return COLLISION_TRUE;
		}
	}
	return COLLISION_FALSE;
}