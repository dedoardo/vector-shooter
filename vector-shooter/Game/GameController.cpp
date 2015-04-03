/**
==========================================================================
						Game Controller Source
==========================================================================
**/

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../Utility/WindowInfo.h"
#include "GameController.h"
#include "../Menu/MenuObject.h"
#include "../Utility/WindowInfo.h"
#include "../Game/Game.h"
#include "../Game/States/GameRunState.h"
#include "../Controller/ControllerXBox.h"
#include "../Globals.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>

/** STRUCT FOR SORTING **/
struct PlayerRecord
{
	std::string name;
	int			score;
};

bool compare(const PlayerRecord& left,const PlayerRecord& right)
{
	return left.score > right.score;
}

// compare function for highscore sorting
bool KillSort(const Entity* a,const Entity* b)
{
	return a->Kills_ < b->Kills_;
}

/**=============================
GameController::Init
=============================**/
void GameController::Init(sf::RenderWindow& window,Game& game,GameRunState& state)
{
	Window_ = &window;
	Game_ = &game;
	GameState_ = &state;

	// initializing variables
	Difficulty_ = DIFFICULTY_NORMAL;
	GameMode_ = GAME_MODE_DAY;

	// initializing game info
	MenuFont_ = Globals::Assets->GetFont("sub");
	TimeFont_ = Globals::Assets->GetFont("ehsmb");

	// loading clock
	ClockText_.setColor(sf::Color(0,255,0));
	ClockText_.setFont(*TimeFont_);
	ClockText_.setString("99 : 99"); // setting maximum string
	ClockText_.setPosition(WINDOW_WIDTH - ClockText_.getLocalBounds().width,0);

	HighscoreTexture_ = Globals::Assets->GetTexture("end_game_bg");
	HighscoreSprite_.setTexture(*HighscoreTexture_);

	// creating clock layer
	sf::Image img;
	img.create(ClockText_.getLocalBounds().width + 10,ClockText_.getLocalBounds().height + 10,sf::Color(0,0,0,220));
	ClockLayerTexture_.loadFromImage(img);
	ClockLayerSprite_.setTexture(ClockLayerTexture_);
	ClockLayerSprite_.setPosition(WINDOW_WIDTH - ClockLayerTexture_.getSize().x,0);

	float x = WINDOW_WIDTH / 2;

	TimeLimits_.push_back(999);
	TimeLimits_.push_back(30.f);
	TimeLimits_.push_back(120.f);
	TimeLimits_.push_back(180.f);
	TimeLimits_.push_back(300.f);

	KillLimits_.push_back(999);
	KillLimits_.push_back(10);
	KillLimits_.push_back(15);
	KillLimits_.push_back(30);

	// creating texts

	sf::Text t;
	t.setFont(*MenuFont_);
	t.setCharacterSize(20);
	t.setString("Easy Mode");
	t.setPosition(x - t.getLocalBounds().width /2,200);
	DifficultyTexts_.push_back(t);
	t.setString("Normal Mode");
	t.setPosition(x - t.getLocalBounds().width /2,200);
	DifficultyTexts_.push_back(t);
	t.setPosition(x - t.getLocalBounds().width /2,200);
	t.setString("Hard Mode");
	DifficultyTexts_.push_back(t);

	t.setString("Day Mode");
	t.setPosition(x - t.getLocalBounds().width /2,250);
	GameModeTexts_.push_back(t);
	t.setString("Night Mode");
	t.setPosition(x - t.getLocalBounds().width /2,250);
	GameModeTexts_.push_back(t);

	t.setString("No Time Limit");
	t.setPosition(x - t.getLocalBounds().width /2,300);
	TimeTexts_.push_back(t);
	t.setString("0:30 m");
	t.setPosition(x - t.getLocalBounds().width /2,300);
	TimeTexts_.push_back(t);
	t.setString("2:00 m");
	t.setPosition(x - t.getLocalBounds().width /2,300);
	TimeTexts_.push_back(t);
	t.setString("3.00 m");
	t.setPosition(x - t.getLocalBounds().width /2,300);
	TimeTexts_.push_back(t);
	t.setString("5.00 m");
	t.setPosition(x - t.getLocalBounds().width /2,300);
	TimeTexts_.push_back(t);

	t.setString("No Kill Limit");
	t.setPosition(x - t.getLocalBounds().width /2,350);
	KillLimitTexts_.push_back(t);
	t.setString("10 Kill");
	t.setPosition(x - t.getLocalBounds().width / 2,350);
	KillLimitTexts_.push_back(t);
	t.setString("15 Kill");
	t.setPosition(x - t.getLocalBounds().width /2,350);
	KillLimitTexts_.push_back(t);
	t.setString("30 Kill");
	t.setPosition(x - t.getLocalBounds().width /2,350);
	KillLimitTexts_.push_back(t);

	RightButtonOffTexture_ = Globals::Assets->GetTexture("right_button_off");
	RightButtonOnTexture_ = Globals::Assets->GetTexture("right_button_on");
	LeftButtonOffTexture_ = Globals::Assets->GetTexture("left_button_off");
	LeftButtonOnTexture_ = Globals::Assets->GetTexture("left_button_on");

	LeftGameMode_.Init(*LeftButtonOnTexture_,*LeftButtonOffTexture_,sf::Vector2f(x - 200,250),"",0);
	RightGameMode_.Init(*RightButtonOnTexture_,*RightButtonOffTexture_,sf::Vector2f(x +200 - RightButtonOffTexture_->getSize().x,250),"",0);
	LeftDifficulty_.Init(*LeftButtonOnTexture_,*LeftButtonOffTexture_,sf::Vector2f(x - 200,200),"",0);
	RightDifficulty_.Init(*RightButtonOnTexture_,*RightButtonOffTexture_,sf::Vector2f(x + 200 - RightButtonOffTexture_->getSize().x,200),"",0);
	LeftTime_.Init(*LeftButtonOnTexture_,*LeftButtonOffTexture_,sf::Vector2f(x - 200,300),"",0);
	RightTime_.Init(*RightButtonOnTexture_,*RightButtonOffTexture_,sf::Vector2f(x + 200 - RightButtonOffTexture_->getSize().x,300),"",0);
	LeftKillLimit_.Init(*LeftButtonOnTexture_,*LeftButtonOffTexture_,sf::Vector2f(x - 200,350),"",0);
	RightKillLimit_.Init(*RightButtonOnTexture_,*RightButtonOffTexture_,sf::Vector2f(x + 200 - RightButtonOffTexture_->getSize().x,350),"",0);

	BackgroundTexture_ = Globals::Assets->GetTexture("game_mode_bg");
	BackgroundSprite_.setTexture(*BackgroundTexture_);
}


/**=============================
GameController::GetGameInfo
=============================**/
bool GameController::GetGameInfo()
{
	// internal clock
	sf::Clock clock;
	ClockStarted_ = false;

	int DifficultySelection = DIFFICULTY_NORMAL;
	int GameModeSelection = GAME_MODE_DAY;
	int TimeSelection = GAME_2_00;
	int KillLimitSelection = GAME_KILL_NO_LIMIT;

	// this is used for xbox controller selection 0 is the difficulty menu
	// 1 i
	int ActiveMenu = 0;

	float choiceTime = 0.2;
	float choiceTick = choiceTime;

	while (Window_->isOpen())
	{
		// updating choice tick
		choiceTick += clock.restart().asSeconds();

		// deactivating all the selections
		LeftDifficulty_.IsActive_ = false;
		RightDifficulty_.IsActive_ = false;
		LeftGameMode_.IsActive_ = false;
		RightGameMode_.IsActive_ = false;
		LeftTime_.IsActive_ = false;
		RightTime_.IsActive_ = false;
		LeftKillLimit_.IsActive_ = false;
		RightKillLimit_.IsActive_ = false;

		// Handling events
		sf::Event event;
		while (Window_->pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed :
				{
					Window_->close();
					exit(0);
					break;
				}
			}

			// Handling controls
			if (Game_->ControllerXBox_->IsConnected())
			{
				if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
				{
					if (choiceTick >= choiceTime)
					{
						if (ActiveMenu == 0 && DifficultySelection - 1 >= 0)
						{
							--DifficultySelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 1 && GameModeSelection -1 >= 0)
						{
							--GameModeSelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 2 && TimeSelection -1 >= 0)
						{
							--TimeSelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 3 && KillLimitSelection -1 >= 0)
						{
							--KillLimitSelection;
							choiceTick = 0;
						}
					}
				}
				else if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
				{
					if (choiceTick >= choiceTime) 
					{
						if (ActiveMenu == 0 && DifficultySelection + 1 < 3)
						{
							++DifficultySelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 1 && GameModeSelection +1 < 2)
						{
							++GameModeSelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 2 && TimeSelection +1 < 5)
						{
							++TimeSelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 3 && KillLimitSelection +1 < 4)
						{
							++KillLimitSelection;
							choiceTick = 0;
						}
					}
				}
				else if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					if (ActiveMenu + 1 < 4 && choiceTick >= choiceTime)
					{
						++ActiveMenu;
						choiceTick = 0;
					}
				}
				else if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					if (ActiveMenu -1 >= 0 && choiceTick >= choiceTime)
					{
						--ActiveMenu;
						choiceTick = 0;
					}
				}
				if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					Difficulty_ = DifficultySelection;
					GameMode_ = GameModeSelection;
					Time_ = TimeSelection;
					KillLimit_ = KillLimitSelection;
					return true;
				}
				if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
				{
					return false;
				}
			}
			else
			{			
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					if (choiceTick >= choiceTime)
					{
						if (ActiveMenu == 0 && DifficultySelection - 1 >= 0)
						{
							--DifficultySelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 1 && GameModeSelection -1 >= 0)
						{
							--GameModeSelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 2 && TimeSelection -1 >= 0)
						{
							--TimeSelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 3 && KillLimitSelection -1 >= 0)
						{
							--KillLimitSelection;
							choiceTick = 0;
						}
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					if (choiceTick >= choiceTime) 
					{
						if (ActiveMenu == 0 && DifficultySelection + 1 < 3)
						{
							++DifficultySelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 1 && GameModeSelection +1 < 2)
						{
							++GameModeSelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 2 && TimeSelection +1 < 5)
						{
							++TimeSelection;
							choiceTick = 0;
						}
						else if (ActiveMenu == 3 && KillLimitSelection +1 < 4)
						{
							++KillLimitSelection;
							choiceTick = 0;
						}
					}
				}


				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					if (ActiveMenu -1 >= 0 && choiceTick >= choiceTime)
					{
						--ActiveMenu;
						choiceTick = 0;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					if (ActiveMenu + 1 < 4 && choiceTick >= choiceTime)
					{
						++ActiveMenu;
						choiceTick = 0;
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					Difficulty_ = DifficultySelection;
					GameMode_ = GameModeSelection;
					Time_ = TimeSelection;
					KillLimit_ = KillLimitSelection;
					return true;
				}
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
				{
					return false;
				}
			}
		}

		//	Window_->draw(opacityLayer);
		Window_->clear(sf::Color(20,20,20));

		Window_->draw(BackgroundSprite_);

		// activating the buttons
		if (ActiveMenu == 0)
		{
			LeftDifficulty_.IsActive_ = true;
			RightDifficulty_.IsActive_ = true;
		}
		else if (ActiveMenu == 1)
		{
			LeftGameMode_.IsActive_ = true;
			RightGameMode_.IsActive_ = true;
		}
		else if (ActiveMenu == 2)
		{
			LeftTime_.IsActive_ = true;
			RightTime_.IsActive_ = true;
		}
		else if (ActiveMenu == 3)
		{
			LeftKillLimit_.IsActive_ = true;
			RightKillLimit_.IsActive_ = true;
		}


		// Drawing arrows		
		LeftGameMode_.Draw(*Window_);
		RightGameMode_.Draw(*Window_);
		LeftDifficulty_.Draw(*Window_);
		RightDifficulty_.Draw(*Window_);
		LeftTime_.Draw(*Window_);
		RightTime_.Draw(*Window_);
		LeftKillLimit_.Draw(*Window_);
		RightKillLimit_.Draw(*Window_);

		// window
		DifficultyTexts_[DifficultySelection].setFont(*MenuFont_);
		Window_->draw(DifficultyTexts_[DifficultySelection]);

		GameModeTexts_[GameModeSelection].setFont(*MenuFont_);
		Window_->draw(GameModeTexts_[GameModeSelection]);

		TimeTexts_[TimeSelection].setFont(*MenuFont_);
		Window_->draw(TimeTexts_[TimeSelection]);

		KillLimitTexts_[KillLimitSelection].setFont(*MenuFont_);
		Window_->draw(KillLimitTexts_[KillLimitSelection]);

		// updating window
		Window_->display();
	}

	return false;
}

/**=============================
GameController::Update
=============================**/
void GameController::Update()
{
	if (!ClockStarted_)
	{
		StartTimePoint_ = GameClock_.now();
		ClockStarted_ = true;
	}

	// getting seconds elapsed til now
	NowTimePoint_ = GameClock_.now();
	int secondsElapsed = std::chrono::duration_cast<std::chrono::seconds> (NowTimePoint_ - StartTimePoint_).count();
	
	// formatting the time
	std::stringstream ss;
	ss << secondsElapsed / 60;
	std::string minutes = ss.str();
	ss.str(std::string());
	ss.clear();
	ss << secondsElapsed % 60;
	std::string seconds = ss.str();
	
	// adding a 0 in front only if the lenght is less than 1 
	minutes = minutes.length()==1?"0"+minutes:minutes;
	seconds = seconds.length()==1?"0"+seconds:seconds;

	// setting the string
	ClockText_.setString(minutes + " : " + seconds);

	// The game Controller is drawing the time counter aswell
	ClockText_.setFont(*TimeFont_);
	Window_->draw(ClockLayerSprite_);
	Window_->draw(ClockText_);


	// Checkig game rules
	if (Time_ != GAME_TIME_NO_LIMIT)
	{
		if (secondsElapsed >= TimeLimits_[Time_])
		{
			GameState_->GameOver();
		}
	}

	// checking kill limit
	if (KillLimit_ != GAME_KILL_NO_LIMIT)
	{
		std::size_t s = GameState_->EnemyFactory_.GetEntities()->size();
		std::vector<Entity*> winners;
		for (int i = 0; i < s;++i)
		{
			Entity* e  = (*GameState_->EnemyFactory_.GetEntities())[i];
			if (e->Kills_ >= KillLimits_[KillLimit_])
			{
				winners.push_back(e);
			}
		}
		if (!winners.empty())
		{
			GameState_->GameOver();
		}
	}
}

/**=============================
GameController::ShowScores
=============================**/
void GameController::ShowScores()
{
	std::vector<Entity*> rankedEntities;
	std::size_t s = GameState_->EnemyFactory_.GetEntities()->size();
	rankedEntities.push_back(&GameState_->Player_);
	for(int i = 0; i < s;++i)
	{
		rankedEntities.push_back((*GameState_->EnemyFactory_.GetEntities())[i]);
	}

	// sorting list
	std::sort(rankedEntities.begin(),rankedEntities.end(),KillSort);
	std::reverse(rankedEntities.begin(),rankedEntities.end());


	std::vector<sf::Text> highscoreTexts;

	// loading font
	sf::Font* font;
	font = Globals::Assets->GetFont("capture");

	sf::Text t;

	s = rankedEntities.size();

	for (int i = 0; i < s;++i)
	{
		Entity* entity = rankedEntities[i];

		t.setString(entity->Name_ + " : " + std::to_string( entity->Kills_));
		t.setFont(*font);
		t.setPosition(WINDOW_WIDTH / 2 - t.getLocalBounds().width / 2,200 + 35 * i);
		highscoreTexts.push_back(t);
	}

	// creating clock and after 10 seconds redirecting to the loading menu
	std::chrono::time_point<std::chrono::high_resolution_clock> start,end;

	start = std::chrono::high_resolution_clock::now();

	while (Window_->isOpen())
	{
		sf::Event event;
		while (Window_->pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed :
				Window_->close();
				exit(1);
			}
		}
		Window_->clear(sf::Color(0,0,0,127));
		Window_->draw(HighscoreSprite_);

		end = std::chrono::high_resolution_clock::now();
		int elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();

		if (elapsed_seconds >= 10)
		{
			UpdateHighscores(); // updating highscores
			return;
		}
		std::size_t s = highscoreTexts.size();
		for (int i = 0; i < s;++i)
		{
			Window_->draw(highscoreTexts[i]);
		}

		Window_->display();
	}
}

/**=============================
GameController::Reset
=============================**/
void GameController::Reset()
{
	ClockStarted_ = false;
}


/**=============================
GameController::UpdateHighscores
=============================**/
void GameController::UpdateHighscores()
{
	// creating container to hold data
	std::vector<PlayerRecord> records;

	PlayerRecord record;
	std::string line;

	/** PUSHING PLAYER AND ENEMIES THERE **/
	std::size_t s = GameState_->EnemyFactory_.GetEntities()->size();
	for (int i = 0; i < s;++i)
	{
		record.score = (*GameState_->EnemyFactory_.GetEntities())[i]->Kills_;
		record.name = (*GameState_->EnemyFactory_.GetEntities())[i]->Name_;

		records.push_back(record);
	}

	record.name = GameState_->Player_.Name_;
	record.score = GameState_->Player_.Kills_;

	records.push_back(record);

	/** READING FILE_STREAM **/
	std::ifstream file_input_stream;
	file_input_stream.open("data//new-assets//cfg//highscores.data");

	while (std::getline(file_input_stream,line))
	{
		int divider_index = line.find_first_of("#");
		record.name = line.substr(0,divider_index);
		record.score = std::stoi(line.substr(divider_index+1));

		records.push_back(record);
	}

	// sorting them
	std::sort(records.begin(),records.end(),compare);

	file_input_stream.close();
	
	// rewriting them
	std::ofstream file_output_stream;
	file_output_stream.open("data//new-assets//cfg//highscores.data");

	for (int i = 0; i < 10;++i) // writing only first 10 records
	{
		file_output_stream << records[i].name + "#" + std::to_string(records[i].score) + "\n";
	}

	file_output_stream.close();
}