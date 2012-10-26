///////////////////////////////////////////////////////
//				GAME STATE CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#ifndef GAMECLASS_INCLUDED_H
#define GAMECLASS_INCLUDED_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "../entity/Player.h"
#include "../menu/Menu.h"
#include "Game.h"
#include "../graph/Map.h"
#include "Collision.h"

// Abstract class
class GameState
{
public :
	virtual void Loop()=0;
	virtual void Init(sf::RenderWindow&,Game&)=0;

	inline void setState(int stateIndex)
	{
		pGame_->setGameState(stateIndex+1);
		stateChanged_ = true;
	};

protected :
	virtual void HandleEvents()=0;

	int				classState_;
	bool			stateChanged_;

	sf::RenderWindow*		pWindow_;
	Game*					pGame_;

};

///////////////////////////////////
// ALL THE FOLLOWING CLASSES WILL INHERITH FROM THE ABSTRACT ONE
///////////////////////////////////

class MainMenuState : public GameState
{
public :
	MainMenuState();
	void Loop();
	void Init(sf::RenderWindow&,Game&);

private :
	MainMenu		mainMenu_;

	void			HandleEvents();
};




class GameRunState : public GameState
{
public :
	GameRunState();
	void Loop();
	void Init(sf::RenderWindow&,Game&);

private :
	Player					player_;
	MapHandler				mapHandler_;
	CollisionHandler		collisionHandler_;

	void			HandleEvents();
};

#endif // Game class