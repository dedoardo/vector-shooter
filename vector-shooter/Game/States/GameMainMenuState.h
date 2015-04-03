/**
==========================================================================
								MainMenuState
==========================================================================
**/

/**=============================
Includes
=============================**/

#include <SFML/Graphics/RenderWindow.hpp>
#include "BaseGameState.h"
#include "..\..\Menu\MainMenu.h"

/**=============================
Forward declarations
=============================**/
class Game;

#ifndef MAIN_MENU_STATE_H_INCLUDED
#define MAIN_MENU_STATE_H_INCLUDED

class MainMenuState : public GameState
{
public :
	MainMenuState(){};
	void Loop();
	void Init(sf::RenderWindow&,Game&);

private :
	void			HandleEvents();
	MainMenu		MainMenu_;
};

#endif // main menu state