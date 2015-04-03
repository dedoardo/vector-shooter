/**
==========================================================================
							Weapon System Header
==========================================================================
**/


#ifndef WEAPON_SYSTEM_H_INCLUDED
#define WEAPON_SYSTEM_H_INCLUDED

#include <vector>

#include "../Utility/Enum.h"
#include "../Entity/Entity.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "Weapon.h"

class Game;
class GameRunState;

class WeaponSystem
{
public :
	void					Init(int,Entity&,sf::RenderWindow&,Game&,GameRunState&); // loading all files

	void					Fire();
	void					Update(const sf::Time&); 

	unsigned int			GetActiveWeapon(){return ActiveWeapon_;};
	std::vector<Weapon>*		GetWeapons(){return &Weapons_;};
	void					SetActiveWeapon(unsigned int w){ActiveWeapon_ = w;};
	int						GetTotalRemainingAmmo();
	std::vector<Bullet>*	GetBullets(){return &Bullets_;};
	void					SetActiveWeapon(int);

private :
	std::vector<Weapon>					Weapons_; // map indexed by ID
	std::vector<unsigned int>			VibrationPowers_;
	unsigned int						ActiveWeapon_;

	Entity*								Owner_;
	sf::RenderWindow*					Window_;
	Game*								Game_;
	GameRunState*						GameState_;
	std::vector<Bullet>					Bullets_;

public :
	inline void				IncreaseAmmo(){Weapons_[ActiveWeapon_].AddLoader_();};
};

#endif