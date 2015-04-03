/**
==========================================================================
								Map File
==========================================================================
**/

#include "Map.h"
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#include "../Utility/Helper.h"
#include "../Utility/WindowInfo.h"
#include "../Utility/Enum.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <iostream>
#include <fstream>
#include <sstream>


using namespace rapidxml;
/**=============================
Tile::Tile(IntRect)
=============================**/
Tile::Tile(sf::Texture& text,sf::IntRect& rect)
{
	IntRect_ = rect;
	Texture_ = &text;
	Sprite_.setTexture(text);
	Sprite_.setTextureRect(rect);
}

/**=============================
Tile::Draw
=============================**/
void Tile::Draw(sf::RenderWindow& window)
{
	window.draw(Sprite_);
}


/**=============================
TiledMap::LoadFromFile
=============================**/
void TiledMap::LoadFromFile(std::string fileName)
{
	// The file will be loaded, but then a big static image will be created 
	// with all the tiles, since the background is static there is no need.
	// the image is dinamically created.

	Window_ = nullptr;

	xml_document<> doc;
	xml_node<>* rootNode;

	std::ifstream fileStream(fileName + ".tmx");

	std::vector<char> buffer((std::istreambuf_iterator<char>(fileStream)),std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	fileStream.close();

	rootNode = doc.first_node("map");

	// getting tile info
	MapWidth_ = std::stoi(static_cast<std::string>(rootNode->first_attribute("width")->value()).c_str());
	MapHeight_ = std::stoi(static_cast<std::string>(rootNode->first_attribute("height")->value()).c_str());
	CellWidth_ = std::stoi(static_cast<std::string>(rootNode->first_attribute("tilewidth")->value()).c_str());
	CellHeight_ = std::stoi(static_cast<std::string>(rootNode->first_attribute("tileheight")->value()).c_str());

	TotalSize_ = sf::Vector2f(MapWidth_ * CellWidth_,MapHeight_ * CellHeight_);

	// getting texture names
	int count = 0; 
	for (xml_node<>* tileSetNode = rootNode->first_node("tileset");tileSetNode;
		tileSetNode = tileSetNode->next_sibling("tileset"))
	{
		if (count == 0)
		{
			FloorTextureFilename_ = static_cast<std::string>(tileSetNode->first_node("image")->first_attribute("source")->value());
			FloorID_ = atoi(tileSetNode->first_attribute("firstgid")->value());
		}
		else
		{
			WallsTextureFilename_ = static_cast<std::string>(tileSetNode->first_node("image")->first_attribute("source")->value());
			WallsID_ = atoi(tileSetNode->first_attribute("firstgid")->value());
		}
		++count;
	}

	// counter for the nodes
	int nodeCounter = 0;
	int indexCounter = 0;

	// getting the tiles id
	xml_node<>* dataNode = rootNode->first_node("layer")->first_node("data");
	for (xml_node<>* tileNode = dataNode->first_node(); tileNode;
		tileNode = tileNode->next_sibling())
	{
		int gid = atoi(static_cast<std::string>(tileNode->first_attribute("gid")->value()).c_str());
		TilesID_.push_back(gid);

		// creating the node from here
		// I'm also checking wheter the node on the right and on the top are walls, so that the node is not created 
		// on the neighbor node edge. Checking topleft aswell
 		if (gid < WallsID_) // if the tile is not a wall
		{
			// checking topleft only if different from 1 
			int topLeftIndex = nodeCounter - MapWidth_ -1;
			if (topLeftIndex > 0 && TilesID_[topLeftIndex] < WallsID_)
			{
				float posX = nodeCounter % MapWidth_;
				float posY = (nodeCounter - posX) / MapWidth_;
				MapNode node(sf::Vector2f(posX * CellWidth_,posY * CellHeight_),indexCounter,NODE_TYPE_NONE);
				Nodes_.push_back(node);
					++indexCounter;
			}
		}

		++nodeCounter;
	}

	fileStream.close();

	// Splitting the filenames to get only the texture name
	FloorTextureFilename_ = FloorTextureFilename_.substr(FloorTextureFilename_.rfind("/") + 1);
	WallsTextureFilename_ = WallsTextureFilename_.substr(WallsTextureFilename_.rfind("/") + 1);

	// Now it's time to load textures and create the map
	FloorTexture_.loadFromFile("data\\new-assets\\textures\\" + FloorTextureFilename_);
	WallTexture_.loadFromFile("data\\new-assets\\textures\\" + WallsTextureFilename_);

	// creating the subrects
	int maxWidth = FloorTexture_.getSize().x;
	int maxHeight = FloorTexture_.getSize().y;
	for (int row = 0; row < maxHeight; row += CellHeight_)
	{
		for (int col = 0; col < maxWidth; col += CellWidth_)
		{
			sf::IntRect rect(sf::Vector2i(col,row),sf::Vector2i(CellWidth_,CellHeight_));
			TileRects_.push_back(rect);
		}
	}

	maxWidth = WallTexture_.getSize().x;
	maxHeight = WallTexture_.getSize().y;
	for (int row = 0; row < maxHeight; row += CellHeight_)
	{
		for (int col = 0; col < maxWidth; col += CellWidth_)
		{
			sf::IntRect rect(sf::Vector2i(col,row),sf::Vector2i(CellWidth_,CellHeight_));
			TileRects_.push_back(rect);
		}
	}
	// creating the tiles
	count = 0; 
	for (int row = 0; row < MapHeight_ * CellWidth_; row += CellWidth_)
	{
		for (int col = 0; col < MapWidth_ * CellHeight_; col += CellHeight_)
		{
			if (TilesID_[count] >= WallsID_)
			{
				Tiles_.push_back(Tile(WallTexture_,TileRects_[TilesID_[count] -1]));
				Tiles_[count].Sprite_.setPosition(sf::Vector2f(col,row));
				Tiles_[count].TileID_ = TilesID_[count];
			}
			else
			{
				Tiles_.push_back(Tile(FloorTexture_,TileRects_[TilesID_[count] -1]));
				Tiles_[count].Sprite_.setPosition(sf::Vector2f(col,row));
				Tiles_[count].TileID_ = TilesID_[count];
			}

			++count;
		}
	}

	// Filling the Walls vector with all the entities for collision detection
	std::size_t tSize = Tiles_.size();
	// Getting texture sizes
	float w = static_cast<float>(WallTexture_.getSize().x);
	float h = static_cast<float>(WallTexture_.getSize().y);

	for (std::size_t i = 0; i < tSize; ++i)
	{
		if (Tiles_[i].TileID_ >= WallsID_)
		{
			Entity entity(Tiles_[i].Sprite_.getPosition(),CellWidth_,CellHeight_,ENTITY_TYPE_WALL);
			
			// Adding points of visibility
			int mapIndex = i;// getting index from position

			if (entity.Position_.x < (MapWidth_ * CellWidth_) - CellWidth_ && TilesID_[mapIndex+1] < WallsID_ ) 
			{
				entity.RightVisible_ = true; 
			}else entity.RightVisible_ = false;

			if (entity.Position_.x > CellWidth_ && TilesID_[mapIndex-1] < WallsID_  )
			{
					entity.LeftVisible_ = true; 
			}else entity.LeftVisible_ = false;
					
			if (entity.Position_.y < (MapHeight_ * CellHeight_ ) - CellHeight_ && TilesID_[mapIndex+MapWidth_] < WallsID_ )
			{
				entity.DownVisible_ = true;
			}else entity.DownVisible_ = false;
			
						
			if (entity.Position_.y > CellHeight_ && TilesID_[mapIndex-MapWidth_] < WallsID_ )
			{
				entity.TopVisible_ = true; 
			}else entity.TopVisible_ = false;
			

			Walls_.push_back(entity);
		}
	}


	// value to check whether the texture has been assigned to the sprite or not
	Loaded_ = false;

	// before caching the edges i'm creating the background texture

	// loading the images
	sf::Image WallImage;
	WallImage.loadFromFile("data\\new-assets\\textures\\" + WallsTextureFilename_);

	sf::Image FloorImage;
	FloorImage.loadFromFile("data\\new-assets\\textures\\" + FloorTextureFilename_);

	FullBackgroundImage_.create(WINDOW_WIDTH,WINDOW_HEIGHT);

	std::size_t tileSize = Tiles_.size();
	for (int i = 0; i < tileSize;++i)
	{
		if (TilesID_[i] >= WallsID_)
		{
			FullBackgroundImage_.copy(WallImage,Tiles_[i].Sprite_.getPosition().x,Tiles_[i].Sprite_.getPosition().y,Tiles_[i].IntRect_);
		}
		else
		{
			FullBackgroundImage_.copy(FloorImage,Tiles_[i].Sprite_.getPosition().x,Tiles_[i].Sprite_.getPosition().y,Tiles_[i].IntRect_);
		}
	}

	FullBackgroundTexture_.loadFromImage(FullBackgroundImage_);
	FullBackgroundSprite_.setTexture(FullBackgroundTexture_);
	
	// checking if XML file exists
	std::ifstream edgeFile(fileName + ".edge.cache");
	if (edgeFile.is_open())
	{
		// edges are presents ! parsing the file
		xml_document<> parseDoc;

		std::vector<char> buffer((std::istreambuf_iterator<char>(edgeFile)),std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		parseDoc.parse<0>(&buffer[0]);

		xml_node<>* RootNode = parseDoc.first_node();
		for (xml_node<>* SingleEdge = RootNode->first_node(); SingleEdge;
			SingleEdge = SingleEdge->next_sibling())
		{
			// getting the info
			int From = atoi(SingleEdge->first_attribute("From")->value());
			int To = atoi(SingleEdge->first_attribute("To")->value());
			int Cost = atoi(SingleEdge->first_attribute("Cost")->value());
			int Index = atoi(SingleEdge->first_attribute("Index")->value());

			MapEdge edge(From,To,Cost);
			edge.SetIndex(Index);
			Edges_.push_back(edge);
		
			// setting the edges to the node
			(*Nodes_[From].GetEdges()).push_back(edge);

		}

		return;
		edgeFile.close();
	}
	else
		edgeFile.close();

	// creating the edges
	std::size_t s = Nodes_.size();

	sf::Vector2f posRight;
	sf::Vector2f posLeft;
	sf::Vector2f posDown;
	sf::Vector2f posUp;
	sf::Vector2f upRight;
	sf::Vector2f upLeft;
	sf::Vector2f downLeft;
	sf::Vector2f downRight;

	// edge index counter
	int edgeIndex = 0;

	for (std::size_t i = 0; i < s; ++i)
	{
		// this is the code from the old project 
		//////////////////////////////////////
		posRight.x = Nodes_[i].GetPosition().x + CellWidth_;
		posRight.y = Nodes_[i].GetPosition().y;

		posLeft.x = Nodes_[i].GetPosition().x - CellWidth_;
		posLeft.y = Nodes_[i].GetPosition().y;

		posDown.x = Nodes_[i].GetPosition().x;
		posDown.y = Nodes_[i].GetPosition().y + CellHeight_;

		posUp.x = Nodes_[i].GetPosition().x;
		posUp.y = Nodes_[i].GetPosition().y - CellHeight_;

		upRight.x = Nodes_[i].GetPosition().x + CellWidth_;
		upRight.y = Nodes_[i].GetPosition().y - CellHeight_;

		upLeft.x = Nodes_[i].GetPosition().x - CellWidth_;
		upLeft.y = Nodes_[i].GetPosition().y - CellHeight_;

		downRight.x = Nodes_[i].GetPosition().x + CellWidth_;
		downRight.y = Nodes_[i].GetPosition().y + CellHeight_;

		downLeft.x = Nodes_[i].GetPosition().x - CellWidth_;
		downLeft.y = Nodes_[i].GetPosition().y + CellHeight_;

		int rightIndex;
		int leftIndex;
		int downIndex;
		int upIndex;
		int upRightIndex;
		int upLeftIndex;
		int downRightIndex;
		int downLeftIndex;

		// Checking on 4 sides
		if (NodeExists(posRight,Nodes_,rightIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),rightIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),rightIndex,10);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(posLeft,Nodes_,leftIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),leftIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),leftIndex,10);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(posDown,Nodes_,downIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),downIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),downIndex,10);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(posUp,Nodes_,upIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),upIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),upIndex,10);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		// obliqual nodes
		if (NodeExists(upRight,Nodes_,upRightIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),upRightIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),upRightIndex,12);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(upLeft,Nodes_,upLeftIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),upLeftIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),upLeftIndex,12);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(downRight,Nodes_,downRightIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),downRightIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),downRightIndex,12);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(downLeft,Nodes_,downLeftIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),downLeftIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),downLeftIndex,12);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}


		////////////////////////////////////////////
		// i'll rewrite it in the next future //////
	}

	// formatting the output into XML file
	xml_document<> edgeDoc;

	xml_node<>* RootNode = edgeDoc.allocate_node(node_element,"root");
	edgeDoc.append_node(RootNode);	

	std::ostringstream stringStream;
	std::size_t edgeSize = Edges_.size();
	for (std::size_t i = 0; i < edgeSize; ++i)
	{
		// From
		std::string From = static_cast<std::ostringstream*>(&(std::ostringstream() << Edges_[i].From()))->str();
		stringStream.clear();
		stringStream.str(std::string());

		// To
		std::string To = static_cast<std::ostringstream*>(&(std::ostringstream() << Edges_[i].To()))->str();
		stringStream.clear();
		stringStream.str(std::string());

		// Cost
		std::string Cost = static_cast<std::ostringstream*>(&(std::ostringstream() << Edges_[i].Cost()))->str();
		stringStream.clear();
		stringStream.str(std::string());

		// Index
		std::string Index = static_cast<std::ostringstream*>(&(std::ostringstream() << Edges_[i].Index()))->str();
		stringStream.clear();
		stringStream.str(std::string());

		xml_node<>*	edgeNode = edgeDoc.allocate_node(node_element,"edge");
		char* FromName = doc.allocate_string(From.c_str());
		char* ToName = doc.allocate_string(To.c_str());
		char* IndexName = doc.allocate_string(Index.c_str());
		char* CostName = doc.allocate_string(Cost.c_str());

		edgeNode->append_attribute(edgeDoc.allocate_attribute("From",FromName));
		edgeNode->append_attribute(edgeDoc.allocate_attribute("To",ToName));
		edgeNode->append_attribute(edgeDoc.allocate_attribute("Index",IndexName));
		edgeNode->append_attribute(edgeDoc.allocate_attribute("Cost",CostName));

		RootNode->append_node(edgeNode);
	}

	std::ofstream edgeStream(fileName + ".edge.cache");

	edgeStream << edgeDoc;

	edgeStream.close();


}

/**=============================
TiledMap::IsPathObstructed
=============================**/
bool TiledMap::IsPathObstructed(const sf::Vector2f& from,const sf::Vector2f& to)
{
	int stepY,stepX;
	int error;
	int errorprev;

	sf::Vector2f start;
	start.x = from.x - (static_cast<int>(floor(from.x)) % CellWidth_);
	start.y = from.y - (static_cast<int>(floor(from.y)) % CellHeight_);

	sf::Vector2f end;
	end.x = to.x - (static_cast<int>(floor(to.x)) % CellWidth_);
	end.y = to.y - (static_cast<int>(floor(to.y)) % CellHeight_);

	sf::Vector2f xy;
	xy.x = start.x;
	xy.y = start.y;

	// checking starting point
	// solving the position into id
	int index = ((xy.y / CellHeight_) * MapWidth_) + (xy.x / CellWidth_);

	// immediatly checking if it is a wall -> faster algorithm
	if (index < TilesID_.size() &&TilesID_[index] >= WallsID_)
		return true;

	int dx = end.x - start.x;
	int dy = end.y - start.y;

	if (dy < 0)
	{
		stepY = -CellHeight_;
		dy = - dy;
	}else
		stepY = CellHeight_;
	if (dx < 0)
	{
		stepX = -CellWidth_;
		dx = - dx;
	}
	else
		stepX = CellWidth_;

	int ddy = 2 * dy;
	int ddx = 2 * dx;

	if (ddx >= ddy)
	{
		errorprev = dx;
		error = dx;
		for (int i = 0; i < dx;i += CellWidth_)
		{
			xy.x += stepX;
			error += ddy;
			if (error > ddx)
			{
				xy.y += stepY;
				error -= ddx;
				if (error + errorprev < ddx)
				{
					// solving the position into id
					int index = ((xy.y / CellHeight_) * MapWidth_) + (xy.x / CellWidth_);

					// immediatly checking if it is a wall -> faster algorithm
					if (index < TilesID_.size() &&TilesID_[index] >= WallsID_)
						return true;
				}
				else if (error + errorprev > ddx)
				{
					// solving the position into id
					int index = ((xy.y / CellHeight_) * MapWidth_) + (xy.x / CellWidth_);

					// immediatly checking if it is a wall -> faster algorithm
					if (index < TilesID_.size() &&TilesID_[index] >= WallsID_)
						return true;
				}
			}

			// solving the position into id
			int index = ((xy.y / CellHeight_) * MapWidth_) + (xy.x / CellWidth_);

			// immediatly checking if it is a wall -> faster algorithm
			if (index < TilesID_.size() &&TilesID_[index] >= WallsID_)
				return true;

			errorprev = error;

		}
	}
	else
	{
		errorprev = error = dy;
		for (int i = 0; i < dy; i += CellHeight_)
		{
			xy.y += stepY;
			error += ddx;
			if (error > ddy)
			{
				xy.x += stepX;
				error -= ddy;
				if (error + errorprev < ddy)
				{
					// solving the position into id
					int index = ((xy.y / CellHeight_) * MapWidth_) + (xy.x / CellWidth_);

					// immediately checking if it is a wall -> faster algorithm
					if (index < TilesID_.size() &&TilesID_[index] >= WallsID_)
						return true;
				}
				else if (error + errorprev > ddy)
				{
					// solving the position into id
					int index = ((xy.y / CellHeight_) * MapWidth_) + (xy.x / CellWidth_);

					// immediatly checking if it is a wall -> faster algorithm
					if (index < TilesID_.size() &&TilesID_[index] >= WallsID_)
						return true;
				}
			}

			// solving the position into id
			int index = ((xy.y / CellHeight_) * MapWidth_) + (xy.x / CellWidth_);

			// immediatly checking if it is a wall -> faster algorithm
			if (index < TilesID_.size() &&TilesID_[index] >= WallsID_)
				return true;

			errorprev = error;

		}
	}
	return false;
}

/**=============================
TiledMap::IsPathObstructed
=============================**/
bool TiledMap::IsPathObstructed(int from,int to)
{
	// to check if a path is obstructed or not i've readapted a line drawing algorithm 
	// every tile is treated as a pixel, should be pretty fast	

	return true;
}


/**=============================
TiledMap::Draw
=============================**/
void TiledMap::Draw(sf::RenderWindow& window)
{
	Window_ = &window;

	if (!Loaded_) // doing this because the class is losing pointer.. i'll come here later
	{
		std::size_t s = Tiles_.size();
		for (int i = 0; i < s; ++i)
		{
			if (Tiles_[i].TileID_ >= WallsID_)
			{
				Tiles_[i].Sprite_.setTexture(WallTexture_);
				Tiles_[i].Sprite_.setTextureRect(Tiles_[i].IntRect_);
			}
			else
			{
				Tiles_[i].Sprite_.setTexture(FloorTexture_);
				Tiles_[i].Sprite_.setTextureRect(Tiles_[i].IntRect_);
			}
		}
		FullBackgroundSprite_.setTexture(FullBackgroundTexture_);
		Loaded_ = true;
	}
	window.draw(FullBackgroundSprite_);
	std::size_t s = Edges_.size();
	/**for (int i = 0; i < s;++i)
	{
		sf::Vertex l[2] = {Nodes_[Edges_[i].From()].GetPosition(),Nodes_[Edges_[i].To()].GetPosition()};
		window.draw(l,2,sf::Lines);
	}**/
}


/**=============================
MapHandler::NewMap
=============================**/
void MapHandler::NewMap(std::string fileName)
{
	// creating new tiled map
	TiledMap newMap;
	newMap.LoadFromFile(fileName);
	Maps_.push_back(newMap);
	ActiveMap_ = &Maps_[0];
	return;
}

/**=============================
MapHandler::Init
=============================**/
void MapHandler::Init(sf::RenderWindow& window)
{
	Window_ = &window;
}

/**=============================
MapHandler::Update
=============================**/
void MapHandler::Update()
{
	std::size_t s = Maps_.size();
	for (std::size_t i = 0; i < s; ++i)
	{
		Maps_[i].Draw(*Window_);
	}
}