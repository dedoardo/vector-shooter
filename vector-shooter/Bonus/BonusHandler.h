/**
==========================================================================
								Bonus Handler header
==========================================================================
**/

#ifndef BONUS_HANDLER_H_INCLUDED
#define BONUS_HANDLER_H_INCLUDED

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

class Player;
class EnemyFactory;
class Bonus;
class MapHandler;

class BonusHandler
{
public :
	BonusHandler(){};
	void			Init(Player&,EnemyFactory&,sf::RenderWindow&,MapHandler&);
	~BonusHandler();

	void				Update(const sf::Time&);

private :
	int					MaxBonusNumber_;

	Player*				Player_;
	EnemyFactory*		EnemyFactory_;
	MapHandler*			MapHandler_;
	sf::RenderWindow*	Window_;

	std::vector<Bonus*>	Bonuses_;
	
	std::vector<float>		RespawnTimes_;
	std::vector<sf::Texture*>Textures_;
};

#endif // bonus handler