/**
==========================================================================
								Game File
==========================================================================
**/

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

/**=============================
Includes
=============================**/

#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

/**=============================
Forward declarations
=============================**/

class GameState;
class ControllerXBox;

class Game
{
public :
	Game();
	void Run();
	~Game();
	ControllerXBox*					ControllerXBox_;

private :
	GameState*						ActiveGameState_;
	int								ActiveGameStateIndex_;
	std::vector<GameState*>			States_; // indexed by enumerator

	void							InitStates();

	sf::RenderWindow*				Window_;
	sf::VideoMode*					VideoMode_;

public :
	// Accessors & mutators
	inline void		setGameState(int gameStateID){ActiveGameState_ = States_[gameStateID];};
	inline int		getGameState(){return ActiveGameStateIndex_;};
	inline GameState*	getActiveGameState(){return ActiveGameState_;};

};

#endif // Game Class Included