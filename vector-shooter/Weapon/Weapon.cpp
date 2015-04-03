/**
==========================================================================
								Weapon Source
==========================================================================
**/

#include "Weapon.h"
#include "../Utility/WindowInfo.h"
#include "../Utility/Enum.h"
#include "../Game/Game.h"
#include "../Game/Collision.h"
#include "../Controller/ControllerXBox.h"
#include "../Globals.h"

#include <SFML/Window/Mouse.hpp>

/**=========================
Weapon::Weapon
=========================**/

Weapon::Weapon(int ammoLeftInLoader,int ammoPerLoader,int loaders,int bulletsPerShot,int id,					// All the values are set 
								 float range,float spread,float damage,float ratio,float bulletSpeed,float reloadTime,		// with Init
								 const std::string& bulletSource,const std::string& crossAirSource,Entity& entity,int effectID,float radius)
{
	// Setting Values
	AmmoLeftInLoader_ = ammoLeftInLoader;
	AmmoPerLoader_ = ammoPerLoader;
	Loaders_ = loaders;
	BulletsPerShot_ = bulletsPerShot;
	ID_ = id;
	Range_ = range;
	Spread_ = spread;
	Damage_ = damage;
	Ratio_ = ratio;
	BulletSpeed_ = bulletSpeed;
	ReloadTime_ = reloadTime;
	BulletSource_ = bulletSource;
	CrossAirSource_ = crossAirSource;
	Owner_	= &entity;
	EffectID_ = effectID;
	Radius_ = radius;
		
	// Loading textures
	BulletTexture_ = Globals::Assets->GetTexture(BulletSource_);
	CrossAirTexture_ = Globals::Assets->GetTexture(CrossAirSource_);
	if (CrossAirSource_ != "NONE")
		CrossAirSprite_.setTexture(*CrossAirTexture_);

	TimeCount_ = Ratio_;
	ReloadCount_ = 0;
	Loaded_ = false;
	IsReloading_ = false;
}

/**=========================
Weapon::Reload
=========================**/
void Weapon::Reload()
{
	if (Loaders_ >= 1)
	{
		Loaders_ -= 1;
		IsReloading_ = true;
	} // else you are out of ammo
}


/**=========================
Weapon::Update
=========================**/
void Weapon::Update(sf::RenderWindow& window,const sf::Time& deltaTime,Entity* entity,Game* game)
{
	if (!Loaded_)
	{
		Loaded_ = true;
		if (CrossAirSource_ != "NONE")
			CrossAirSprite_.setTexture(*CrossAirTexture_);
	}
	TimeCount_ += deltaTime.asSeconds();
	if (IsReloading_)
	{
		ReloadCount_ += deltaTime.asSeconds();
		if (ReloadCount_ >= ReloadTime_)
		{
			IsReloading_ = false;
			AmmoLeftInLoader_ = AmmoPerLoader_;
			ReloadCount_ = 0;
		}
	}
	// Drawing the crossair
	if (CrossAirSource_ != "NONE")
	{
		float x = entity->Position_.x + (-entity->Heading_.x * Range_) - CrossAirTexture_->getSize().x / 2;
		float y = entity->Position_.y + (entity->Heading_.y * Range_)- CrossAirTexture_->getSize().y / 2;
		CrossAirSprite_.setPosition(x,y);
	}
	window.draw(CrossAirSprite_);

}


/**==========================
Weapon::Fire
==========================**/
bool Weapon::Fire()
{
	std::size_t bullets = 0;
	// this is the single shot, if there are not enough ammo
	// it's simply gonna fire that that remains
	if (TimeCount_ >= Ratio_ && !IsReloading_)
	{
		if (AmmoLeftInLoader_ - BulletsPerShot_ > 0)
		{
			AmmoLeftInLoader_ -= BulletsPerShot_; 
			bullets = BulletsPerShot_;
			TimeCount_ = 0;
		}
		else if (AmmoLeftInLoader_ > 0) 
		{
			AmmoLeftInLoader_ -= BulletsPerShot_;
			bullets = BulletsPerShot_ - AmmoLeftInLoader_;
			AmmoLeftInLoader_ = 0;
			TimeCount_ = 0;
			Reload();
		}
		else if (AmmoLeftInLoader_ == 0 && Loaders_ > 0)
		{
			Reload();
		}
		else 
			return false;
	}
	else
		return false;
	return true;
}