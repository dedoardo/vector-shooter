/**
==========================================================================
						Enemy factory header
==========================================================================
**/

#ifndef ENEMY_FACTORY_H_INCLUDED
#define ENEMY_FACTORY_H_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>

#include "../Entity/Enemy.h"

#include <string>

class GameRunState;
class Game;
class WeaponSystem;

class EnemyFactory
{
public :
	EnemyFactory(){};
	~EnemyFactory();
	
	void				Init(sf::RenderWindow&,GameRunState&,Game&,std::string fileName,int);
	void				Update(const sf::Time&);
	
	inline std::vector<Enemy*>* GetEntities(){return &Enemies_;};

private :
	sf::RenderWindow*				Window_;
	GameRunState*					GameState_;
	Game*							Game_;

	std::vector<Enemy*>				Enemies_;
	int								Difficulty_;
	sf::Texture*						EnemyTexture_;
	std::vector<WeaponSystem*>		WeaponSystems_;

	// single entity info
	EnemyInfo						EnemyInfo_;

	// more game info
	int								NumberOfEnemies_;

};

#endif // Included