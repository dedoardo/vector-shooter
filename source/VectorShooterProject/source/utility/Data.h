///////////////////////////////////////////////////////
//				DATA CLASS
//				PLANT ATTACK!
//			developed by sparkon 
//		for the "Vegetation" theme
//////////////////////////////////////////////////////

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <string>
#include <vector>
#include <list>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/ConvexShape.hpp>


// Enumator for game_state
// Simply used to check if a game state is run for the first time
enum {
	NEW_GAME_STATE,
	OLD_GAME_STATE
};

namespace state{
// Enumerator for game states
enum {
	MAIN_MENU,
	GAME_RUN,
	OPTIONS,
	CREDITS
};

}

namespace path
{
	enum
	{
		DFS_SEARCH,
		BFS_SEARCH,
		DIJKSTRA_SEARCH,
		A_STAR_SEARCH
	};
}

namespace node
{
	enum
	{
		NODE_VISITED,
		NODE_UNVISITED
	};

	static int NO_PARENT_ASSIGNED = -1;
	static int NO_NODE_EXISTS = -1;
	static int NO_COST_ASSIGNED = -1;
}

namespace weapon
{
	enum
	{
		HAND_GUN,
		SHOT_GUN,
		RAY_GUN
	};
}

// Window Data
static unsigned int WINDOW_WIDTH = 1280;
static unsigned int WINDOW_HEIGHT = 768;
static bool			WINDOW_FULLSCREEN = true;
static unsigned int WINDOW_FPS = 120;
static std::string  WINDOW_CAPTION = "Vector shooter";

static double MATH_PI = 3.1415926;

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


// Overloading a couple of vector operations
template <typename T,typename U>
inline sf::Vector2<float> operator -(const sf::Vector2<T>& left,const sf::Vector2<U>& right)
{
	return sf::Vector2<float>(static_cast<float>(left.x - right.x),
							  static_cast<float>(left.y - right.y));
}

template <typename T,typename U>
inline sf::Vector2<float> operator *(const sf::Vector2<T>& left,const sf::Vector2<U>& right)
{
	return sf::Vector2<float>(static_cast<float>(left.x * right.x),
							  static_cast<float>(left.y * right.y));
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

template<typename T>
inline float getMagnitude(const sf::Vector2<T>& vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}


template<typename T>
inline void Normalize(sf::Vector2<T>& vector)
{
	float magnitude = getMagnitude(vector);
	vector.x /= magnitude;
	vector.y /= magnitude;
}

// operation with 2d matrix
template<typename T>
inline sf::Vector2<T> Rotate(sf::Vector2<T>& vector,float angle)
{
	sf::Vector2<T> rotatedVector;
	rotatedVector.x = static_cast<T>(vector.x * cos(angle) + vector.y * -sin(angle));
	rotatedVector.y = static_cast<T>(vector.x * sin(angle) + vector.y * cos(angle));
	return rotatedVector;
}



///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////



template<typename T>
inline float DotProduct(sf::Vector2<T> v1,sf::Vector2<T> v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

template<typename T>
inline sf::Vector2<T> Project(const std::vector<sf::Vector2<T> >& shape,const sf::Vector2<T>& axis)
{
	float dotProduct = DotProduct(shape[0],axis);
	float min = dotProduct;
	float max = dotProduct;
	for ( int i = 0; i < shape.size();i++)
	{
		dotProduct = DotProduct(shape[i],axis);
		if (dotProduct < min)
			min = dotProduct;
		else if (dotProduct > max)
			max = dotProduct;
	}
	return sf::Vector2<T>(min,max);
}

template<typename T>
inline sf::Vector2<T> Project(sf::ConvexShape& shape,const sf::Vector2<T>& axis)
{
	float dotProduct = DotProduct(shape.getPoint(0),axis);
	float min = dotProduct;
	float max = dotProduct;
	for ( int i = 0; i < shape.getPointCount();i++)
	{
		dotProduct = DotProduct(shape.getPoint(i),axis);
		if (dotProduct < min)
			min = dotProduct;
		else if (dotProduct > max)
			max = dotProduct;
	}
	return sf::Vector2<T>(min,max);
}

template<typename T>
inline float  Interval(const sf::Vector2<T>& v1,const sf::Vector2<T>& v2)
{
	if (v1.x < v2.x)
		return v2.x - v1.x;
	else
		return v1.x - v2.x;
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

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

template <typename T>
bool isInside(const sf::Vector2<T>& test,const std::vector<sf::Vector2<T> >& container)
{
	for (int i = 0; i < container.size(); i++)
	{
		if (container[i].x == test.x && container[i].y == test.y)
			return true;
	}
	return false;
}

template <typename T>
bool isInside(const sf::Vector2<T>& test,const std::list<sf::Vector2<T> >& container )
{
	std::list<sf::Vector2<T> >::const_iterator iter = container.begin();
	for(; iter != container.end(); iter++)
	{
		if ( iter->x == test.x && iter->y == test.y)
			return true;
	}
	return false;
}

template <typename T>
bool isValid(const sf::Vector2<T>& vector)
{
	if (vector.x >= 0 && vector.x <= WINDOW_WIDTH && vector.y >= 0 && vector.y <= WINDOW_HEIGHT)
		return true;
	return false;
}


#endif // Data