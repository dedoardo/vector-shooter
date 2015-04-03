/**
==========================================================================
						Collision File
==========================================================================
**/



#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "../Entity/Entity.h"
#include "../Weapon/Bullet.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

// Forward declarations
class TiledMap;
class Player;
class EnemyFactory;
class GameRunState;

class QuadTree
{
public :
	QuadTree(){};
	QuadTree(float,float,float,float,int,int);
	~QuadTree();

	void		Clear();

	void		AddEntity(Entity*);
	void		AddFixedEntity(Entity*);

	std::vector<Entity*>	GetEntitiesAt(float,float);
	std::vector<Entity*>	GetFixedEntitiesAt(float,float);

private :
	float			X_;
	float			Y_;
	float			Width_;
	float			Height_;
	int				Level_;
	int				MaxLevel_;
	
	QuadTree*		Parent_;
	QuadTree*		RightUp_;
	QuadTree*		LeftUp_;
	QuadTree*		RightDown_;
	QuadTree*		LeftDown_;

	bool			Contains(QuadTree*,Entity*);

	std::vector<Entity*>	Entities_;

	std::vector<Entity*>	FixedEntities_;
};

class CollisionHandler
{
public :
	CollisionHandler(){};
	~CollisionHandler();
	void			Init(TiledMap&,Player&,EnemyFactory&,GameRunState&);
	void			Update(sf::RenderWindow&);

	int				PlayerToWall();
	int				EntityToWall(const Entity&);
	int				BulletToWall(Bullet&,sf::RenderWindow&);

private :

	QuadTree*		QuadTree_;
	TiledMap*		ActiveMap_;
	Player*			Player_;
	EnemyFactory*	EnemyFactory_;
	GameRunState*	GameRunState_;

	static bool IsInside(const sf::Vector2f& point,const Entity* entity2)
	{
		if ((point.x >= entity2->Position_.x && point.x <= (entity2->Position_.x + entity2->Width_)) &&
			(point.y >= entity2->Position_.y && point.y <= (entity2->Position_.y + entity2->Height_)))
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline float CrossProduct(const sf::Vector2<T>& v1,const sf::Vector2<T>& v2)
	{
		return static_cast<float>(v1.x * v2.y - v1.y * v2.x);
	}

	template<typename T>
	inline bool AreIntersecting(sf::Vector2<T>& p,sf::Vector2<T>& pr,sf::Vector2<T>& q, sf::Vector2<T>& qs)
	{
		sf::Vector2f r = pr - p;
		sf::Vector2f s = qs - q;
		// p + t*r and q + us
		// First of all checking if r x s == 0
		if (CrossProduct(r,s) == 0)
			return false; // false the lines are parallel
		float t = CrossProduct((q - p),s) / CrossProduct(r,s);
		float u = CrossProduct((q - p),r) / CrossProduct(r,s);
		if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
			return true;
		else 
			return false;
	}

};

#endif // collision included