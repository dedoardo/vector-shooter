/**
==========================================================================
						PathFinder
==========================================================================
**/ 

#include "PathFinder.h"
#include "../Entity/Enemy.h"
#include "../Utility/Enum.h"

#include <iostream>
#include <concurrent_priority_queue.h>
#include <random>

/**======================================================================
============= STRUCTURES FOR PRIORITY QUEUE =============================
======================================================================**/
struct NodeCost
{
	NodeCost(int i,int c):index(i),cost(c){};
	int index;
	int cost; // or costF for the A* algorithm
	int costG;
	int costH;
	int parent; // parent index
};

struct compare
{
	bool operator() (const NodeCost& l,const NodeCost& r)
	{
		return l.cost > r.cost;
	}

};

struct compare2
{
	bool operator() (const NodeCost& l,const NodeCost& r)
	{
		return l.costH > r.costH;
	}
};

/**
==========================
GetManhattanDistance
==========================
**/
template <typename T>
float GetManhattanDistance(const sf::Vector2<T>& v1,const sf::Vector2<T>& v2)
{
	sf::Vector2f d = v2 - v1;
	return sqrt(d.x * d.x + d.y * d.y);
}

/**=============================
PathFinder::PathFinder
=============================**/
PathFinder::PathFinder(MapHandler& map,Enemy& e)
{
	MapHandler_ = &map;
	Owner_  = &e;
}

/**=============================
PathFinder::FindNearestItem
=============================**/
void PathFinder::FindNearestItem(int itemType)
{
	TiledMap* map = MapHandler_->GetActiveMap();

	// Using Dijkstra algorithm to find the closest item
	std::vector<int> CostToNode(map->Nodes_.size(),NODE_NO_COST_ASSIGNED);
	std::priority_queue<NodeCost,std::vector<NodeCost>,compare> pq;
	std::vector<int> visitedNodes(map->Nodes_.size(),NODE_UNVISITED);
	std::vector<MapNode*> searchTree(map->Nodes_.size(),nullptr);

	// calculating enemy nearest node and using it as dummy node
	int sourceNode = GetNodeNearestToEnemy();

	pq.push(NodeCost(sourceNode,0));

	CostToNode[sourceNode] = 0; 
	visitedNodes[sourceNode] = sourceNode;

	int endIndex;

	while (!pq.empty())
	{
		// getting the next node
		int nextNodeIndex = pq.top().index;

		// removing it
		pq.pop();

		// adding it to searchtree
		searchTree[nextNodeIndex] = &(map->Nodes_[nextNodeIndex]);

		// if it's the target i'm terminating
		if (itemType == searchTree[nextNodeIndex]->GetType())
		{
			endIndex = nextNodeIndex;
			break;
		}
		// else getting all the parents
		std::size_t neighborSize = map->Nodes_[nextNodeIndex].GetEdges()->size();
		for (int i = 0; i < neighborSize;++i)
		{
			// calculating the new cose
			int altCost = CostToNode[nextNodeIndex] + (*map->Nodes_[nextNodeIndex].GetEdges())[i].Cost();
			
			int edgeToIndex = (*map->Nodes_[nextNodeIndex].GetEdges())[i].To();

			// if the node was never visited
			if (visitedNodes[edgeToIndex] == NODE_UNVISITED)
			{
				// noting the cost to the index
				CostToNode[edgeToIndex] = altCost;

				visitedNodes[edgeToIndex] = nextNodeIndex;

				pq.push(NodeCost(edgeToIndex,altCost));
			}
			else if (altCost < CostToNode[edgeToIndex])
			{
				// edge relaxation
				CostToNode[edgeToIndex] = altCost;

				// noting the new parent
				visitedNodes[edgeToIndex] = nextNodeIndex;

			}

		}
	}

	// Recreating position from visited nodes
	std::vector<sf::Vector2f> path;

	int n = endIndex;

	while (true)
	{
		// pushing element in path
		path.push_back(map->Nodes_[n].GetPosition());

		// getting next one
		n = visitedNodes[n];

		if (n == sourceNode)
		{
			path.push_back(map->Nodes_[n].GetPosition());
			break;
		}

	}

	Owner_->PathToFollow_ = path;

}

/**=============================
PathFinder::FindPathTo
=============================**/
void PathFinder::FindPathTo(int nodeIndex)
{

	int result = false;

	TiledMap* map = MapHandler_->GetActiveMap();

	// using A* for this kind of search
	std::priority_queue<NodeCost,std::vector<NodeCost>,compare2> pq;
	std::vector<int> openList(map->Nodes_.size(),NODE_DOESNT_EXIST);
	std::vector<NodeCost> closedList(map->Nodes_.size(),NodeCost(NODE_DOESNT_EXIST,0));
	std::vector<float> CostToNode(map->Nodes_.size(),NODE_NO_COST_ASSIGNED);

	// Getting position
	int sourceIndex = GetNodeNearestToEnemy();

	// precalculating end position
	sf::Vector2f endPosition = map->Nodes_[nodeIndex].GetPosition();

	// pushing dummy node
	NodeCost startNode(sourceIndex,0);
	startNode.parent = sourceIndex;
	startNode.costG = 0;
	startNode.costH = GetManhattanDistance(map->Nodes_[sourceIndex].GetPosition(),map->Nodes_[nodeIndex].GetPosition());
	
	pq.push(startNode);

	while (!pq.empty())
	{
		// getting next node
		NodeCost node = pq.top();

		// moving it to closedlist
		closedList[node.index] = node;
		
		// removing from openlist
		openList[node.index] = NODE_DOESNT_EXIST;

		// removing it from queue
		pq.pop();

		// checking if last node
		if (node.index == nodeIndex)
		{
			result = true;
			break;
		}
		// getting adjacents
		std::size_t s = map->Nodes_[node.index].GetEdges()->size();
		for (int i = 0; i < s; ++i)
		{
			// taking adjacent
			MapEdge* adj = &(*map->Nodes_[node.index].GetEdges())[i];

			// calculating cost
			float CostG = CostToNode[node.index] + adj->Cost();
			float CostH = GetManhattanDistance(map->Nodes_[adj->To()].GetPosition(),endPosition);

			// if it's in closed list let's continue
			if (closedList[adj->To()].index != NODE_DOESNT_EXIST)
				continue;
			else if (openList[adj->To()] == NODE_DOESNT_EXIST)
			{
				// if it's not on the openlist
				NodeCost n(adj->To(),CostH + CostG);
				n.parent = node.index;
				n.costG = CostG;
				n.costH = CostH;

				// Adding it to queue and openlist
				openList[adj->To()] = adj->To();
				pq.push(n);
				
				// updating cost
				CostToNode[adj->To()] = CostG;
			}
			else if(openList[adj->To()] != NODE_DOESNT_EXIST)
			{
				if (CostG < CostToNode[adj->To()])
				{
					// Edge relaxations comes into play
					// changing the parent
					closedList[adj->To()].index = node.index;
					
					// saving new cost
					CostToNode[adj->To()] = CostG;

					// recalculating cost
					closedList[adj->To()].costG = CostG;
					closedList[adj->To()].cost = CostG + closedList[adj->To()].costH;
				}
			}
		}

	}

	if (result == true ) // if path was found do all calculations
	{

		// rebuilding path from closedlist
		std::vector<sf::Vector2f> path;

		int	n = nodeIndex;

		while (true)
		{
			// pushing element in path
			path.push_back(map->Nodes_[n].GetPosition());

			// getting next one
			n = closedList[n].parent;

			if (n == startNode.index)
			{
				path.push_back(map->Nodes_[n].GetPosition());
				break;
			}
		}

		Owner_->PathToFollow_ = path;
	}
}

/**=============================
PathFinder::FindPathTo
=============================**/
void PathFinder::FindPathTo(const sf::Vector2f& destination)
{

}

/**=============================
PathFinder::GetNodeNearestToEnemy
=============================**/
int PathFinder::GetNodeNearestToEnemy()
{
	TiledMap* map = MapHandler_->GetActiveMap();
	std::size_t s = map->Nodes_.size();

	// rounding the enemy position
	sf::Vector2f roundedPosition;
	roundedPosition.x = Owner_->Position_.x - (static_cast<int>(Owner_->Position_.x) % map->CellWidth_);
	roundedPosition.y = Owner_->Position_.y - (static_cast<int>(Owner_->Position_.y) % map->CellHeight_);
	for (int i = 0; i < s ;++i)
	{
		if (map->Nodes_[i].GetPosition() == roundedPosition)
		{
			return map->Nodes_[i].GetIndex();
		}
	}

	// if it's not found than getting the nearest one 
	// the function is not supposed to get here, just
	// for more safety
	for (int i = 0; i < s;++i)
	{
		if ( Owner_->LastNodePosition_ == map->Nodes_[i].GetPosition())
		{
			return map->Nodes_[i].GetIndex();
		}
	}
}

/**=============================
PathFinder::FindRandomValidPoint
=============================**/
int PathFinder::FindRandomValidPoint()
{
	int id =  rand() % MapHandler_->GetActiveMap()->Nodes_.size();
	if (id > 0)
		id -= 1;
	return id;

}

/**============================
PathFindef::FindRandomValidPosition
============================**/
sf::Vector2f PathFinder::FindRandomValidPosition()
{
	int id = FindRandomValidPoint();
	return (*MapHandler_->GetActiveMap()->GetNodes())[id].GetPosition();
}

/**=============================
PathFinder::IsPathObstructed
=============================**/
bool PathFinder::IsPathObstructed(const sf::Vector2f& from,const sf::Vector2f& to)
{
	return MapHandler_->ActiveMap_->IsPathObstructed(from,to);
}