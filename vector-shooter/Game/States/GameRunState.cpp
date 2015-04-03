/**
==========================================================================
								Game Run State
==========================================================================
**/

#include "GameRunState.h"
#include "../../Graph/Map.h"
#include "../../Utility/WindowInfo.h"
#include "../../Bonus/BonusHandler.h"
#include "../../Utility/Enum.h"
#include "../../Controller/ControllerXBox.h"
#include "../../Globals.h"
#include "../../rapidxml/rapidxml.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <cassert>

using namespace rapidxml;

/**=============================
GameRunState::Init
=============================**/

void GameRunState::Init(sf::RenderWindow& window,Game& game)
{
	Window_ = &window;
	Game_	= &game;

	MapNames_.clear();
	PreviewsTextures_.clear();

	StateChanged_ = false;
	GameControllerSet_ = false;
	MapSet_ = false;

	// loading texture
	MapChooseTexture_ = Globals::Assets->GetTexture("map_choose_bg");
	MapChooseSprite_.setTexture(*MapChooseTexture_);
	SelectorTexture_ = Globals::Assets->GetTexture("over_thumbnail");

	// preloading all textures
	// loading the file 
	std::ifstream fstream;
	fstream.open("data/new-assets/cfg/maps_cfg.xml");

	std::vector<char> buffer((std::istreambuf_iterator<char>(fstream)),std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	xml_document<> doc;
	doc.parse<0>(&buffer[0]);

	xml_node<>* maps = doc.first_node();

	for (xml_node<>* map = maps->first_node("map"); map; map = map->next_sibling())
	{
		MapNames_.push_back(map->first_attribute("name")->value());
		FileNames_.push_back(map->first_attribute("data")->value());
		PreviewsTextures_.push_back(*Globals::Assets->GetTexture(map->first_attribute("thumbnail")->value()));
	}

	// initializing only the controller
	GameController_.Init(*Window_,*Game_,*this);
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
		case sf::Event::GainedFocus :
			Clock_.restart();
			break;
		case sf::Event::KeyPressed :
			if (event.key.code == sf::Keyboard::Escape)
			{
				bool result = PausedState();
				Clock_.restart();
			}
		}
	}
}

/**=============================
GameRunState::DrawShadow
=============================**/
void GameRunState::DrawShadow()
{
	// clearing back buffer with black
	BackBuffer_.clear(sf::Color(0,0,0,127));

	// drawing light on top of the player
	LightSprite_.setPosition(Player_.Position_.x - LightSpriteSize_.x / 2,Player_.Position_.y - LightSpriteSize_.y / 2);
	BackBuffer_.draw(LightSprite_);

	BackBuffer_.display();

	// now drawing everything on the lightmap
	LightMap_.setTexture(BackBuffer_.getTexture());

	// drawing on actual screen & multiply blending
	Window_->draw(LightMap_,sf::BlendMultiply);
}

/**=============================
GameRunState::Loop
=============================**/

void GameRunState::Loop()
{

	std::string map;

	int menuCount = 0;

	while (true)
	{
		Sleep(200);
		if (menuCount == 0)
		{
			GameControllerSet_ = GameController_.GetGameInfo();
			if (GameControllerSet_ == false)
			{
				setState(MAIN_MENU_STATE);
				StateChanged_ = false;
				return;
			}
			else
			{
				++menuCount;
			}
		}
		else if (menuCount == 1)
		{
			map = AskForMap();
			if (map.size() == 0)
			{
				--menuCount;
			}
			else
				++menuCount;
		}
		else
			break;
	}
	int count = 0; 

	// loading everything
	NewGame(map);

	BackBuffer_.create(WINDOW_WIDTH,WINDOW_HEIGHT);
	sf::Texture* lightTexture;
	lightTexture = Globals::Assets->GetTexture("light_mask");
	LightSprite_.setTexture(*lightTexture);
	LightSpriteSize_.x  = static_cast<float>(lightTexture->getSize().x);
	LightSpriteSize_.y  = static_cast<float>(lightTexture->getSize().y);

	Clock_.restart();

	while (Window_->isOpen())
	{
		//checking if game over event was popped
		if (GameOver_)
		{
			GameController_.ShowScores();
			// sending the player back to the main menu
			setState(MAIN_MENU_STATE);

			// returning to main menu
			return;
		}

		HandleEvents();

		MapHandler_.Update();
		sf::Time dt = Clock_.restart();
		if (count > 60)
		{
			std::cout << 1.f / dt.asSeconds() << std::endl;
			count = 0;
		}
		++count;
		// Drawing grid
		Window_->draw(GridSprite_);
		
		// drawing everything else
		Player_.Update(dt);
		WeaponSystem_.Update(dt);
		EnemyFactory_.Update(dt);
		BonusHandler_.Update(dt);
		EffectHandler_.Update(dt);
		
		CollisionHandler_.Update(*Window_);

		if (GameController_.IsNightMode())
		{
			DrawShadow();
		}
		UI_.Update(dt);

		GameController_.Update();
		Window_->display();
	}
}

/**=============================
GameRunState::AskForMap
=============================**/
std::string GameRunState::AskForMap()
{
	std::string map;

	// map preview
	sf::Sprite previewSprite;
	previewSprite.setTexture(PreviewsTextures_[0]);
	previewSprite.setPosition(WINDOW_WIDTH / 2 - PreviewsTextures_[0].getSize().x /2,WINDOW_HEIGHT / 2 - PreviewsTextures_[0].getSize().y / 2);

	sf::Sprite selector;
	selector.setTexture(*SelectorTexture_);
	selector.setPosition(WINDOW_WIDTH / 2 - SelectorTexture_->getSize().x /2,WINDOW_HEIGHT /2 - SelectorTexture_->getSize().y / 2);

	int activeMap = 0;
	bool mapChosen = false;

	float choiceTime = 0.2;
	float choiceTick = choiceTime;

	sf::Font* f;
	f = Globals::Assets->GetFont("capture");

	std::vector<sf::Text> texts;
	std::size_t s = MapNames_.size();
	for (int i = 0; i < s;++i)
	{
		sf::Text t;
		t.setFont(*f);
		t.setString(MapNames_[i]);
		t.setPosition(WINDOW_WIDTH / 2- t.getLocalBounds().width /2,WINDOW_HEIGHT / 3 - t.getLocalBounds().height /2);		

		texts.push_back(t);
	}

	sf::Clock clock;
	
	while (!mapChosen)
	{
		// getting elapsed time
		choiceTick += clock.restart().asSeconds();

		/** Handling events **/
		sf::Event event;
		while (Window_->pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed :
				{
					Window_->close();
					exit(1);
					break;
				}
			}

			if (Game_->ControllerXBox_->IsConnected())
			{
				if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
					return FileNames_[activeMap];
				if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
					return "";
				if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
				{
					if (activeMap - 1 >= 0 && choiceTick >= choiceTime)
					{
						--activeMap;
						choiceTick = 0;
					}
				}
				else if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
				{
					if (activeMap +1 < FileNames_.size() && choiceTick >= choiceTime)
					{
						++activeMap;
						choiceTick = 0;
					}
				}
			}
			else
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					return FileNames_[activeMap];
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
				{
					return "";
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					if (activeMap -1 >= 0 && choiceTick >= choiceTime)
					{
						--activeMap;
						choiceTick = 0;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					if (activeMap + 1 < FileNames_.size() && choiceTick >= choiceTime)
					{
						++activeMap;
						choiceTick = 0;
					}
				}
			}
		}

		// setting texture
		previewSprite.setTexture(PreviewsTextures_[activeMap]);

		// clearing the window
		Window_->draw(MapChooseSprite_);
		Window_->draw(previewSprite);
		Window_->draw(selector);

		Window_->draw(texts[activeMap]);

		// updating window
		Window_->display();
	}


	return "open_combat";
}

bool GameRunState::PausedState()
{
	bool paused = true;
	
	sf::ConvexShape Polygon;
	Polygon.setPointCount(4);

	assert(Polygon.getPointCount() > 0);

	Polygon.setPoint(0,sf::Vector2f(0.f,0.f));
	Polygon.setPoint(1,sf::Vector2f(150.f,0.f));
	Polygon.setPoint(2,sf::Vector2f(150.f,300.f));
	Polygon.setPoint(3,sf::Vector2f(0.f,300.f));
	Polygon.setFillColor(sf::Color(0,198,0,60));

	Polygon.setPosition(WINDOW_WIDTH / 2 - 75.f,WINDOW_HEIGHT / 2 - 150.f);

	// i'm drawing only once, because of transparency
	// if i draw it in the loop the transparency will be
	// eliminated
	Window_->draw(Polygon);
	Window_->display();

	while (paused)
	{
		sf::Event e;
		while (Window_->pollEvent(e))
		{
			if (e.type == e.KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Escape)
				{
					paused = false;
				}
			}
			if (e.type == sf::Event::Closed)
			{
				exit(1);
			}
		}
	}
	return true;
}


/**=============================
GameRunState::GameOver
=============================**/
void GameRunState::GameOver()
{
	GameOver_ = true;
}

/**=============================
GameRunState::NewGame
=============================**/
void GameRunState::NewGame(const std::string& map)
{
	GameOver_ = false;

	// Loading a new Map
	MapHandler_ = MapHandler();
	MapHandler_.Init(*Window_);
	MapHandler_.NewMap(map);

	// Initializing everything

	Player_.Init(*Window_,*Game_,*this);

	WeaponSystem_ = WeaponSystem();
	WeaponSystem_.Init(WEAPON_SYSTEM_PLAYER_DEFAULT,Player_,*Window_,*Game_,*this);
	Player_.LoadWeaponSystem(WeaponSystem_);

	EnemyFactory_ = EnemyFactory();
	EnemyFactory_.Init(*Window_,*this,*Game_,"data//new-assets//cfg//difficulty_cfg.xml",GameController_.GetDifficulty());

	CollisionHandler_.Init(*MapHandler_.GetActiveMap(),Player_,EnemyFactory_,*this);

	// Loading grid
	GridTexture_ = Globals::Assets->GetTexture("grid");
	GridSprite_.setTexture(*GridTexture_);
	
	BonusHandler_ = BonusHandler();
	BonusHandler_.Init(Player_,EnemyFactory_,*Window_,MapHandler_);

	UI_.Init(*Window_,Player_,WeaponSystem_,*Game_);

	EffectHandler_.Init(*Window_,Player_);

}