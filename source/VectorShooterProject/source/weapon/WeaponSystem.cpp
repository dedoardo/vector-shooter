
#include "WeaponSystem.h"

WeaponSystem::WeaponSystem()
{
}

// constructor
void WeaponSystem::Init(sf::RenderWindow& window,std::vector<BaseWeapon>& weapons)
{
	pWindow_ = &window;

	for (int i = 0; i < weapons.size(); i++)
	{
		weapons_[weapons[i].getType()] = weapons[i];
	}
}

void WeaponSystem::Update(const sf::Time& deltaTime)
{
	std::map<int,BaseWeapon>::iterator iter = weapons_.begin();
	for (iter; iter != weapons_.end(); iter++)
	{
		iter->second.Update(deltaTime);
	}
}