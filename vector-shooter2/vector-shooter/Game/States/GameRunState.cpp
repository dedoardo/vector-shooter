/**
==========================================================================
								Game Run State
==========================================================================
**/

#include "GameRunState.h"
#include "../../Graph/Map.h"
#include "../../Entity/Player.h"
#include "../../Weapon/WeaponSystem.h"
#include "../../UI/UI.h"

#include <iostream>
#include <SFML/Window/Event.hpp>

/**=============================
GameRunState::Init
=============================**/

void GameRunState::Init(sf::RenderWindow& window,Game& game)
{
	Window_ = &window;
	Game_	= &game;

	StateChanged_ = false;
}

/**=============================
GameRunState::HandleEvents
=============================**/

void GameRunState::HandleEvents()
{
	sf::Event event;
	while (Window_->pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::Closed :
			exit(0);
		}
	}
}

/**=============================
GameRunState::Loop
=============================**/

void GameRunState::Loop()
{
	// Loading a new Map
	MapHandler NewMap;
	NewMap.Init(*Window_);
	NewMap.NewMap("data//maps//MAP1");

	Player player;
	player.Init(*Window_,*Game_,*this);

	CollisionHandler_.Init(*NewMap.GetActiveMap(),player);

	WeaponSystem w;
	w.Init(WEAPON_SYSTEM_PLAYER_DEFAULT,player,*Window_,*Game_);
	player.LoadWeaponSystem(w);

	UI ui;
	ui.Init(*Window_,player,w,*Game_);

	sf::Clock c;
	int count = 0; 

	while (Window_->isOpen())
	{
		HandleEvents();

		Window_->clear(sf::Color(0,0,0));

		NewMap.Update();
		sf::Time dt = c.restart();
		if (count > 60)
		{
			std::cout << 1.f / dt.asSeconds() << std::endl;
			count = 0;
		}
		++count;

		player.Update(dt);
		CollisionHandler_.Update(*Window_);
		w.Update(dt);
		ui.Update(dt);

		Window_->display();
	}
}