/**
==========================================================================
								Map File
==========================================================================
**/

#ifndef TILED_MAP_H_INCLUDED
#define TILED_MAP_H_INCLUDED

#include <string>
#include <list>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "MapElement.h"
#include "../Entity/Entity.h"

class Tile
{
public :
	Tile(sf::Texture&,sf::IntRect&);
	void Draw(sf::RenderWindow&);
	
	sf::Sprite			Sprite_;
	sf::Texture*		Texture_;
	sf::IntRect			IntRect_;
	int					TileID_;
};

class TiledMap
{
public :
	void					LoadFromFile(std::string);
	void					Draw(sf::RenderWindow&);

	inline sf::Vector2f		GetSize()const{return TotalSize_;};
	std::vector<Entity>*	GetWalls(){return &Walls_;};
	std::vector<MapNode>*	GetNodes(){return &Nodes_;};

	bool					IsPathObstructed(const sf::Vector2f&,const sf::Vector2f&);
	bool					IsPathObstructed(int,int);

private :
	sf::Vector2f				TotalSize_;
	bool						Loaded_;
	std::string					MapName_;
	int							CellWidth_;
	int							CellHeight_;
	int							MapWidth_;
	int							MapHeight_;
	std::vector<int>			TilesID_;
	int							FloorID_;
	int							WallsID_;
	std::string					WallsTextureFilename_;
	std::string					FloorTextureFilename_;
	
	sf::Texture					FloorTexture_;
	sf::Texture					WallTexture_;
	sf::Image					FullBackgroundImage_;
	sf::Texture					FullBackgroundTexture_;
	sf::Sprite					FullBackgroundSprite_;
	std::vector<sf::Texture>	Textures_;
	std::vector<sf::IntRect>	TileRects_;
	std::vector<Tile>			Tiles_;

	std::vector<MapEdge>		Edges_;
	std::vector<MapNode>		Nodes_;
	std::vector<Entity>			Walls_;

	sf::RenderWindow*			Window_;

	friend class PathFinder;
};

#endif // tiled map included

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <vector>
#include <string>

class MapHandler
{
public :
	void						Init(sf::RenderWindow&);
	void						NewMap(std::string);
	void						Update();
	inline TiledMap*			GetActiveMap(){return ActiveMap_;};

private :
	std::vector<TiledMap>		Maps_;
	TiledMap*					ActiveMap_;
	sf::RenderWindow*			Window_;

	friend class PathFinder;

};

#endif // map included