/**
==========================================================================
						Collision File
==========================================================================
**/


#include "Collision.h"
#include "../Graph/Map.h"
#include "../Entity/Player.h"
#include "../Utility/Enum.h"
#include "../Utility/WindowInfo.h"

#include <iostream>

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
	delete LeftUp_;
	delete LeftDown_;
	delete RightUp_;
	delete RightDown_;
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
	std::vector<Entity*> returnOne,returnTwo;
	if (!Entities_.empty())
	{
		returnOne = Entities_;
	}

	if (x >= X_ && x < X_ + Width_ / 2.0f)
	{
		if ( y >= Y_ && y < Y_ + Height_ / 2.0f)
		{
			returnTwo = LeftUp_->GetEntitiesAt(x,y);
			returnOne.insert(returnOne.end(),returnTwo.begin(),returnTwo.end());
			return returnOne;
		}
		else if (y >= Y_ + Height_ / 2.0f && y <= Y_ + Height_)
		{
			returnTwo = LeftDown_->GetEntitiesAt(x,y);
			returnOne.insert(returnOne.end(),returnTwo.begin(),returnTwo.end());
			return returnOne;
		}
	}
	else if (x >= X_ + Width_ / 2.0f && x <= X_ + Width_)
	{
		if ( y >= Y_ && y < Y_ + Height_ / 2.0f)
		{
			returnTwo = RightUp_->GetEntitiesAt(x,y);
			returnOne.insert(returnOne.end(),returnTwo.begin(),returnTwo.end());
			return returnOne;
		}
		else if (y >= Y_ + Height_ / 2.0f && y <= Y_ + Height_)
		{
			returnTwo = RightDown_->GetEntitiesAt(x,y);
			returnOne.insert(returnOne.end(),returnTwo.begin(),returnTwo.end());
			return returnOne;
		}
	}

	return returnOne;

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
	std::vector<Entity*> returnOne,returnTwo;
	if (!Entities_.empty())
	{
		returnOne = FixedEntities_;
	}

	if (x >= X_ && x < X_ + Width_ / 2.0f)
	{
		if ( y >= Y_ && y < Y_ + Height_ / 2.0f)
		{
			returnTwo = LeftUp_->GetFixedEntitiesAt(x,y);
			returnOne.insert(returnOne.end(),returnTwo.begin(),returnTwo.end());
			return returnOne;
		}
		else if (y >= Y_ + Height_ / 2.0f && y <= Y_ + Height_)
		{
			returnTwo = LeftDown_->GetFixedEntitiesAt(x,y);
			returnOne.insert(returnOne.end(),returnTwo.begin(),returnTwo.end());
			return returnOne;
		}
	}
	else if (x >= X_ + Width_ / 2.0f && x <= X_ + Width_)
	{
		if ( y >= Y_ && y < Y_ + Height_ / 2.0f)
		{
			returnTwo = RightUp_->GetFixedEntitiesAt(x,y);
			returnOne.insert(returnOne.end(),returnTwo.begin(),returnTwo.end());
			return returnOne;
		}
		else if (y >= Y_ + Height_ / 2.0f && y <= Y_ + Height_)
		{
			returnTwo = RightDown_->GetFixedEntitiesAt(x,y);
			returnOne.insert(returnOne.end(),returnTwo.begin(),returnTwo.end());
			return returnOne;
		}
	}

	return returnOne;

}

/**=============================
QuadTree::Contains
=============================**/
bool QuadTree::Contains(QuadTree* child,Entity* entity)
{
	if (entity->Position_.x < child->X_ ||
		entity->Position_.y < child->Y_ ||
		entity->Position_.x > child->X_ + child->Width_ ||
		entity->Position_.y > child->Y_ + child->Height_ ||
		entity->Position_.x + entity->Width_ < child->X_ ||
		entity->Position_.y + entity->Height_ < child->Y_ ||
		entity->Position_.x + entity->Width_ > child->X_  + child->Width_||
		entity->Position_.y + entity->Height_ > child->Y_ + child->Width_ )
		return false;
	return true;
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
CollisionHandler::Init
=============================**/
void CollisionHandler::Init(TiledMap& map,Player& player)
{
	ActiveMap_ = &map;
	Player_ = &player;

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
}


/**=============================
CollisionHandler::PlayerToWall
=============================**/
int CollisionHandler::PlayerToWall()
{
	// For the wall collision i'm doing a bruteforce
	std::size_t s = ActiveMap_->GetWalls()->size();
	std::size_t vSize = Player_->GetUpdatedVertices()->size();
	std::vector<sf::Vector2f>* vertices = Player_->GetUpdatedVertices();

	sf::Vector2f startVertex = (*vertices)[0] + Player_->PreviousPosition_; 
	sf::Vector2f endVertex = (*vertices)[0] + Player_->Position_;
	sf::Vector2f offset = endVertex - startVertex;
	endVertex += offset +  offset;

	std::vector<Entity>* walls = ActiveMap_->GetWalls();

	for (std::size_t i = 0; i < s; ++i)
	{
		for (std::size_t j = 0; j < vSize;++j)
		{
			if (IsInside(startVertex,&(*walls)[i]))
			{
				// Checking for the side of collision
				sf::Vector2f topLeft = (*walls)[i].Position_;
				sf::Vector2f topRight = sf::Vector2f((*walls)[i].Position_.x + (*walls)[i].Width_,(*walls)[i].Position_.y); 
				sf::Vector2f downRight = sf::Vector2f((*walls)[i].Position_.x + (*walls)[i].Width_,(*walls)[i].Position_.y + (*walls)[i].Height_);
				sf::Vector2f downLeft = sf::Vector2f((*walls)[i].Position_.x,(*walls)[i].Position_.y + (*walls)[i].Height_);

				if (AreIntersecting(endVertex,startVertex,topLeft,topRight) && (*walls)[i].TopVisible_)
					return COLLISION_TOP;
				else if (AreIntersecting(endVertex,startVertex,topRight,downRight) && (*walls)[i].RightVisible_)
					return COLLISION_RIGHT;
				else if (AreIntersecting(endVertex,startVertex,downRight,downLeft) && (*walls)[i].DownVisible_)
					return COLLISION_DOWN;
				else if (AreIntersecting(endVertex,startVertex,downLeft,topLeft) && (*walls)[i].LeftVisible_)
					return COLLISION_LEFT;
				else 
					return COLLISION_TRUE;

			}
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