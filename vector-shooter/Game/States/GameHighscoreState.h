#ifndef GAME_HIGHSCORE_STATE_H_INCLUDED_
#define GAME_HIGHSCORE_STATE_H_INCLUDED_

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "BaseGameState.h"

class Game;

class GameHighscoreState : public GameState
{
public :
	void Loop();
	void Init(sf::RenderWindow&,Game&);

private :
	void			HandleEvents();

	std::string		Filename_;
	sf::Texture*		BackgroundTexture_;

};

#endif // GAME_HIGHSCORE_STATE_H_INCLUDED_