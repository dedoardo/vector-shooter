/**
==========================================================================
								Bonus sources
==========================================================================
**/

#include "../Entity/Entity.h"
#include "Bonus.h"


void Bonus::Draw(sf::RenderWindow& window)
{
	Sprite_.setPosition(Position_);
	window.draw(Sprite_);
}

void Bonus::Update(sf::RenderWindow& window,Entity* entity,const sf::Time& deltaTime)
{
	if(IsRespawning_)
	{
		// checking if it's time to respawn
		RespawnTick_ += deltaTime.asSeconds();
	
		if (RespawnTick_ >= RespawnTime_)
		{
			// activating it
			RespawnTick_ = 0;
			IsRespawning_ = false;
		}

	}
	else
	{
		// otherwise checking if it's colliding with an entity
		// i'm not using any square root, instead i'm working with
		// squared values, because it's way faster
		sf::Vector2f distance = entity->Position_ - Center_;
		float squaredMagnitude = distance.x * distance.x + distance.y * distance.y;
		
		// checking if it's in the bounding radius
		if (squaredMagnitude <= (BoundingRadius_ * BoundingRadius_))
		{
			// setting it as dead
			RespawnTick_ = 0;
			IsRespawning_ = true;
			GiveBonus(entity);
		}
	}
}

/**========================================================================
				WEAPON BONUS
=========================================================================**/
Bonus_Weapon::Bonus_Weapon(sf::Texture& texture,sf::Vector2f& position,float respawnTime)
{
	RespawnTime_ = respawnTime;
	Texture_ = &texture;
	Position_ = position;
	IsRespawning_ = false;
	BoundingRadius_ = Texture_->getSize().x * 1.5;

	Center_.x = Position_.x + Texture_->getSize().x /2;
	Center_.y = Position_.y + Texture_->getSize().y /2;

	// setting the texture
	Sprite_.setTexture(*Texture_);
}

void Bonus_Weapon::GiveBonus(Entity* e)
{
	e->IncreaseAmmo();
}

/**========================================================================
				HEALTH BONUS
=========================================================================**/
Bonus_Health::Bonus_Health(sf::Texture& texture,sf::Vector2f& position,float respawnTime)
{
	RespawnTime_ = respawnTime;
	Texture_ = &texture;
	Position_ = position;
	IsRespawning_ = false;
	BoundingRadius_ = Texture_->getSize().x * 1.5;

	Center_.x = Position_.x + Texture_->getSize().x /2;
	Center_.y = Position_.y + Texture_->getSize().y /2;

	// setting the texture
	Sprite_.setTexture(*Texture_);
}

void Bonus_Health::GiveBonus(Entity* e)
{
	e->HitPoints_ += 25 + rand() % 75;
}

/**========================================================================
				SPEED BONUS
=========================================================================**/
Bonus_Speed::Bonus_Speed(sf::Texture& texture,sf::Vector2f& position,float respawnTime)
{

}

void Bonus_Speed::GiveBonus(Entity* e)
{

}


/**========================================================================
				DAMAGE BONUS
=========================================================================**/
Bonus_Damage::Bonus_Damage(sf::Texture& texture,sf::Vector2f& position,float respawnTime)
{

}

void Bonus_Damage::GiveBonus(Entity* e)
{

}