#ifndef ASSET_MANAGER_H_INCLUDED_
#define ASSET_MANAGER_H_INCLUDED_

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>

#include "../rapidxml/rapidxml.hpp"
using namespace rapidxml;

#include <string>
#include <unordered_map>

class AssetManager
{
public : 
	AssetManager();
	AssetManager(const std::string&);
	~AssetManager();

	// Getters and setters
	sf::Texture* GetTexture(const std::string&);
	sf::SoundBuffer* GetSoundBuffer(const std::string&);
	sf::Font* GetFont(const std::string&);

	// Loads all the assets
	void Load(const std::string&);

private :

	// Asset maps
	std::unordered_map<std::string,sf::Texture*> TextureMap_;
	std::unordered_map<std::string,sf::SoundBuffer*> SoundMap_;
	std::unordered_map<std::string,sf::Font*> FontMap_;
};

#endif // ASSET_MANAGER_H_INCLUDED_