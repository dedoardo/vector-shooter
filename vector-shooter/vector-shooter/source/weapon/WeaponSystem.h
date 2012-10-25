


#ifndef WEAPONSYSTEM_H_INCLUED
#define WEAPONSYSTEM_H_INCLUDED

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../utility/Data.h"
#include "Weapon.h"

#include <vector>
#include <map>

class WeaponSystem
{
public :
	WeaponSystem();

	void							Init(sf::RenderWindow&,std::vector<BaseWeapon>&);
	void							Update(const sf::Time&);

	// setters
	float							getDamage(int);
	float							getAmmo(int);

	std::map<int,BaseWeapon>		weapons_;

private :
	sf::RenderWindow*				pWindow_;

};

#endif // !WEAPONSYSTEM_H_INCLUED
