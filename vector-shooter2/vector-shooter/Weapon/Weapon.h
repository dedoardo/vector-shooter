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

class Weapon
{
public :
	Weapon(){};
	Weapon(std::string name):Name_(name){};
	Weapon(int ammoLeftInLoader,int ammoPerLoader,int loaders,int bulletsPerShot,int id,					// All the values are set 
									 float range,float spread,float damage,float ratio,float bulletSpeed,float reloadTime,		// with Init
									 const std::string& bulletSource,const std::string& crossAirSource);
	int							GetAmmoLeft(){return AmmoLeftInLoader_;};
	int							GetLoadersLeft(){return Loaders_;};


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
	bool						IsReloading_;
	bool						Loaded_;
	unsigned int				VibrationPower_;

	// Graphics
	std::string					BulletSource_;
	std::string					CrossAirSource_;
	sf::Texture					BulletTexture_;
	sf::Texture					CrossAirTexture_;
	sf::Sprite					CrossAirSprite_;

public:

	void						AddLoader_(){Loaders_ += 1;};
	void						AddDamage_(){Damage_ += 10;};

	// Fire
	bool Fire(Entity* entity);

	// Update
	void Update(sf::RenderWindow& window,const sf::Time& deltaTime,Entity* entity);
	void UpdateBullets(sf::RenderWindow& window,const sf::Time& deltaTime,Entity* entity);

	// Reload
	void Reload();

	};
#endif