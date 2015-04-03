

#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED
// Helper functions
#include "../Graph/MapElement.h"
#include <SFML/System/Vector2.hpp>

/**
==========================
Round
==========================
**/
int round(float number)
{
	if (number + 0.5 >= floor(number) +1)
		return ceil(number);
	return floor(number);
}


/**
==========================
GetSquareMagnitude
==========================
**/
template <typename T>
float GetSquaredMagnitude(const sf::Vector2<T>& v1,const sf::Vector2<T>& v2)
{
	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	return sqrt(dx * dx + dy * dy);
}

/**
==========================
NodeExists()
==========================
**/

template <typename T>
bool NodeExists(sf::Vector2<T>& node,std::vector<MapNode>& nodes,int& index,std::size_t nodeSize)
{
	for (int i = 0; i < nodeSize; ++i)
	{
		if (nodes[i].GetPosition() == node)
		{
			index = nodes[i].GetIndex();
			return true;
		}
	}
	return false;
}

/**
==========================
EdgeExists()
==========================
**/

bool EdgeExists(int from,int to,std::vector<MapEdge>& edges,std::size_t edgeSize)
{
	for (int i = 0; i < edgeSize; ++i)
	{
		if (edges[i].From() == from && edges[i].To() == to)
			return true;
	}
	return false;
}
#endif