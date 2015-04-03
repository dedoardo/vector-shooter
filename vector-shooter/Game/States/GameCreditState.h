#ifndef GAME_CREDIT_STATE_H_INCLUDED_
#define GAME_CREDIT_STATE_H_INCLUDED_

#include "BaseGameState.h"

class GameCreditState : public GameState
{
public :
	GameCreditState(){};

	void Loop();
	void Init(sf::RenderWindow&,Game&);

private :
	void HandleEvents();

};

#endif // GAME_CREDIT_STATE_H_INCLUDED_