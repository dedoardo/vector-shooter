///////////////////////////////////////////////////////
//				GAME STATE CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#include "Collision.h"
#include "../entity/Player.h"
#include "../graph/Map.h"
#include "../utility/Data.h"

#include <iostream>

#include <SFML/Graphics/ConvexShape.hpp>

CollisionHandler::CollisionHandler()
{

}

void CollisionHandler::Init(Player& player,MapHandler& mapHandler)
{
	pPlayer_ = &player;
	pMapHandler_ = &mapHandler;
}

void CollisionHandler::addEntity(BaseEntity& entity)
{
	entities_.push_back(&entity);
}

void CollisionHandler::CheckCollision()
{
	if(CheckPlayerWall())
	{
		//pPlayer_->goBack();
	}
}

bool CollisionHandler::CheckPlayerWall()
{
	CollisionResult result;
	result.Intersect = true;
	result.WillIntersect = true;

	// Getting intersecting axes

	std::vector<sf::Vector2f> playerAxes;
	std::vector<std::vector<sf::Vector2f> > wallAxes;

	for (int i = 0; i < pMapHandler_->getActiveMap()->getWalls()->size(); i++)
	{
		int pointCount = (*(*pMapHandler_->getActiveMap()).getWalls())[i].getPointCount();
		std::vector<sf::Vector2f> axes;
		for (int j = 0; j < pointCount; j++)
		{
		sf::Vector2f p1 =(*(*pMapHandler_->getActiveMap()).getWalls())[i].getPoint(j);
		sf::Vector2f p2;
		if (j == (pointCount -1))
			p2 = (*(*pMapHandler_->getActiveMap()).getWalls())[i].getPoint(0);
		else 
			p2 = (*(*pMapHandler_->getActiveMap()).getWalls())[i].getPoint(j+1);
		sf::Vector2f edge = p2 - p1;
		sf::Vector2f normal(-edge.y,edge.x);
		axes.push_back(normal);
		}
		wallAxes.push_back(axes);
	}

	// Getting the player axes
	for ( int i = 0; i < 4; i++)
	{
		sf::Vector2f p1 = Rotate((*pPlayer_->getBounds())[i],pPlayer_->getRotationAngle());
		sf::Vector2f p2;
		if (i == 3)
			p2 = Rotate((*pPlayer_->getBounds())[0],pPlayer_->getRotationAngle());
		else 
			p2 = Rotate((*pPlayer_->getBounds())[i+1],pPlayer_->getRotationAngle());
		sf::Vector2f edge = p2 - p1;
		sf::Vector2f normal(-edge.y,edge.x);
		playerAxes.push_back(normal);
	}

	for (int i = 0; i < 1; i++)
	{
		for (int s = 0; s < wallAxes[i].size();s++)
		{
		sf::Vector2f axis = wallAxes[i][s];
		Normalize(axis);
		sf::Vector2f proj1 = Project((*pPlayer_->getBounds()),axis);
		sf::Vector2f proj2 = Project((*(*pMapHandler_->getActiveMap()).getWalls())[i],axis);
		if (proj1.y < proj2.x)
			return false;
		}

		for (int s = 0; s < playerAxes.size();s++)
		{
		sf::Vector2f axis = playerAxes[s];
		Normalize(axis);
		sf::Vector2f proj1 = Project((*pPlayer_->getBounds()),axis);
		sf::Vector2f proj2 = Project((*(*pMapHandler_->getActiveMap()).getWalls())[i],axis);
		if (proj1.y < proj2.x)
			return false;
		}
	}
	
	return true;
}