/**
==========================================================================
								Map File
==========================================================================
**/

#include "Map.h"
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#include "../Utility/Helper.h"

#include <SFML/Graphics/Sprite.hpp>
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

	xml_document<> doc;
	xml_node<>* rootNode;

	std::ifstream fileStream(fileName + ".tmx");
	std::vector<char> buffer((std::istreambuf_iterator<char>(fileStream)),std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	rootNode = doc.first_node("map");

	// getting tile info
	MapWidth_ = atoi(static_cast<std::string>(rootNode->first_attribute("width")->value()).c_str());
	MapHeight_ = atoi(static_cast<std::string>(rootNode->first_attribute("height")->value()).c_str());
	CellWidth_ = atoi(static_cast<std::string>(rootNode->first_attribute("tilewidth")->value()).c_str());
	CellHeight_ = atoi(static_cast<std::string>(rootNode->first_attribute("tileheight")->value()).c_str());

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
		if (gid < WallsID_) // if the tile is not a wall
		{
			float posX = nodeCounter % MapWidth_;
			float posY = (nodeCounter - posX) / MapWidth_;
			MapNode node(sf::Vector2f(posX * CellWidth_,posY * CellHeight_),indexCounter);
			Nodes_.push_back(node);
			++indexCounter;
		}

		++nodeCounter;
	}

	fileStream.close();

	// Splitting the filenames to get only the texture name
	FloorTextureFilename_ = FloorTextureFilename_.substr(FloorTextureFilename_.rfind("/") + 1);
	WallsTextureFilename_ = WallsTextureFilename_.substr(WallsTextureFilename_.rfind("/") + 1);

	// Now it's time to load textures and create the map
	FloorTexture_.loadFromFile("data\\assets\\" + FloorTextureFilename_);
	WallTexture_.loadFromFile("data\\assets\\" + WallsTextureFilename_);

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

	// texture will be assigned later
	Loaded_ = false;

	
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
				MapEdge from(Nodes_[i].GetIndex(),upRightIndex,14);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(upLeft,Nodes_,upLeftIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),upLeftIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),upLeftIndex,14);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(downRight,Nodes_,downRightIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),downRightIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),downRightIndex,14);
				from.SetIndex(edgeIndex);
				++edgeIndex;
				Edges_.push_back(from);
				Nodes_[i].GetEdges()->push_back(from);
			}
		if (NodeExists(downLeft,Nodes_,downLeftIndex,s))
			if (!EdgeExists(Nodes_[i].GetIndex(),downLeftIndex,Edges_,edgeIndex))
			{
				MapEdge from(Nodes_[i].GetIndex(),downLeftIndex,14);
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
TiledMap::Draw
=============================**/
void TiledMap::Draw(sf::RenderWindow& window)
{
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
		Loaded_ = true;
	}
	std::size_t s = Tiles_.size();
	for (std::size_t i = 0; i < s; ++i)
	{
		Tiles_[i].Draw(window);
	}
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