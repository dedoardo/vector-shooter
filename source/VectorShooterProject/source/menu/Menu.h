

#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>
#include <map>
#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class MainMenuState;

enum{
	MAINMENU_PLAY,
	MAINMENU_HIGHSCORES,
	MAINMENU_OPTIONS,
	MAINMENU_QUIT
};

enum{
	MENU_OFF,
	MENU_ON
};

class MenuObject
{
public :
	MenuObject();
	void							Init(sf::Texture&,sf::Texture&,sf::Vector2f&,std::string);

	void							Draw(sf::RenderWindow& window);
	bool							isClicked(const sf::RenderWindow&);

	inline int						getID(){return id_;};

private :
	bool							isOn(const sf::RenderWindow&);

	sf::Vector2f					position_;
	sf::Texture*					textureOn_;
	sf::Texture*					textureOff_;
	sf::Sprite						spriteOn_;
	sf::Sprite						spriteOff_;
	int								state_;
	int								id_;
	float							width_;
	float							height_;
	sf::Text						text_;
	sf::Font						font_;
	sf::Color						color_;
};

class MainMenu
{
public :
	MainMenu();

	void							Init(sf::RenderWindow&,MainMenuState&);
	void							Update();

private :
	std::vector<MenuObject>			menuObjects_;

	MainMenuState*					pGameState_;
	sf::RenderWindow*				pWindow_;
	sf::Texture						buttonOff_;
	sf::Texture						buttonOn_;
	sf::Texture						dialogBox_;
};

#endif // !MENU_H_INCLUDED