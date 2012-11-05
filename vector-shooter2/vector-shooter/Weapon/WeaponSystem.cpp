/**
==========================================================================
							Weapon System Source File
==========================================================================
**/

#include "WeaponSystem.h"
#include "../rapidxml/rapidxml.hpp"
#include "../Game/Game.h"
#include "../Controller/ControllerXBox.h"

#include <fstream>
#include <iostream>

using namespace rapidxml;

/**=============================
WeaponSystem::Init
=============================**/
void WeaponSystem::Init(int systemType,Entity& owner,sf::RenderWindow& window,Game& game)
{
	Owner_ = &owner;
	Window_ = &window;
	Game_ = &game;

	std::string filename;
	if (systemType == WEAPON_SYSTEM_PLAYER_DEFAULT)
		filename = "data//cfg//PlayerWeapons.xml";
	else if (systemType == WEAPON_SYSTEM_ENEMY_DEFAULT)
		filename = "data//cfg//EnemyWeapons.xml";
	else 
		std::cerr << "system type  :  " << systemType << " is not valid" << std::endl;
	

	// opening the file stream
	std::ifstream fileStream(filename);

	xml_document<> weaponDoc;
	std::vector<char> buffer((std::istreambuf_iterator<char>(fileStream)),std::istreambuf_iterator<char>());
	buffer.push_back('\0');

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

		// Initializing weapon
		Weapon w(Ammo,AmmoPerLoader,Loaders,BulletsPerShot,ID,Range,Spread,Damage,Ratio,BulletSpeed,ReloadTime,BulletSource,crossairSource);
		VibrationPowers_.push_back(static_cast<unsigned int>(Power));
		Weapons_.push_back(w);
	}


	// closing the file stream
	fileStream.close();

	ActiveWeapon_ = 3;

}

/**=============================
WeaponSystem::Update
=============================**/
void WeaponSystem::Update(const sf::Time& deltaTime)
{
	Weapons_[ActiveWeapon_].Update(*Window_,deltaTime,Owner_);
	std::size_t s = Weapons_.size();
	for (std::size_t i = 0; i < s; ++i)
	{
		Weapons_[i].UpdateBullets(*Window_,deltaTime,Owner_);
	}
}


/**=============================
WeaponSystem::Fire
=============================**/
void WeaponSystem::Fire()
{
	bool result = Weapons_[ActiveWeapon_].Fire(Owner_);
	if (result = true)
		Game_->ControllerXBox_->Vibrate(VibrationPowers_[ActiveWeapon_],VibrationPowers_[ActiveWeapon_]);
	else
		Game_->ControllerXBox_->StopAllVibrations();
}