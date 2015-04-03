/**
==========================================================================
								Weapon Header
==========================================================================
**/


#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Bullet.h"
#include "../Entity/Entity.h"

class Game;
class CollisionHandler;

class Weapon
{
public :
	Weapon(){};
	Weapon(std::string name):Name_(name){};
	Weapon(int ammoLeftInLoader,int ammoPerLoader,int loaders,int bulletsPerShot,int id,					// All the values are set 
									 float range,float spread,float damage,float ratio,float bulletSpeed,float reloadTime,		// with Init
									 const std::string& bulletSource,const std::string& crossAirSource,Entity&,int effectID,float radius);
	int							GetAmmoLeft(){return AmmoLeftInLoader_;};
	int							GetLoadersLeft(){return Loaders_;};
	int							GetAmmoPerLoader(){return AmmoPerLoader_;};

private :
	std::vector<Bullet>			Bullets_;
	int							ID_;
	std::string					Name_;

	// Weapon stats
	int							AmmoLeftInLoader_;
	int							AmmoPerLoader_;
	int							Loaders_;
	int							BulletsPerShot_;
	float						Range_;
	float						Spread_;
	float						Damage_;
	float						Ratio_;
	float						BulletSpeed_;
	float						ReloadTime_;
	float						TimeCount_;
	float						ReloadCount_;
	float						Radius_;
	bool						IsReloading_;
	bool						Loaded_;
	unsigned int				VibrationPower_;
	int						    EffectID_;

	// Graphics
	std::string					BulletSource_;
	std::string					CrossAirSource_;
	sf::Texture*				BulletTexture_;
	sf::Texture*				CrossAirTexture_;
	sf::Sprite					CrossAirSprite_;
	Entity*						Owner_;

public:

	void						AddLoader_(){Loaders_ += 1;};
	void						AddDamage_(){Damage_ += 10;};

	int							GetEffectID(){return EffectID_;};

	// Fire
	bool Fire();

	// Update
	void Update(sf::RenderWindow& window,const sf::Time& deltaTime,Entity* entity,Game* game);

	// Reload
	void Reload();

	friend class WeaponSystem;

	};
#endif