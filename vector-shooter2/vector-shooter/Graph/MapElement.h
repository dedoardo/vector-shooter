/**
==========================================================================
						Map Elements
==========================================================================
**/

#ifndef MAP_ELEMENTS_H_INCLUDED
#define MAP_ELEMENTS_H_INCLUDED

#include <vector>
#include <SFML/System/Vector2.hpp>

class MapEdge
{
public :
	MapEdge(int from,int to,float cost):From_(from),To_(to),Cost_(cost){};
	
	inline int					From(){return From_;};
	inline int					To(){return To_;};
	inline float				Cost(){return Cost_;}
	inline int					Index(){return Index_;};

	inline void					SetCost(float cost){Cost_ = cost;};
	inline void					SetIndex(float index){Index_ = index;};

private :
	int							Index_;
	int							From_;
	int							To_;
	float						Cost_;
};

class MapNode
{
public :
	MapNode(sf::Vector2f& pos,int index = 0):Index_(index),Position_(pos){};

	inline sf::Vector2f GetPosition(){return Position_;};
	inline int			GetIndex(){return Index_;};
	inline std::vector<MapEdge>* GetEdges(){return &Edges_;};
	inline int			GetParent(){return Parent_;};

private :
	sf::Vector2f				Position_;
	int							Parent_;
	int							Index_;
	std::vector<MapEdge>		Edges_;

};


#endif // map elements included