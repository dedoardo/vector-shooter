/**
==========================================================================
								Game Run State
==========================================================================
**/

/**=============================
Includes
=============================**/

#include <SFML/Graphics/RenderWindow.hpp>
#include "BaseGameState.h"

/**=============================
Forward declarations
=============================**/
class Game;

#ifndef GAME_RUN_STATE_H_INCLUDED
#define GAME_RUN_STATE_H_INCLUDED

class GameRunState : public GameState
{
public :
	GameRunState();
	void Loop();
	void Init(sf::RenderWindow&,Game&);

private :
	void			HandleEvents();


};

#endif // main menu state