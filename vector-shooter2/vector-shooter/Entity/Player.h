/**
==========================================================================
						Player file
==========================================================================
**/

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include "Entity.h"

#include <vector>

/**=============================
Forward declarations
=============================**/
class Game;
class GameRunState;
class WeaponSystem;

class Player : public Entity
{
public :
	Player(){};
	void										Init(sf::RenderWindow&,Game&,GameRunState&);

	void										Update(const sf::Time&);

	// Inline accessors
	inline float								GetSpeed()const{return Speed_;};
	inline std::vector<sf::Vector2f>*			GetVertices(){return &PlayerVertices_;}; // this gives the raw vertices with no transformation or position updating
	std::vector<sf::Vector2f>*					GetUpdatedVertices();
	void										LoadWeaponSystem(WeaponSystem&);

private :
	void										HandleInput(const sf::Time&);

	// stats
	float										HitPoints_;
	float										Speed_;
	float										LateralSpeed_;
	float										MaxSpeed_;
	float										Acceleration_;
	

	// local coordinates for the player vertices [ collision detection ]
	std::vector<sf::Vector2f>					PlayerVertices_;
	std::vector<sf::Vector2f>					UpdatedVertices_;
	sf::ConvexShape								PlayerShape_;
	int											ActiveWeapon_;

	float										ChangeWeaponTick_;
	float										ChangeWeaponTime_;

	float										PreviousAngle_;

	bool										Colliding_;

	// Graphics
	sf::Texture									ActiveTexture_;
	sf::Sprite									Sprite_;

	// some useful pointers
	sf::RenderWindow*							Window_;
	Game*										Game_;
	GameRunState*								GameState_;
	WeaponSystem*								WeaponSystem_;

};

#endif // player