#include "EffectHandler.h"
#include "../Entity/Player.h"
#include "../Utility/Enum.h"
#include "../rapidxml/rapidxml.hpp"
#include "../Globals.h"

#include <iostream>
#include <fstream>

using namespace rapidxml;

/** EFFECT CLASS **/
void Effect::Init(sf::Texture& texture,float frametime,sf::IntRect baseRect,bool loop)
{
	Texture_ = &texture;
	FrameTime_ = frametime;
	BaseIntRect_ = baseRect;
	Loop_ = false;

	// loading texture
	Sprite_.setTexture(*Texture_);

	// clearing rects from previous sprite
	Rects_.clear();

	// generating textures
	int max_width = Texture_->getSize().x;
	int max_height = Texture_->getSize().y;
	for (int row = 0; row < max_height; row += BaseIntRect_.height)
	{
		for (int col = 0; col < max_width; col += BaseIntRect_.width)
		{
			sf::IntRect rect(sf::Vector2i(col,row),sf::Vector2i(BaseIntRect_.width,BaseIntRect_.height));
			Rects_.push_back(rect);
		}
	}

	// getting max number of sprites
	MaxRectNumber_ = Rects_.size() -1 ; // MaxRectNumber_ is a valid index

	// setting base variables
	FrameTick_ = 0;
	IsFinished_ = false;
	ActiveRectIndex_ = 0;
	Angle_ = 0;
	Position_ = sf::Vector2f(0,0);
}

void Effect::Draw(sf::RenderWindow& window,sf::Time& deltaTime)
{
	if (ActiveRectIndex_ == MaxRectNumber_ && !Loop_)
	{
		IsFinished_ = true;
		return;
	}

	FrameTick_ += deltaTime.asSeconds();
	if (FrameTick_ >= FrameTime_)
	{
		// adding frame only if index is within boundaries
		if (ActiveRectIndex_ + 1 <= MaxRectNumber_)
		{
			++ActiveRectIndex_;
		}
		else if(ActiveRectIndex_<= MaxRectNumber_ && Loop_) // no need to duoble check
		{
			// resetting count
			ActiveRectIndex_ = 0;
		}

		// resetting frame tick time
		FrameTick_ = 0;
	}

	// setting current rect
	Sprite_.setTextureRect(Rects_[ActiveRectIndex_]);

	Sprite_.setPosition(Position_);
	Sprite_.setRotation(Angle_);

	// drawing it
	window.draw(Sprite_);
}

/** EFFECT HANDLER **/
EffectHandler::EffectHandler()
{
	Player_ = nullptr;
	Window_ = nullptr;
}

EffectHandler::~EffectHandler()
{
	// destroying effect pool and active effect
	while (!EffectPool_.empty())
	{
		delete EffectPool_.back();
		EffectPool_.pop_back();
	}

	while (ActiveEffects_.empty())
	{
		delete ActiveEffects_.back();
		ActiveEffects_.pop_back();
	}
}

void EffectHandler::Init(sf::RenderWindow& window ,Player& player)
{
	// saving pointers
	Window_ = &window;
	Player_ = &player;

	// determining pool size
	PoolSize_ = 50;

	// generating effect pool
	for (int i = 0; i < PoolSize_;++i)
	{
		Effect* e = new Effect;
		EffectPool_.push_back(e);
	}

	std::ifstream fstream;
	fstream.open("data/new-assets/cfg/effects_cfg.xml");

	std::vector<char> buffer((std::istreambuf_iterator<char>(fstream)),std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	
	fstream.close();

	xml_document<> doc;
	doc.parse<0>(&buffer[0]);

	// getting data
	xml_node<>* rootNode = doc.first_node();
	
	// looping through all the effects
	for (xml_node<>* effect = rootNode->first_node("effect");effect; effect = effect->next_sibling())
	{
		int width = std::stoi(effect->first_attribute("width")->value());
		int height = std::stoi(effect->first_attribute("height")->value());
		float frameTime = std::stof(effect->first_attribute("frame")->value());
		bool loop = std::stoi(effect->first_attribute("loop")->value());

		// loading texture and rects

		Textures_.push_back(Globals::Assets->GetTexture(effect->first_attribute("texture")->value()));

		sf::IntRect rect(sf::Vector2i(0,0),sf::Vector2i(width,height));
		Rects_.push_back(rect);

		FrameTimes_.push_back(frameTime);

		Loops_.push_back(loop);
	}
}

void EffectHandler::Update(sf::Time& deltaTime)
{

	std::vector<Effect*>::iterator iter = ActiveEffects_.begin();
	while (iter != ActiveEffects_.end())
	{
		// drawing
		(*iter)->Draw(*Window_,deltaTime);

		// checking if finished
		if ((*iter)->IsFinished())
		{
			// moving it to the pool
			EffectPool_.push_back((*iter));
			iter = ActiveEffects_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void EffectHandler::AddExplosion(unsigned int effectID,sf::Vector2f& position)
{
	// checking if there is some space
	if (!EffectPool_.empty())
	{
		Effect* e = EffectPool_.back();
		e->Init(*Textures_[effectID],FrameTimes_[effectID],Rects_[effectID],Loops_[effectID]);
		e->SetPosition(position);
		ActiveEffects_.push_back(e);
		EffectPool_.pop_back();
	}
	else
	{
		Effect* e = new Effect;
		e->Init(*Textures_[effectID],FrameTimes_[effectID],Rects_[effectID],Loops_[effectID]);
		e->SetPosition(position);
		ActiveEffects_.push_back(e);
	}
}