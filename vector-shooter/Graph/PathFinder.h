/**
==========================================================================
						Pathfinder
==========================================================================
**/ 


#ifndef PATH_FINDER_H_INCLUDED
#define PATH_FINDER_H_INCLUDED

#include "../Graph/Map.h"

class Enemy;

class PathFinder
{
public :
	PathFinder(){};
	PathFinder(MapHandler&,Enemy&);

	// Search functions
	void FindNearestItem(int);
	void FindPathTo(int);
	void FindPathTo(const sf::Vector2f&);

	bool IsPathObstructed(const sf::Vector2f&,const sf::Vector2f&);

	int FindRandomValidPoint();
	sf::Vector2f FindRandomValidPosition();

	int GetNodeNearestToEnemy();

private :
	MapHandler*			MapHandler_;
	Enemy*				Owner_;
};

#endif