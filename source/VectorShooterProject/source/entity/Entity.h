

#ifndef BASEENTITY_H_INCLUDED
#define BASEENTITY_H_INCLUDED

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class BaseEntity 
{
public :
	virtual void Init(sf::RenderWindow&)=0;
	virtual void Update(const sf::Time&)=0;

protected :
	static int						type_;

};

#endif

