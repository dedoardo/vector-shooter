/**
==========================================================================
								UI source
==========================================================================
**/


#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <vector>
#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../Utility/Enum.h"
#include "UIElement.h"

// Forward declarations
class Player;
class WeaponSystem;
class Game;

class UI
{
public :
	UI();

	void Init(sf::RenderWindow&,Player&,WeaponSystem&,Game&);

	void Update(const sf::Time& deltaTime);

private :
	std::vector<sf::Texture>			Textures_; // indexed by id
	std::vector<UIElement>				AmmoIcons_;
	std::vector<sf::IntRect>			TextureRects_;

	unsigned int						ActiveWeapon_;
	sf::ConvexShape						SelectBox_;
	sf::Texture*						WeaponSpriteSheet_;
	bool								Loaded_;

	unsigned int						NumberOfElements_;

	int									SpriteWidth_;
	int									SpriteHeight_;

	std::vector<sf::Text>				AmmoTexts_;
	std::vector<sf::Text>				LoaderTexts_;
	sf::Font*							AmmoFont_;
	unsigned int						AmmoFontSize_;

	sf::RenderWindow*					Window_;
	Player*								Player_;
	WeaponSystem*						WeaponSystem_;
	Game*								Game_;

};

#endif