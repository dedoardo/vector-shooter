/**
==========================================================================
								Bonus Handler source
==========================================================================
**/

#include "BonusHandler.h"
#include "Bonus.h"
#include "../Entity/Player.h"
#include "../Entity/EnemyFactory.h"
#include "../Utility/Enum.h"
#include "../Graph/Map.h"
#include "../rapidxml/rapidxml.hpp"
#include "../Globals.h"

#include <random>
#include <fstream>
#include <iostream>

using namespace rapidxml;

/**=============================
BonusHandler::~BonusHandler
=============================**/
BonusHandler::~BonusHandler()
{
	while (!Bonuses_.empty())
	{
		delete Bonuses_.back();
		Bonuses_.pop_back();
	}
}

/**=============================
BonusHandler::Init
=============================**/
void BonusHandler::Init(Player& player,EnemyFactory& enemyFactory,sf::RenderWindow& window,MapHandler& map)
{
	Player_ = &player;
	EnemyFactory_ = &enemyFactory;
	MapHandler_ = &map;
	Window_ = &window;

	std::ifstream fstream;
	fstream.open("data/new-assets/cfg/bonus_cfg.xml");

	std::vector<char> buffer((std::istreambuf_iterator<char>(fstream)),std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	fstream.close();

	xml_document<> doc;
	doc.parse<0>(&buffer[0]);

	xml_node<>* bonuses = doc.first_node();
	MaxBonusNumber_ = std::stoi(bonuses->first_attribute("number_per_map")->value());

	for (xml_node<>* bonus = bonuses->first_node("bonus"); bonus; bonus = bonus->next_sibling())
	{
		std::string id = bonus->first_attribute("id")->value();
		Textures_.push_back(Globals::Assets->GetTexture(id));
		RespawnTimes_.push_back(std::stof(bonus->first_attribute("respawn_time")->value()));
	}

	// creating the bonuses
	for (int i = 0; i < MaxBonusNumber_; ++i)
	{
		// choosing a random bonus
		int n = rand() % 3;

		// choosing a random node in the map
		int randomIndex = rand() % MapHandler_->GetActiveMap()->GetNodes()->size();
		
		//rounding the index a bit
		randomIndex = randomIndex>0?randomIndex:randomIndex-1;

		// taking the position
		sf::Vector2f randomPosition = (*MapHandler_->GetActiveMap()->GetNodes())[randomIndex].GetPosition();

		switch (n)
		{
		case 0 :
			{
				Bonuses_.push_back(new Bonus_Health(*Textures_[BONUS_HEALTH],randomPosition,RespawnTimes_[BONUS_HEALTH] * 10));
				break;
			}
		case 1 :
			{
				Bonuses_.push_back(new Bonus_Weapon(*Textures_[BONUS_WEAPON],randomPosition,RespawnTimes_[BONUS_WEAPON] * 10));
				break;
			}
		case 2 :
			{
				Bonuses_.push_back(new Bonus_Weapon(*Textures_[BONUS_WEAPON],randomPosition,RespawnTimes_[BONUS_WEAPON] * 10));
				break;
			}
		default :
			{
				Bonuses_.push_back(new Bonus_Weapon(*Textures_[BONUS_WEAPON],randomPosition,RespawnTimes_[BONUS_WEAPON] * 10));
				break;
			}
		}

	}

}

/**============================
BonusHandler::Update
=============================**/
void BonusHandler::Update(const sf::Time& deltaTime)
{
	// checking collision for all entities, player included
	// checking player first then all the enemies
	std::size_t s = Bonuses_.size();
	for (int i = 0; i < s; ++i)
	{
		Bonuses_[i]->Update(*Window_,Player_,deltaTime);

		// updating all enemies
		std::size_t enemySize = EnemyFactory_->GetEntities()->size();
		for (int j = 0; j  < enemySize; ++j)
		{
			Bonuses_[i]->Update(*Window_,(*EnemyFactory_->GetEntities())[j],deltaTime);
		}

		// drawing it if active
		if (Bonuses_[i]->IsActive())
			Bonuses_[i]->Draw(*Window_);
	}
}