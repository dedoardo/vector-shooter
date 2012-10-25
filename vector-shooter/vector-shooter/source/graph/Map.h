///////////////////////////////////////////////////////
//				MAP CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


#include <string>
#include <list>
#include <vector>
#include <map>

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

enum
{
	NODE_TAKEN,
	NODE_EMPTY
};

// forward declaration
class MapEdge;

class MapNode
{
public :
	MapNode(){}
	MapNode(sf::Vector2f& pos,int index = 0):index_(index),position_(pos){};

	MapNode*		parent_;
	int				index_;
	sf::Vector2f	position_;
	std::vector<MapEdge> edges_;
};

class MapEdge
{
public :
	MapEdge(int,int,float);

	// "Accessors"
	inline int		From()const{return from_;};
	inline int		To()const{return to_;};
	inline float	Cost()const{return cost_;};
	inline int		Index()const{return index_;};

	// Mutators
	inline void		setCost(float cost){cost_ = cost;};
	inline void		setIndex(int index){index_ = index;};

private :
	int				from_;
	int				to_;
	int				index_;
	float			cost_;

};

/////////////////////////////////////////////////////////////////////

class Map
{
public :
	Map();
	Map(std::string&);
	void pushWall(std::vector<sf::Vector2f>&);

	void Update(sf::RenderWindow&);

	// accessors
	inline std::vector<MapNode>*		 getNodes(){return &nodes_;};
	inline std::vector<sf::ConvexShape>* getWalls(){return &walls_;};
	inline std::vector<MapEdge>*		 getEdges(){return &edges_;};

	// Used to create nodes from the actual map
	void								 CreateNodes();

private :
	// recursive function for flood-fill algorithm
	void						FloodFill(std::vector<MapNode*>&,int&,std::vector<sf::Vector2f>);

	std::string							name_;
	std::vector<sf::ConvexShape>		walls_;
	std::vector<MapNode>				nodes_;
	std::vector<MapEdge>				edges_;

	sf::Clock							clock_;
	float								tickTime_;
	std::vector<int>					path_;
};

class MapHandler
{
public :
	////////////////////////////////////
	//  CTORS
	////////////////////////////////////

	MapHandler();

	///////////////////////////////////
	// METHODS 
	//////////////////////////////////
	void			Init(sf::RenderWindow& window);
	void			newMap(std::string);
	void			Update();
	inline Map*		getActiveMap(){return pActiveMap_;};

private :
	std::vector<Map>			maps_;
	Map*						pActiveMap_;
	sf::RenderWindow*			pWindow_;

};

#endif