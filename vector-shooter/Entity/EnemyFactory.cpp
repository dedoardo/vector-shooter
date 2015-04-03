/**
==========================================================================
						Enemy factory source
==========================================================================
**/

#include "EnemyFactory.h"
#include "../Game/States/GameRunState.h"
#include "../rapidxml/rapidxml.hpp"
#include "../Utility/Enum.h"
#include "../Weapon/WeaponSystem.h"
#include "../Globals.h"

#include <random>
#include <sstream>
#include <fstream>

using namespace rapidxml;

/**=============================
EnemyFactory::EnemyFactory
=============================**/
void EnemyFactory::Init(sf::RenderWindow& window,GameRunState& gamestate,Game& g,std::string fileName,int difficulty)
{
	Window_ = &window;
	GameState_ = &gamestate;
	Difficulty_ = difficulty;
	Game_		= &g;

	// Loading the texture & setting it to enemyinfo struct
	EnemyTexture_ = Globals::Assets->GetTexture("enemy");
	EnemyInfo_.texture = EnemyTexture_;

	// Load the configuration file for the active map
	std::ifstream statStream;
	statStream.open(fileName);

	xml_document<> statDoc;
	std::vector<char> buffer((std::istreambuf_iterator<char>(statStream)),std::istreambuf_iterator<char>());
	buffer.push_back('\0'); // null-terminated string

	statDoc.parse<0>(&buffer[0]);

	// Converting difficulty to integer using stringstream
	std::stringstream ss;
	ss << difficulty;
	std::string D = ss.str();

	xml_node<>* difficultyNode = statDoc.first_node()->first_node("enemyinfo")->first_node(D.c_str());

	// saving data
	EnemyInfo_.speed = std::stof(difficultyNode->first_node("stats")->first_attribute("speed")->value());
	EnemyInfo_.aggressivity = std::stoi(difficultyNode->first_node("stats")->first_attribute("aggressivity")->value());
	EnemyInfo_.respawn = std::stof(difficultyNode->first_node("stats")->first_attribute("respawn")->value());
	NumberOfEnemies_ = std::stoi(difficultyNode->first_node("enemy")->first_attribute("number")->value());

	// closing the stream
	statStream.close();

	// Creating enemies now
	for (int i = 0; i < NumberOfEnemies_; ++i)
	{
		// creating weaponsystem
		WeaponSystem* s = new WeaponSystem();

		// randomizing starting position
		int randomIndex = rand() % GameState_->MapHandler_.GetActiveMap()->GetNodes()->size();
		if (randomIndex > 0) 
			randomIndex -= 1;
		EnemyInfo_.position = (*GameState_->MapHandler_.GetActiveMap()->GetNodes())[randomIndex].GetPosition();

		Enemy* e  = new Enemy(EnemyInfo_,gamestate.MapHandler_,*this,*GameState_);
		
		// giving it a name
		e->Name_ = "Computer " + std::to_string(i);

		// loading weapon system
		s->Init(WEAPON_SYSTEM_ENEMY_DEFAULT,*e,*Window_,*Game_,*GameState_);
		
		e->LoadWeaponSystem(*s);
		
		Enemies_.push_back(e);
		WeaponSystems_.push_back(s);
	}
}

/**=============================
EnemyFactory::~EnemyFactory
=============================**/
EnemyFactory::~EnemyFactory()
{
	// deleting dinamically allocated pointers to Enemy
	// w/8 using fancy iterators
	while (!Enemies_.empty())
	{
		delete Enemies_.back();
		Enemies_.pop_back();
	}

	while (!WeaponSystems_.empty())
	{
		delete WeaponSystems_.back();
		WeaponSystems_.pop_back();
	}
}

/**=============================
EnemyFactory::Update
=============================**/
// Updates all the enemies aswell
void EnemyFactory::Update(const sf::Time& deltaTime)
{
	std::size_t s = Enemies_.size();
	for (int i = 0; i < s; ++i)
	{
		Enemies_[i]->Update(Window_,deltaTime);
	}
}