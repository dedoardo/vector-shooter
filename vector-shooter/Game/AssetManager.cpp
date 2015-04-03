#include "AssetManager.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include "../rapidxml/rapidxml.hpp"

using namespace rapidxml;

// Empty constructor
AssetManager::AssetManager()
{

}

// RAII loads all the files
AssetManager::AssetManager(const std::string& assetFilename)
{
	Load(assetFilename);	
}

AssetManager::~AssetManager()
{
	// Deleting from memory all the assets
	for (auto asset : TextureMap_)
	{
		delete asset.second;
	}

	for (auto asset : SoundMap_)
	{
		delete asset.second;
	}

	for (auto asset : FontMap_)
	{
		delete asset.second;
	}

	// clearing the vectors
	TextureMap_.clear();
	SoundMap_.clear();
	FontMap_.clear();
}

sf::Texture* AssetManager::GetTexture(const std::string& assetID)
{
	return TextureMap_[assetID];
}

sf::Font* AssetManager::GetFont(const std::string& assetID)
{
	return FontMap_[assetID];
}

sf::SoundBuffer* AssetManager::GetSoundBuffer(const std::string& assetID)
{
	return SoundMap_[assetID];
}


void AssetManager::Load(const std::string& assetFilename)
{
	// Loads the file
	xml_document<> doc;
	std::ifstream fstream;
	fstream.open(assetFilename);
	
	if (!fstream.is_open())
	{
		std::cout << "AssetManager::Load() failed!, couldn't find the file " << assetFilename << std::endl;
		return;
	}

	// filling the document with data
	std::vector<char> data((std::istreambuf_iterator<char>(fstream)),std::istreambuf_iterator<char>());
	data.push_back('\0');
	doc.parse<0>(&data[0]);

	fstream.close();

	xml_node<>* assets = doc.first_node();
	for (xml_node<>* asset = assets->first_node("asset"); asset; asset = asset->next_sibling())
	{
		std::string type = asset->first_attribute("type")->value();
		std::transform(type.begin(),type.end(),data.begin(),::tolower);
		std::string id = asset->first_attribute("id")->value();
		std::string src = asset->first_attribute("src")->value();
	
		if (type == "texture")
		{
			sf::Texture* texture = new sf::Texture;
			texture->loadFromFile(src);
			TextureMap_.insert(std::make_pair(id,texture));
		}
		else if (type == "font")
		{
			sf::Font* font = new sf::Font;
			font->loadFromFile(src);
			FontMap_.insert(std::make_pair(id,font));
		}
		else if (type == "sound")
		{
			sf::SoundBuffer* sound = new sf::SoundBuffer;
			sound->loadFromFile(src);
			SoundMap_.insert(std::make_pair(id,sound));
		}
	}
}