/**
==========================================================================
							Weapon System Source File
==========================================================================
**/

#include "WeaponSystem.h"
#include "../rapidxml/rapidxml.hpp"
#include "../Game/Game.h"
#include "../Controller/ControllerXBox.h"
#include "../Game/States/GameRunState.h"
#include "../Utility/WindowInfo.h"
#include "../Globals.h"

#include <fstream>
#include <iostream>

using namespace rapidxml;

/**=============================
WeaponSystem::Init
=============================**/
void WeaponSystem::Init(int systemType,Entity& owner,sf::RenderWindow& window,Game& game,GameRunState& state)
{
	Owner_ = &owner;
	Window_ = &window;
	Game_ = &game;
	GameState_ = &state;

	ActiveWeapon_ = 0;
	
	xml_document<> weaponDoc;
	std::string filename;

	if (systemType == WEAPON_SYSTEM_PLAYER_DEFAULT)
		filename = "data/new-assets/cfg/player_weapon_cfg.xml";
	else if (systemType == WEAPON_SYSTEM_ENEMY_DEFAULT)
		filename = "data/new-assets/cfg/enemy_weapon_cfg.xml";

	std::ifstream fstream;
	fstream.open(filename);
	std::vector<char> buffer((std::istreambuf_iterator<char>(fstream)),std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	fstream.close();

	weaponDoc.parse<0>(&buffer[0]);

	xml_node<>* weaponsNode = weaponDoc.first_node()->first_node("weapons");
	std::size_t s = atoi(weaponsNode->first_attribute("number")->value());
	
	for (xml_node<>* weaponNode = weaponsNode->first_node("weapon"); weaponNode;
		weaponNode = weaponNode->next_sibling())
	{
		// Getting all the info
		int ID = atoi(weaponNode->first_attribute("id")->value());
		std::string weaponName = weaponNode->first_attribute("name")->value();
		std::string crossairSource = weaponNode->first_node("crossair")->first_attribute("source")->value();
		float Range = atof(weaponNode->first_node("stats")->first_attribute("range")->value());
		float Spread = atof(weaponNode->first_node("stats")->first_attribute("spread")->value());
		unsigned int Power = atoi(weaponNode->first_node("stats")->first_attribute("power")->value());
		float Damage = atof(weaponNode->first_node("stats")->first_attribute("damage")->value());
		float Ratio = atof(weaponNode->first_node("stats")->first_attribute("ratio")->value());
		float BulletSpeed = atof(weaponNode->first_node("bullet")->first_attribute("speed")->value());
		float ReloadTime = atof(weaponNode->first_node("stats")->first_attribute("reload")->value());
		int Ammo = atoi(weaponNode->first_node("stats")->first_attribute("ammo")->value());
		int AmmoPerLoader = atoi(weaponNode->first_node("stats")->first_attribute("ammo")->value());
		int Loaders = atoi(weaponNode->first_node("stats")->first_attribute("loaders")->value());
		int BulletsPerShot = atoi(weaponNode->first_node("bullet")->first_attribute("bulletspershot")->value());
		std::string BulletSource = weaponNode->first_node("bullet")->first_attribute("source")->value();
		int effectID = std::stoi(weaponNode->first_attribute("effect")->value());
		float radius = std::stof(weaponNode->first_node("bullet")->first_attribute("radius")->value());

		// Initializing weapon
		Weapon w(Ammo,AmmoPerLoader,Loaders,BulletsPerShot,ID,Range,Spread,Damage,Ratio,BulletSpeed,ReloadTime,BulletSource,crossairSource,*Owner_,effectID,radius);
		VibrationPowers_.push_back(static_cast<unsigned int>(Power));
		Weapons_.push_back(w);
	}
}

/**=============================
WeaponSystem::Update
=============================**/
void WeaponSystem::Update(const sf::Time& deltaTime)
{
	Weapons_[ActiveWeapon_].Update(*Window_,deltaTime,Owner_,Game_);
	std::size_t s = Bullets_.size();
	for (std::size_t i = 0; i< s; ++i)
	{
		Bullets_[i].Draw(*Window_,deltaTime);
	}

	// Checking if bullets are dead
	if (!Bullets_.empty())
	{
		std::vector<Bullet>::iterator iter = Bullets_.begin();
		for (iter; iter != Bullets_.end();iter)
		{
			if (iter->GetSpeed() <= 0 || iter->GetPosition().x < -50 || iter->GetPosition().y < -50 ||
				iter->GetPosition().x > WINDOW_WIDTH + 50 || iter->GetPosition().y > WINDOW_HEIGHT + 50 )
				iter = Bullets_.erase(iter);
			else
			{
				// otherwise checking if it is colliding with walls ( quadtree)
				int result = GameState_->CollisionHandler_.BulletToWall(*iter,*Window_);
				
				if (result == COLLISION_TRUE)
				{
					iter = Bullets_.erase(iter);
				}
				else 
					++iter;
			}
		}
	}
}

/**=============================
WeaponSystem::GetTotalRemaininAmmo
=============================**/
int WeaponSystem::GetTotalRemainingAmmo()
{
	std::size_t s = Weapons_.size();
	
	int remainingAmmo = 0;

	for (int i = 0; i < s;++i)
	{
		remainingAmmo += Weapons_[i].GetAmmoLeft();
		std::size_t s2 = Weapons_[i].GetLoadersLeft();
		for (int j = 0; j < s2;++j)
		{
			remainingAmmo += Weapons_[i].GetAmmoPerLoader();
		}
	}
	return remainingAmmo;
}

/**=============================
WeaponSystem::Fire
=============================**/
void WeaponSystem::Fire()
{
	bool result = Weapons_[ActiveWeapon_].Fire();
	if (result )
	{
		// creating bullets
		sf::Vector2f bulletPos;
		bulletPos.x = (Owner_->Position_.x + Owner_->FireVertex_.x) * cos(Owner_->Angle_) - (Owner_->Position_.y + Owner_->FireVertex_.y ) * sin(Owner_->Angle_);
		bulletPos.y = (Owner_->Position_.y + Owner_->FireVertex_.y) * cos(Owner_->Angle_) + (Owner_->Position_.x + Owner_->FireVertex_.x) * sin(Owner_->Angle_);

		for (std::size_t i = 0; i < Weapons_[ActiveWeapon_].BulletsPerShot_; ++i)
		{
			// calculating a random heading
			sf::Vector2f heading;
			float angle = (Owner_->Angle_ - Weapons_[ActiveWeapon_].Spread_/2) + (rand() % (int)(Weapons_[ActiveWeapon_].Spread_ +1)) ;

			heading.x = -cos(angle * 3.1415926 / 180);
			heading.y = sin(angle * 3.1415926 / 180);

			Bullet b(*Weapons_[ActiveWeapon_].BulletTexture_,Owner_->Position_,heading,Weapons_[ActiveWeapon_].BulletSpeed_,Weapons_[ActiveWeapon_].Damage_,*Owner_,Weapons_[ActiveWeapon_].Radius_);
			Bullets_.push_back(b);
		}
	}
}
/**=============================
WeaponSystem::SetActiveWeapon
=============================**/
void WeaponSystem::SetActiveWeapon(int w)
{
	ActiveWeapon_ = w;
}