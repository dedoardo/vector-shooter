/**
==========================================================================
						Game Controller header
==========================================================================
**/


#ifndef GAME_CONTROLLER_H_INCLUDED
#define GAME_CONTROLLER_H_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../Menu/MenuObject.h"

#include "../Utility/Enum.h"

#include <chrono>

/**

	This class is used both as pre-game menu and in-game rule controller

**/

// forward declarations
class Game;
class GameRunState;

class GameController
{
public :
	GameController(){};

	// initializing all internal variables
	void		Init(sf::RenderWindow&,Game&,GameRunState&);

	// pre-game menu
	bool		GetGameInfo();
	void		ShowScores();

	// game rules checker
	bool		IsTimePassed();

	// rule controller
	void		Update();

	void		UpdateHighscores();

	void		Reset();

private :
	sf::RenderWindow*			Window_;
	Game*						Game_;
	GameRunState*				GameState_;

	sf::Font*					MenuFont_;
	sf::Font*					TimeFont_;

	std::vector<sf::Text>		GameModeTexts_;
	std::vector<sf::Text>		DifficultyTexts_;
	std::vector<sf::Text>		TimeTexts_;
	std::vector<sf::Text>		KillLimitTexts_;

	sf::Texture*				HighscoreTexture_;
	sf::Sprite					HighscoreSprite_;

	std::vector<float>			TimeLimits_;
	std::vector<int>			KillLimits_;

	sf::Text					ClockText_;

	sf::Texture*				LeftButtonOffTexture_;
	sf::Texture*				RightButtonOffTexture_;
	sf::Texture*				LeftButtonOnTexture_;
	sf::Texture*				RightButtonOnTexture_;
	sf::Texture*				BackgroundTexture_;
	sf::Sprite					BackgroundSprite_;
	sf::Texture					ClockLayerTexture_;
	sf::Sprite					ClockLayerSprite_;
	
	MenuObject					LeftDifficulty_;
	MenuObject					RightDifficulty_;
	MenuObject					LeftGameMode_;
	MenuObject					RightGameMode_;
	MenuObject					LeftTime_;
	MenuObject					RightTime_;
	MenuObject					LeftKillLimit_;
	MenuObject					RightKillLimit_;


	int							Difficulty_;
	int							GameMode_;
	int							Time_;
	int							KillLimit_;

	bool						ClockStarted_;
	
	// time variables
	std::chrono::high_resolution_clock GameClock_;
	std::chrono::time_point<std::chrono::high_resolution_clock> StartTimePoint_;
	std::chrono::time_point<std::chrono::high_resolution_clock> NowTimePoint_;

public :
	inline bool				   IsNightMode(){if (GameMode_ == GAME_MODE_DAY)return false;return true;};
	inline int				   GetDifficulty(){return Difficulty_;};

};

#endif // game controller included
