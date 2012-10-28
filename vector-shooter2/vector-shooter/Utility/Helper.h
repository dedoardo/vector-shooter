

#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED
// Helper functions
#include "../Graph/MapElement.h"

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