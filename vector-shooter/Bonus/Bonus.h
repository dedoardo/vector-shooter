/**
==========================================================================
								Bonus header
					All bonuses are included here, to avoid too many
					files. 
==========================================================================
**/


#ifndef ALL_BONUS_H_INCLUDED
#define ALL_BONUS_H_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Entity;

class Bonus
{
public :
	Bonus(){};

	void Update(sf::RenderWindow&,Entity*,const sf::Time&);
	void Draw(sf::RenderWindow&);
	virtual void GiveBonus(Entity*){};

protected : 
	float						RespawnTime_;
	float						RespawnTick_;
	bool						IsRespawning_;

	float						BoundingRadius_;

	sf::Texture*				Texture_;
	sf::Sprite					Sprite_;
	sf::Vector2f				Position_;
	sf::Vector2f				LastPosition_;
	sf::Vector2f				Center_;

public :
	bool IsActive(){return !IsRespawning_;};

};


class Bonus_Weapon : public Bonus
{
public :
	Bonus_Weapon(sf::Texture&,sf::Vector2f&,float);

	virtual void GiveBonus(Entity*);

};


class Bonus_Health : public Bonus
{
public  :
	Bonus_Health(sf::Texture&,sf::Vector2f&,float);

	virtual void GiveBonus(Entity*);
};

class Bonus_Speed : public Bonus
{
public :
	Bonus_Speed(sf::Texture&,sf::Vector2f&,float);

	virtual void GiveBonus(Entity*);
};

class Bonus_Damage : public Bonus
{
public : 
	Bonus_Damage(sf::Texture&,sf::Vector2f&,float);

	virtual void GiveBonus(Entity*);
};

#endif // all bonuses included