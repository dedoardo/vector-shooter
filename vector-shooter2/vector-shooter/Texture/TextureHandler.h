/**
==========================================================================
								Texture Handler
==========================================================================
**/

#ifndef TEXTURE_HANDLER_H_INCLUDED
#define TEXTURE_HANDLER_H_INCLUDED

#include <vector>

#include <SFML/Graphics/Texture.hpp>

class TextureHandler
{
public :
	TextureHandler();
	
	void								LoadAllTextures(); // called on loading phase

	sf::Texture*						GetTexture(int);

	void								Update();// used to check if textures are unused 

private :
	void								UnloadTexture();
	void								LoadTexture();

	std::vector<sf::Texture*>			Textures_; // vector of textures indexed by enumerator
	std::vector<int>					TexturesUsers_; // number of person using the texture;
};

#endif