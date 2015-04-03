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

#include "Entity.h"

#include <vector>
#include <chrono>

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
	void										LoadWeaponSystem(WeaponSystem&);
	sf::Vector2f								GetOrigin(){return Sprite_.getOrigin();};
	inline WeaponSystem*						GetWeaponSystem(){return WeaponSystem_;};
	virtual void								IncreaseAmmo();

	sf::Vector2f								LateralHeading_;

private :
	void										HandleInput(const sf::Time&);

	// stats
	float										LateralSpeed_;

	// local coordinates for the player vertices [ collision detection ]
	std::vector<sf::Vector2f>					PlayerVertices_;
	std::vector<sf::Vector2f>					UpdatedVertices_;
	int											ActiveWeapon_;
	sf::Vector2f								Origin_;

	float										ChangeWeaponTick_;
	float										ChangeWeaponTime_;

	float										PreviousAngle_;

	bool										Colliding_;

	std::chrono::time_point<std::chrono::high_resolution_clock>		DeadTimePoint_;
	bool										StartDeathClock_;

	// Graphics
	sf::Texture*									ActiveTexture_;
	sf::Sprite									Sprite_;

	// some useful pointers
	sf::RenderWindow*							Window_;
	Game*										Game_;
	GameRunState*								GameState_;

};

#endif // player