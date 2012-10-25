

#ifndef GRAPHSEARCH_H_INCLUDED
#define GRAPHSEARCH_H_INCLUDED

#include "Map.h"

class GraphSearch
{
public :
	GraphSearch();
	GraphSearch(Map&);

	void LoadNewMap(Map&);

	void SearchCurrentMap(int,int,int,std::vector<int>&); // int represent the type of search and the other is the target,the vector is the path to store the nodes in

private :
	// different searches
	void DFSearch(int,int,std::vector<int>&);
	void BFSearch(int,int,std::vector<int>&);
	void DijkstraSearch(int,int,std::vector<int>&);
	void AStarSearch(int,int,std::vector<int>&);
	void FindConnectingNodes(int,std::vector<MapEdge*>&);

	std::vector<MapEdge>*   pEdges_;
	std::vector<MapNode>*	pNodes_;
	Map*					pMap_;
};

#endif