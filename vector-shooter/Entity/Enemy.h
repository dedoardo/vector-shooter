/**
==========================================================================
							Enemy Header
==========================================================================
**/


#ifndef ENEMY_CLASS_H_INCLUDED
#define ENEMY_CLASS_H_INCLUDED

#include "../Entity/Entity.h"
#include "../Graph/PathFinder.h"
#include "../Graph/Map.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <queue>

class Goal_Think;
class WeaponSystem;
class EnemyFactory;
class GameRunState;

struct EnemyInfo
{
public :
	float speed;
	int aggressivity;
	float respawn;
	sf::Texture* texture;
	sf::Vector2f position;
};

class Enemy : public Entity
{
public :
	Enemy(){};
	~Enemy();
	Enemy(EnemyInfo,MapHandler&,EnemyFactory&,GameRunState&);

	void	LoadWeaponSystem(WeaponSystem&);

	// simple function to pass the call to the pathfinder
	bool	IsPathObstructed(const sf::Vector2f&,const sf::Vector2f&);

	void						Update(sf::RenderWindow*,const sf::Time&);
	void						Respawn();
	void						Fire();

	inline PathFinder*			GetPathFinder(){return PathFinder_;};
	inline WeaponSystem*		GetWeaponSystem(){return WeaponSystem_;};
	inline EnemyFactory*		GetEnemyFactory(){return EnemyFactory_;};
	inline GameRunState*		GetGameState(){return GameState_;};
	virtual void IncreaseAmmo();

private :
	sf::Texture*				Texture_;
	sf::Sprite					Sprite_;
	float						Aggressivity_;
	float						Respawn_;
	float						RespawnTick_;
	Entity*						Target_;

	sf::Vector2f				NextPosition_;
	std::vector<sf::Vector2f>	PathToFollow_;
	sf::Vector2f				LastNodePosition_;

	PathFinder*					PathFinder_;
	Goal_Think*					Goal_Think_;

	EnemyFactory*				EnemyFactory_;
	GameRunState*				GameState_;

	friend class Goal_Seek;
	friend class PathFinder;
	friend class Goal_Think;
	friend class Goal_Attack;
};

#endif // enemy class included