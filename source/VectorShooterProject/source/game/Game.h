///////////////////////////////////////////////////////
//				GAME CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <map>

class GameState;

class Game
{
public :
	// Starts game
	void run();

	inline void setGameState(int stateIndex){pActiveGameState_ = states_[stateIndex];};

	// it's public so that gamestate can access other states easily
	std::map<int,GameState*>		states_;

private :
	///////////////////////////////////
	// MEMBER DATA
	///////////////////////////////////
	sf::VideoMode*					pVideoMode_;
	sf::RenderWindow*				pWindow_;

	GameState*						pActiveGameState_;
	GameState*						pPreviousGameState_;

};

#endif