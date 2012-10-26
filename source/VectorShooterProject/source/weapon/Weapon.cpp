

#include "Weapon.h"

BaseWeapon::BaseWeapon()
{
}

BaseWeapon::BaseWeapon(int type,int ammo)
{
	type_ = type;
	ammo_ = ammo;

}

void BaseWeapon::Update(const sf::Time& deltaTime)
{

}
