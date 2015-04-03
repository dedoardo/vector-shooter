/**
==========================================================================
								Game States
==========================================================================
**/

#ifndef BASE_GAME_STATE_H_INCLUDED
#define BASE_GAME_STATE_H_INCLUDED

/**=============================
Includes
=============================**/

#include <SFML/Graphics/RenderWindow.hpp>
#include "..\Game.h"

/**=============================
Forward declarations
=============================**/

// defining only abstract class
class GameState
{
public :
	virtual void Loop()=0;
	virtual void Init(sf::RenderWindow&,Game&)=0;

	inline void setState(int stateIndex)
	{
		Game_->setGameState(stateIndex);
		StateChanged_ = true;
	}

protected :
	virtual void HandleEvents()=0;

	int	ClassState_;
	bool StateChanged_;

	sf::RenderWindow*	Window_;
	Game*				Game_;

};

#endif // Base game state