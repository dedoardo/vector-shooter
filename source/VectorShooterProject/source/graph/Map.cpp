///////////////////////////////////////////////////////
//				MAP CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#include "Map.h"
#include "../utility/Data.h"
#include "GraphSearch.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cassert>

#include <SFML/Graphics/CircleShape.hpp>

/**
==========================
isInside()
==========================
**/

bool isInside(const MapEdge& test,const std::vector<MapEdge>& edges)
{
	std::size_t edgeSize = edges.size();
	for (int i = 0; i < edgeSize; ++i)
	{
		if (test.From() == edges[i].From() && test.To() == edges[i].To())
			return true;
	}
	return false;
}

/**
==========================
NodeExists()
==========================
**/

template <typename T>
bool NodeExists(const sf::Vector2<T>& node,const std::vector<MapNode>& nodes,int& index)
{
	std::size_t nodeSize = nodes.size();
	for (int i = 0; i < nodeSize; ++i)
	{
		if (nodes[i].position_ == node)
		{
			index = nodes[i].index_;
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

bool EdgeExists(int from,int to,const std::vector<MapEdge>& edges)
{
	std::size_t edgeSize = edges.size();
	for (int i = 0; i < edgeSize; ++i)
	{
		if (edges[i].From() == from && edges[i].To() == to)
			return true;
	}
	return false;
}




/**
==========================
MapEdge::MapEdge(int,int,float)
==========================
**/

MapEdge::MapEdge(int from,int to,float cost):from_(from),to_(to),cost_(cost)
{
}


/**
==========================
Map::Map()
==========================
**/Map::Map()
{
}

/**
==========================
Map::Map(std::string)
==========================
**/

Map::Map(std::string& name)
{
	name_ = name;
}

/**
==========================
Map::FloodFill()
==========================
**/

void Map::FloodFill(std::vector<MapNode*>& remainingNodes,int& index,std::vector<sf::Vector2f> nodeTaken)
{
}

/**
==========================
Map::CreateNodes()
==========================
**/

void Map::CreateNodes()
{
	// Flood-fill algorithm for nodes and edges
	int x = 0;
	int y = 0;
	sf::Vector2f rayOrigin(-1.01,-0.99);
	sf::Vector2f floodSeed(1280,0);
	int nodeIndex = 0;

	// node seed
	MapNode seedNode(floodSeed,nodeIndex);

	nodes_.push_back(seedNode);

	std::vector<MapNode>		activeNodes;
	std::vector<sf::Vector2f>	visitedNodes;

	activeNodes.push_back(seedNode);

	MapNode nodeRight;
	MapNode nodeLeft;
	MapNode nodeDown;
	MapNode nodeUp;

	// for intersection
	int		intersectRight = 0;
	int		intersectLeft = 0;
	int		intersectUp  = 0;
	int		intersectDown = 0;

	// precalculating sizes for better performance
	std::vector<sf::ConvexShape>::size_type wallSize = walls_.size();

	while (!activeNodes.empty())
	{

		sf::Vector2f startPos = activeNodes.back().position_;
		int			 fromID = activeNodes.back().index_;
		nodeRight.position_ = (sf::Vector2f(startPos.x + 10,startPos.y));
		nodeLeft.position_ = (sf::Vector2f(startPos.x - 10,startPos.y));
		nodeDown.position_ = (sf::Vector2f(startPos.x,startPos.y + 10));
		nodeUp.position_ = (sf::Vector2f(startPos.x,startPos.y - 10));

		activeNodes.pop_back();

		// for intersection
		intersectRight = 0;
		intersectLeft = 0;
		intersectUp  = 0;
		intersectDown = 0;

		for (int i = 0; i < wallSize; ++i)
		{
			for (int j = 0; j < walls_[i].getPointCount(); ++j)
			{
				int wallIndex = j==0?walls_[i].getPointCount()-1:j-1;
				if (AreIntersecting(rayOrigin,nodeRight.position_,walls_[i].getPoint(wallIndex),walls_[i].getPoint(j)))
					intersectRight++;
				if (AreIntersecting(rayOrigin,nodeLeft.position_,walls_[i].getPoint(wallIndex),walls_[i].getPoint(j)))
					intersectLeft++;
				if (AreIntersecting(rayOrigin,nodeDown.position_,walls_[i].getPoint(wallIndex),walls_[i].getPoint(j)))
					intersectDown++;
				if (AreIntersecting(rayOrigin,nodeUp.position_,walls_[i].getPoint(wallIndex),walls_[i].getPoint(j)))
					intersectUp++;
			}
		}


		if (!isInside(nodeRight.position_,visitedNodes) && isValid(nodeRight.position_) && (intersectRight % 2) == 0)
		{
			nodeIndex++;
			nodeRight.index_ = nodeIndex;
			nodes_.push_back(nodeRight);
			activeNodes.push_back(nodes_.back());
			visitedNodes.push_back(nodeRight.position_);

		}
		if (!isInside(nodeLeft.position_,visitedNodes) && isValid(nodeLeft.position_) && (intersectLeft %2 ) == 0)
		{
			nodeIndex++;
			nodeLeft.index_ = nodeIndex;
			nodes_.push_back(nodeLeft);
			activeNodes.push_back(nodes_.back());
			visitedNodes.push_back(nodeLeft.position_);
		}
		if (!isInside(nodeDown.position_,visitedNodes) && isValid(nodeDown.position_) && (intersectDown % 2) == 0)
		{
			nodeIndex++;
			nodeDown.index_ = nodeIndex;
			nodes_.push_back(nodeDown);
			activeNodes.push_back(nodes_.back());
			visitedNodes.push_back(nodeDown.position_);
		}
		if (!isInside(nodeUp.position_,visitedNodes) && isValid(nodeUp.position_) && (intersectUp % 2) == 0)
		{
			nodeIndex++;
			nodeUp.index_ = nodeIndex;
			nodes_.push_back(nodeUp);
			activeNodes.push_back(nodes_.back());
			visitedNodes.push_back(nodeUp.position_);
		}

	}

	////// Generating all the edges

	sf::Vector2f posRight;
	sf::Vector2f posLeft;
	sf::Vector2f posDown;
	sf::Vector2f posUp;

	std::vector<MapNode>::size_type s = nodes_.size();
	int edgeIndex = 0; // edge Index for later researches
	for (int i = 0; i < s; ++i)
	{
		posRight.x = nodes_[i].position_.x + 10;
		posRight.y = nodes_[i].position_.y;

		posLeft.x = nodes_[i].position_.x - 10;
		posLeft.y = nodes_[i].position_.y;

		posDown.x = nodes_[i].position_.x;
		posDown.y = nodes_[i].position_.y + 10;

		posUp.x = nodes_[i].position_.x;
		posUp.y = nodes_[i].position_.y - 10 ;

		int rightIndex;
		int leftIndex;
		int downIndex;
		int upIndex;
		// Checking on 4 sides
		if (NodeExists(posRight,nodes_,rightIndex))
			if (!EdgeExists(nodes_[i].index_,rightIndex,edges_))
			{
				MapEdge from(nodes_[i].index_,rightIndex,10);
				from.setIndex(edgeIndex);
				++edgeIndex;
				edges_.push_back(from);
				nodes_[i].edges_.push_back(from);
			}
		if (NodeExists(posLeft,nodes_,leftIndex))
			if (!EdgeExists(nodes_[i].index_,leftIndex,edges_))
			{
				MapEdge from(nodes_[i].index_,leftIndex,10);
				from.setIndex(edgeIndex);
				++edgeIndex;
				edges_.push_back(from);
				nodes_[i].edges_.push_back(from);
			}
		if (NodeExists(posDown,nodes_,downIndex))
			if (!EdgeExists(nodes_[i].index_,downIndex,edges_))
			{
				MapEdge from(nodes_[i].index_,downIndex,10);
				from.setIndex(edgeIndex);
				++edgeIndex;
				edges_.push_back(from);
				nodes_[i].edges_.push_back(from);
			}
		if (NodeExists(posUp,nodes_,upIndex))
			if (!EdgeExists(nodes_[i].index_,upIndex,edges_))
			{
				MapEdge from(nodes_[i].index_,upIndex,10);
				from.setIndex(edgeIndex);
				++edgeIndex;
				edges_.push_back(from);
				nodes_[i].edges_.push_back(from);
			}

	}

		/**
	graph search
	**/
	tickTime_ = 0;
	path_.clear();
}

/**
================================
Map::pushWall()
================================
**/


void Map::pushWall(std::vector<sf::Vector2f>& coords)
{

	sf::ConvexShape shape;
	shape.setPointCount(coords.size());
	for (int i = 0; i < coords.size(); i++)
	{
		shape.setPoint(i,coords[i]);
	}
	shape.setFillColor(sf::Color(0,255,0,120));
	shape.setOutlineThickness(4);
	shape.setOutlineColor(sf::Color(0,255,0,120));
	walls_.push_back(shape);
}

/**
================================
Map::Update()
================================
**/

void Map::Update(sf::RenderWindow& window)
{
	tickTime_ += 1;
	if (tickTime_ >= 60)
	{
		GraphSearch s(*this);
		s.SearchCurrentMap(path::DIJKSTRA_SEARCH,5000,8000,path_);
	}
	for (int i = 0; i < path_.size();++i)
	{
		sf::CircleShape s(5);
		s.setFillColor(sf::Color(255,0,0));
		s.setPosition(nodes_[path_[i]].position_);
		window.draw(s);
	}
	sf::CircleShape s1(5);
	sf::CircleShape s2(5);
	s1.setFillColor(sf::Color(0,0,255));
	s2.setFillColor(sf::Color(0,0,255));
	s1.setPosition(nodes_[5000].position_);
	s2.setPosition(nodes_[8000].position_);

	window.draw(s1);
	window.draw(s2);


	for (int i = 0; i < walls_.size(); i++)
	{
		window.draw(walls_[i]);
	}
}

/**
================================
MapHandler::MapHandler()
================================
**/

MapHandler::MapHandler()
{
}

/**
================================
MapHandler::Init()
================================
**/

void MapHandler::Init(sf::RenderWindow& window)
{
	pWindow_ = &window;
}

/**
================================
MapHandler::Update()
================================
**/

void MapHandler::Update()
{
	int color = 0;
	for (int i = 0; i < maps_.size(); i++)
	{
		maps_[i].Update(*pWindow_);
	}

}

/**
================================
MapHandler::newMap()
================================
**/

void MapHandler::newMap(std::string filename)
{
	// This is the file_parsing functions
	// it uses istringstream to convert from string to integer
	std::ifstream fileStream;
	fileStream.open(filename);
	
	std::string output;
	std::string token;

	unsigned int resWidth = 0;
	unsigned int resHeight = 0;

	std::stringstream ss;
	
	Map map = Map();

	std::getline(fileStream,output);
	ss << output;
	std::getline(ss,token,' ');
	std::istringstream iss(token);
	iss >> resWidth;
	std::getline(ss,token,' ');
	iss = std::istringstream(token);
	iss >> resHeight;
	iss.clear();
	
	float factorW = static_cast<float>(pWindow_->getSize().x) / static_cast<float>(resWidth);
	float factorH = static_cast<float>(pWindow_->getSize().y) / static_cast<float>(resHeight);
	if (fileStream.is_open())
	{
		// It loops, find n = number of edges and then loops n times
		while (std::getline(fileStream,output))
		{
			int count;
			iss.str(output);
			iss >> count;
			std::vector<sf::Vector2f> walls;
			// clearing the streams
			ss.clear();
			iss.clear();
			for (int i = 0;i < count; i++)
			{
				sf::Vector2f wall;
				// getting the line
				std::getline(fileStream,output);
				// loading the stringstream
				ss << output;
				// splitting the string in tokens
				std::getline(ss,token,' ');
				// converting first token to int
				iss.str(token);
				iss >> wall.x;
				// clearing streams
				ss.clear();
				iss.clear();
				// getting the second token
				std::getline(ss,token,' ');
				// converting second token to int
				iss.str(token);
				iss >> wall.y;
				// clearing the streams
				ss.clear();
				iss.clear();
				// Adding factor
				wall.x *= factorW;
				wall.y *= factorH;
				walls.push_back(wall);
			}
			map.pushWall(walls);
			iss.clear();
		}
	}
	maps_.push_back(map);
	pActiveMap_ = &maps_.back();
	fileStream.close();

	// Generating the nodes
	maps_.back().CreateNodes();
}