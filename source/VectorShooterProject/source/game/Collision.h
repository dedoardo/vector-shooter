///////////////////////////////////////////////////////
//				GAME STATE CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#ifndef COLLISIONHANDLER_H_INCLUDED
#define COLLISIONHANDLER_H_INCLUDED

#include "../entity/Entity.h"

#include <vector>

class Player;
class MapHandler;


struct CollisionResult
{
	bool Intersect;
	bool WillIntersect;

	sf::Vector2f MTV;// minimum translation vector
};

class CollisionHandler
{
public :
	CollisionHandler();
	void						Init(Player&,MapHandler&);

	void						CheckCollision();
	void						addEntity(BaseEntity&);

private :
	bool						CheckPlayerWall();

	Player*						pPlayer_;
	MapHandler*					pMapHandler_;
	std::vector<BaseEntity*>	entities_;
};

#endif