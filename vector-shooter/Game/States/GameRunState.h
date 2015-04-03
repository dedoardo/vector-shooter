/**
==========================================================================
								Game Run State
==========================================================================
**/

/**=============================
Includes
=============================**/

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "BaseGameState.h"
#include "../Collision.h"
#include "../../Entity/Player.h"
#include "../../Graph/Map.h"
#include "../../Weapon/WeaponSystem.h"
#include "../../UI/UI.h"
#include "../../Entity/EnemyFactory.h"
#include "../GameController.h"
#include "../../Bonus/BonusHandler.h"
#include "../EffectHandler.h"

#include <string>

/**=============================
Forward declarations
=============================**/
class Game;

#ifndef GAME_RUN_STATE_H_INCLUDED
#define GAME_RUN_STATE_H_INCLUDED

class GameRunState : public GameState
{
public :
	void Loop();
	void Init(sf::RenderWindow&,Game&);
	void DrawShadow();

	void GameOver();

	void NewGame(const std::string&);

	std::string			DisplayMenu();

	CollisionHandler	CollisionHandler_;
	MapHandler			MapHandler_;
	Player				Player_;
	WeaponSystem		WeaponSystem_;
	UI					UI_;
	EnemyFactory		EnemyFactory_;
	GameController		GameController_;
	BonusHandler		BonusHandler_;
	EffectHandler		EffectHandler_;

private :
	void			HandleEvents();

	// asking for map
	std::string       AskForMap();

	bool				PausedState();

	bool			  GameControllerSet_; 
	bool			  MapSet_;
	bool			  GameOver_;

	sf::RenderTexture BackBuffer_;
	sf::Sprite		  LightSprite_;
	sf::Vector2f	  LightSpriteSize_;
	sf::Sprite		  LightMap_;

	sf::Texture*		  GridTexture_;
	sf::Sprite		  GridSprite_;

	sf::Texture*	  MapChooseTexture_;
	sf::Sprite		  MapChooseSprite_;


	sf::Texture*	  SelectorTexture_;

	std::vector<sf::Texture> PreviewsTextures_;
	std::vector<std::string> FileNames_;
	std::vector<std::string> MapNames_;

	sf::Clock		  Clock_;
};

#endif // main menu state