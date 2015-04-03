#ifndef _EFFECT_HANDLER_H_INCLUDED_
#define _EFFECT_HANDLER_H_INCLUDED_

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

// forward declarations
class Player;

class Effect
{
public :
	Effect(){};

	void Init(sf::Texture&,float,sf::IntRect,bool);

	void Draw(sf::RenderWindow&,sf::Time& deltaTime);
	bool IsFinished(){return IsFinished_;};

private :
	std::vector<sf::IntRect>  Rects_;
	sf::Texture*			  Texture_;
	sf::Sprite				  Sprite_;

	sf::IntRect				  BaseIntRect_;
	
	unsigned int		      ActiveRectIndex_;
	unsigned int			  MaxRectNumber_;
	bool					  Loop_;
	bool					  IsFinished_;

	float					  FrameTime_;
	float					  FrameTick_;

	// sprite info
	sf::Vector2f			  Position_;
	float					  Angle_;

public :
	inline void SetPosition(sf::Vector2f& pos){Position_ = sf::Vector2f(pos.x - BaseIntRect_.width  /2,pos.y - BaseIntRect_.height /2);};
	inline void SetAngle(float angle){Angle_ = angle;};
};

class EffectHandler
{
public :
	EffectHandler();
	~EffectHandler();

	void Init(sf::RenderWindow&,Player&);

	void Update(sf::Time&);

private :
	Player*				Player_;
	sf::RenderWindow*	Window_;

	std::vector<Effect*> ActiveEffects_;
	std::vector<Effect*> EffectPool_;
	unsigned int		 PoolSize_;

	// textures
	std::vector<sf::Texture*> Textures_;

	std::vector<float> FrameTimes_;
	std::vector<sf::IntRect> Rects_;
	std::vector<bool> Loops_;

public :

	void				AddExplosion(unsigned int,sf::Vector2f&);

};

#endif // _EFFECT_HANDLER_H_INCLUDED_