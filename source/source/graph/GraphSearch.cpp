

#include "GraphSearch.h"
#include "../utility/Data.h"
#include <stack>
#include <map>
#include <iostream>
#include <queue>
#include <concurrent_priority_queue.h>

/**
============================
Compare Struct for indexed container
============================
**/
struct NodeCost
{
	NodeCost(int i,int c):index(i),cost(c){};
	int index;
	int cost;
};

struct compare
{
	bool operator() (const NodeCost& l,const NodeCost& r)
	{
		return l.cost > r.cost;
	}

};



/**
============================
GraphSearch::GraphSearch()
============================
**/
GraphSearch::GraphSearch()
{
}

/**
============================
GraphSearch::GraphSearch(const Map&)
============================
**/
GraphSearch::GraphSearch(Map& map)
{
	pEdges_ = map.getEdges();
	pNodes_ = map.getNodes();
	pMap_   = &map;
}

/**
===========================
GraphSearch::SearchCurrentMap()
===========================
**/
void GraphSearch::SearchCurrentMap(int searchType,int sourceID,int targetID,std::vector<int>& path)
{
	switch(searchType)
	{
	case path::DFS_SEARCH :
		DFSearch(targetID,sourceID,path);
		break;
	case path::BFS_SEARCH:
		BFSearch(targetID,sourceID,path);
		break;
	case path::DIJKSTRA_SEARCH :
		DijkstraSearch(targetID,sourceID,path);
		break;
	case path::A_STAR_SEARCH :
		AStarSearch(targetID,sourceID,path);
		break;
	default :
		break;
	}
}

/**
==========================
GraphSearch::FindConnectingNodes()
==========================
**/

void GraphSearch::FindConnectingNodes(int sourceID,std::vector<MapEdge*>& connectingNodes)
{
	std::vector<const MapEdge*>::size_type s = pEdges_->size();
	for (int i = 0;i < s; ++i)
	{
		if ((*pEdges_)[i].From() == sourceID)
			connectingNodes.push_back(&((*pEdges_)[i]));
	}
}

/**
==========================
GraphSearch::DFSearch()
==========================
**/

void GraphSearch::DFSearch(int sourceID,int targetID,std::vector<int>& path)
{
	// creating containers
	std::stack<const MapEdge*> stack;
	std::vector<int> visitedNodes(pNodes_->size(),node::NODE_UNVISITED); // creating the container here is quite slow
	std::vector<MapEdge*> connectingNodes;

	MapEdge dummyStart(sourceID,sourceID,10);

	stack.push(&dummyStart);

	while(!stack.empty())
	{

		const MapEdge* next = stack.top();

		stack.pop();

		// pushing the node in the path before checking if terminal node
		path.push_back(next->From());
		visitedNodes[next->To()] = node::NODE_VISITED;

		if (next->To() == targetID)
		{
			return;
		}

		connectingNodes.clear();
		std::size_t s = (*pNodes_)[next->To()].edges_.size();
		for(int i = 0;i <s;++i)
		{
			if (visitedNodes[(*pNodes_)[next->To()].edges_[i].To()] == node::NODE_UNVISITED)
				stack.push(&(*pNodes_)[next->To()].edges_[i]);
		}		

	}
}

/**
==========================
GraphSearch::BFSearch()
==========================
**/

void GraphSearch::BFSearch(int sourceID,int targetID,std::vector<int>& path)
{
	// creating containers
	std::queue<const MapEdge*> nodeQueue;
	std::vector<int>		   visitedNodes(pNodes_->size(),node::NODE_UNVISITED);
	std::vector<int>		   notCalculatedPath(pNodes_->size(),node::NO_PARENT_ASSIGNED);



	MapEdge dummyEdge(sourceID,sourceID,10.f);
	nodeQueue.push(&dummyEdge);
	// marking the node as visited to avoid infinite loop in path rebuilding
	visitedNodes[sourceID] = node::NODE_VISITED;

	while (!nodeQueue.empty())
	{
		const MapEdge* next = nodeQueue.front();

		// removing it
		nodeQueue.pop();

		// noting the parent before checking if terminal node
		notCalculatedPath[next->To()] = next->From();

		// checking if terminal node
		if (next->To() == targetID)
			break; // just breaking since i have to rebuild the path

		std::size_t s = (*pNodes_)[next->To()].edges_.size();
		for (int i = 0; i < s; ++i)
		{
			if (visitedNodes[(*pNodes_)[next->To()].edges_[i].To()] == node::NODE_UNVISITED)
			{
				// pushing it in the queue
				nodeQueue.push(&((*pNodes_)[next->To()].edges_[i]));

				// marking it as visited
				visitedNodes[(*pNodes_)[next->To()].edges_[i].To()] = node::NODE_VISITED;
			}
		}


	}

	// rebuilding path from notCalculatedPath
	bool		findNextNode = true;
	int			nextNode = targetID;

	while (findNextNode)
	{
		// getting the parent
		nextNode = notCalculatedPath[nextNode];

		// adding parentNode to path
		path.push_back(nextNode);

		// checking if last node
		if (nextNode == sourceID || nextNode == node::NO_PARENT_ASSIGNED)
			findNextNode = false;
	}

}
/**
==========================
GraphSearch::DijkstraSearch()
==========================
**/

void GraphSearch::DijkstraSearch(int sourceID,int targetID,std::vector<int>& path)
{
	std::vector<int> CostToNode(pNodes_->size(),node::NO_COST_ASSIGNED);
	std::priority_queue<NodeCost,std::vector<NodeCost>,compare> pq;
	// initializing all pointer to NULL so that i can easily check if they have been visited
	std::vector<int> visitedNodes(pNodes_->size(),node::NODE_UNVISITED);
	std::vector<MapNode*> searchTree(pNodes_->size(),NULL);

	pq.push(NodeCost(sourceID,0));

	// assigning cost from source to source 
	CostToNode[sourceID] = 0;
	visitedNodes[sourceID] = sourceID;

	while (!pq.empty())
	{
		// getting the next node
		int nextNode = pq.top().index;
		
		// removing it
		pq.pop();

		// adding it to searchtree
		searchTree[nextNode] = &((*pNodes_)[nextNode]);

		// if node is target terminating
		if (nextNode == targetID)
			break;

		// for each neighbor of the node
		std::size_t neighborSize = (*pNodes_)[nextNode].edges_.size();
		for (int i = 0; i < neighborSize;++i)
		{
			// calculating the new cost
			int altCost = CostToNode[nextNode] + (*pNodes_)[nextNode].edges_[i].Cost();

			// if neighbor has never been visited
			if (visitedNodes[(*pNodes_)[nextNode].edges_[i].To()] == node::NODE_UNVISITED)
			{
				// noting the cost to the index
				CostToNode[(*pNodes_)[nextNode].edges_[i].To()] = altCost;

				// marking the node as visited
				visitedNodes[(*pNodes_)[nextNode].edges_[i].To()] = nextNode;

				// putting it in the priority queue
				pq.push(NodeCost((*pNodes_)[nextNode].edges_[i].To(),altCost));

			}
			else if (altCost < CostToNode[(*pNodes_)[nextNode].edges_[i].To()])
			{
				// here comes edge relaxation 
				CostToNode[(*pNodes_)[nextNode].edges_[i].To()] = altCost;

				// noting the parent
				visitedNodes[(*pNodes_)[nextNode].edges_[i].To()] = nextNode;
			}
					 
		}


	}

	// rebuilding the path now
	bool		findNextNode = true;
	int			nextNode = targetID;
	
	while (findNextNode)
	{
		// adding node to path
		path.push_back(nextNode);

		// getting the parent node
		nextNode = visitedNodes[nextNode];

		// checking if terminal node
		if (nextNode == sourceID || nextNode == node::NODE_UNVISITED)
			findNextNode = false;
	}

}

/**
==========================
GraphSearch::AStarSearch()
==========================
**/

void GraphSearch::AStarSearch(int sourceID,int targetID,std::vector<int>& path)
{

}