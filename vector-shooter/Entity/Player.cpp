/**
==========================================================================
						Player file
==========================================================================
**/

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>

#include "Player.h"
#include "../Utility/Enum.h"
#include "../Game/Game.h"
#include "../Game/States/GameRunState.h"
#include "../Controller/ControllerXBox.h"
#include "../rapidxml/rapidxml.hpp"
#include "../Weapon/WeaponSystem.h"
#include "../Globals.h"

#include <iostream>
#include <fstream>
#include <list>

using namespace rapidxml;

/**=============================
Player::Init
=============================**/
void Player::Init(sf::RenderWindow& window,Game& game,GameRunState& gameState)
{
	Game_ = &game;
	Window_ = &window;
	GameState_ = &gameState;

	// Hardcoding the stats here since i don't wanna create a cfg file ( too easy to mod )
	// generating random position
	int randomIndex = rand() % GameState_->MapHandler_.GetActiveMap()->GetNodes()->size();
		if (randomIndex > 0) 
			randomIndex -= 1;
	Position_ = (*GameState_->MapHandler_.GetActiveMap()->GetNodes())[randomIndex].GetPosition();
	PreviousPosition_ = Position_;
	Heading_  = sf::Vector2f(0,0);
	Angle_ = 0;
	PreviousAngle_ = Angle_;
	Speed_ = 150;
	LateralSpeed_ = 0;
	HitPoints_ = 100;

	ActiveTexture_ = Globals::Assets->GetTexture("player");
	Sprite_.setTexture(*ActiveTexture_);
	Sprite_.setOrigin(ActiveTexture_->getSize().x / 2, ActiveTexture_->getSize().y /2);

	Origin_ = Sprite_.getOrigin();

	UpdatedVertices_.resize(PlayerVertices_.size());
	
	Width_ = ActiveTexture_->getSize().x;
	Height_ = ActiveTexture_->getSize().y;

	Colliding_ = false;

	// Weapon info
	ActiveWeapon_ = 0;
	ChangeWeaponTick_ = 0;
	ChangeWeaponTime_ = 0.1;
	BoundingCircleRadius_ = ActiveTexture_->getSize().x /2;

	Name_ = "Player";
	Kills_ = 0;
	IsDead_ = false;
	StartDeathClock_ = false;
}

/**=============================
Player::LoadWeaponSystem
=============================**/
void Player::LoadWeaponSystem(WeaponSystem& w)
{
	WeaponSystem_ = &w;
}


/**=============================
Player::HandleInput
=============================**/
void Player::HandleInput(const sf::Time& deltaTime)
{
	// if controller is controller i'm gonna handle it otherwise i'm gonna use keyboarda and mice
	
	if (Game_->ControllerXBox_->IsConnected())
	{
		// getting position of left thumbstick
		short leftPosX = Game_->ControllerXBox_->GetState().Gamepad.sThumbLX;
		short leftPosY = Game_->ControllerXBox_->GetState().Gamepad.sThumbLY;

		// getting position of right thumbstick
		short rightPosX = Game_->ControllerXBox_->GetState().Gamepad.sThumbRX;
		short rightPosY = Game_->ControllerXBox_->GetState().Gamepad.sThumbRY;

		// calculating heading 
		// checking the dead zone firsts
		float rightThumbMagnitude = sqrt(rightPosX * rightPosX + rightPosY * rightPosY);
		Heading_.x = -(static_cast<float>(rightPosX) / rightThumbMagnitude);
		Heading_.y = -(static_cast<float>(rightPosY) / rightThumbMagnitude);

		PreviousAngle_ = (atan2(Heading_.x,Heading_.y) * 180 / 3.1415926) + 90;

		// getting the movement heading
		sf::Vector2f movHeading;
		float leftThumbMagnitude = sqrt(leftPosX * leftPosX + leftPosY * leftPosY);
		// getting the normalized vector
		movHeading.x = static_cast<float>(leftPosX) / leftThumbMagnitude;
		movHeading.y = -static_cast<float>(leftPosY) / leftThumbMagnitude;

		PreviousPosition_.x += Speed_ * movHeading.x * deltaTime.asSeconds();
		PreviousPosition_.y += Speed_ * movHeading.y * deltaTime.asSeconds();

		ChangeWeaponTick_ += deltaTime.asSeconds();

		// Handling the weapon change
		if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			if (ActiveWeapon_ + 1 < 8 && ChangeWeaponTick_ >= ChangeWeaponTime_)
				ActiveWeapon_ += 1;
			ChangeWeaponTick_ = 0;
		}
		else if (Game_->ControllerXBox_->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			if (ActiveWeapon_ - 1 >= 0 && ChangeWeaponTick_ >= ChangeWeaponTime_)
				ActiveWeapon_ -= 1;
			ChangeWeaponTick_ = 0;
		}

		// handling the fire button
		if (Game_->ControllerXBox_->GetState().Gamepad.bRightTrigger)
		{
			WeaponSystem_->Fire();
		}
		else
		{
			Game_->ControllerXBox_->StopAllVibrations();
		}

	
	}
	else
	{
		// getting the mouse position based on the screen
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*Window_);

		// calculating heading
		float xDiff = static_cast<float>(mousePosition.x) - Position_.x;
		float yDiff = static_cast<float>(mousePosition.y) - Position_.y;
		float magnitude = sqrt(xDiff * xDiff + yDiff * yDiff);
		Heading_.x = xDiff / magnitude;
		Heading_.y = -yDiff / magnitude;

		LateralHeading_ = sf::Vector2f(0,0);

		short moved = 0;

		// checking keyboard
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			LateralSpeed_ = Speed_;
			LateralHeading_.x = -1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			LateralSpeed_ = Speed_;
			LateralHeading_.x = 1;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			LateralSpeed_ = Speed_;
			LateralHeading_.y = -1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			LateralSpeed_ = Speed_;
			LateralHeading_.y = 1;
		}

		PreviousAngle_ = (atan2(Heading_.x,Heading_.y) * 180 / 3.1415926) - 90;

		// firing
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			WeaponSystem_->Fire();
		}

		ChangeWeaponTick_ += deltaTime.asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			if (ActiveWeapon_ + 1 < 8 && ChangeWeaponTick_ >= ChangeWeaponTime_)
				ActiveWeapon_ += 1;
			ChangeWeaponTick_ = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			if (ActiveWeapon_ - 1 >= 0 && ChangeWeaponTick_ >= ChangeWeaponTime_)
				ActiveWeapon_ -= 1;
			ChangeWeaponTick_ = 0;
		}

	}
}


/**=============================
Player::Update
=============================**/
void Player::Update(const sf::Time& deltaTime)
{
	if (!IsDead_)
	{
		// checking if the player is dead
		if (HitPoints_ <= 0)
		{
			IsDead_ = true;
			return;
		}
		// controlling the input
		HandleInput(deltaTime);

		PreviousPosition_.x += LateralSpeed_ * LateralHeading_.x * deltaTime.asSeconds();
		PreviousPosition_.y += LateralSpeed_ * LateralHeading_.y * deltaTime.asSeconds();

		// setting active weapon
		WeaponSystem_->SetActiveWeapon(ActiveWeapon_);

		int result = GameState_->CollisionHandler_.PlayerToWall();

		// Checking it the new position is OK
		if (result != COLLISION_FALSE)
		{
			switch (result)
			{
			case COLLISION_LEFT :
				{
					PreviousPosition_.x = Position_.x;
					break;
				}
			case COLLISION_RIGHT :
				{
					PreviousPosition_.x = Position_.x;
					break;
				}
			case COLLISION_TOP :
				{
					PreviousPosition_.y = Position_.y;
					break;
				}
			case COLLISION_DOWN :
				{
					PreviousPosition_.y = Position_.y;
					break;
				}
			case COLLISION_TRUE :
				{
					PreviousPosition_ = Position_;
					break;
				}
			}
		}

		Position_ = PreviousPosition_;
		Angle_	  = PreviousAngle_;

		Sprite_.setPosition(Position_);
		Sprite_.setRotation(Angle_);

		Window_->draw(Sprite_);
	}
	else
	{
		if (!StartDeathClock_)
		{
			DeadTimePoint_ = std::chrono::high_resolution_clock::now();
			StartDeathClock_ = true;
		}

		// getting seconds elapse
		int seconds_elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - DeadTimePoint_).count();
		if (seconds_elapsed >= 5)
		{
			StartDeathClock_ = false;
			IsDead_ = false;
			// generating random position
			int randomIndex = rand() % GameState_->MapHandler_.GetActiveMap()->GetNodes()->size();
				if (randomIndex > 0) 
					randomIndex -= 1;
			Position_ = (*GameState_->MapHandler_.GetActiveMap()->GetNodes())[randomIndex].GetPosition();
			PreviousPosition_ = Position_;
			PreviousPosition_ = Position_;
			HitPoints_ = 100;
		}
	}
}

/**=============================
Player::IncreaseAmmo
=============================**/
void Player::IncreaseAmmo()
{
	WeaponSystem_->IncreaseAmmo();
} 