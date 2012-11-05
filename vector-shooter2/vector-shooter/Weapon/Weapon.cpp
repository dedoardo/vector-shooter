/**
==========================================================================
								Weapon Source
==========================================================================
**/

#include "Weapon.h"
#include "../Utility/WindowInfo.h"
/**=========================
Weapon::Weapon
=========================**/

Weapon::Weapon(int ammoLeftInLoader,int ammoPerLoader,int loaders,int bulletsPerShot,int id,					// All the values are set 
								 float range,float spread,float damage,float ratio,float bulletSpeed,float reloadTime,		// with Init
								 const std::string& bulletSource,const std::string& crossAirSource)
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
		
	// Loading textures
	BulletTexture_.loadFromFile(BulletSource_);
	CrossAirTexture_.loadFromFile(CrossAirSource_);
	CrossAirSprite_.setTexture(CrossAirTexture_);

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
void Weapon::Update(sf::RenderWindow& window,const sf::Time& deltaTime,Entity* entity)
{
	if (!Loaded_)
	{
		Loaded_ = true;
		CrossAirSprite_.setTexture(CrossAirTexture_);
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
	CrossAirSprite_.setPosition(entity->Position_.x + (-entity->Heading_.x * Range_) - CrossAirTexture_.getSize().x / 2,entity->Position_.y + (entity->Heading_.y * Range_)- CrossAirTexture_.getSize().y / 2);
	window.draw(CrossAirSprite_);

}

/**=========================
Weapon::UpdateBullets
=========================**/
void Weapon::UpdateBullets(sf::RenderWindow& window,const sf::Time& deltaTime,Entity* entity)
{
	std::size_t s = Bullets_.size();
	for (std::size_t i = 0; i< s; ++i)
	{
		Bullets_[i].Draw(window,deltaTime);
	}

	// Checking if bullets are dead
	if (!Bullets_.empty())
	{
		std::vector<Bullet>::iterator iter = Bullets_.begin();
		for (iter; iter != Bullets_.end();iter)
		{
			if (iter->GetSpeed() <= 0 || iter->GetPosition().x < 0 || iter->GetPosition().y < 0 ||
				iter->GetPosition().x > WINDOW_WIDTH || iter->GetPosition().y > WINDOW_HEIGHT)
				iter = Bullets_.erase(iter);
			else
			{
				++iter;
			}
		}
	}
}


/**==========================
Weapon::Fire
==========================**/
bool Weapon::Fire(Entity* entity)
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
		else 
			return false;
	}
	else
		return false;
	// creating bullets
	sf::Vector2f bulletPos;
	bulletPos.x = (entity->Position_.x + entity->FireVertex_.x) * cos(entity->Angle_) - (entity->Position_.y + entity->FireVertex_.y ) * sin(entity->Angle_);
	bulletPos.y = (entity->Position_.y + entity->FireVertex_.y) * cos(entity->Angle_) + (entity->Position_.x + entity->FireVertex_.x) * sin(entity->Angle_);

	for (std::size_t i = 0; i < BulletsPerShot_; ++i)
	{
		// calculating a random heading
		sf::Vector2f heading;
		float angle = ((atan2(entity->Heading_.y,entity->Heading_.x) * 180 / 3.1415926) - Spread_/2) + (rand() % (int)(Spread_ +1)) ;

		heading.x = cos(angle * 3.1415926 / 180);
		heading.y = sin(angle * 3.1415926 / 180);

		Bullet b(BulletTexture_,entity->Position_,heading,BulletSpeed_);
		Bullets_.push_back(b);
	}
	return true;
}