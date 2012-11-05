/**
==========================================================================
						Player file
==========================================================================
**/

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Player.h"
#include "../Utility/Enum.h"
#include "../Game/Game.h"
#include "../Game/States/GameRunState.h"
#include "../Controller/ControllerXBox.h"
#include "../rapidxml/rapidxml.hpp"
#include "../Weapon/WeaponSystem.h"

#include <iostream>
#include <fstream>
#include <list>

using namespace rapidxml;

/**=============================
Player::Init
=============================**/
void Player::Init(sf::RenderWindow& window,Game& game,GameRunState& gameState)
{
	Type_ = ENTITY_TYPE_PLAYER;

	// Hardcoding the stats here since i don't wanna create a cfg file ( too easy to mod )
	Position_ = sf::Vector2f(110,110);
	PreviousPosition_ = Position_;
	Heading_  = sf::Vector2f(0,0);
	Angle_ = 0;
	PreviousAngle_ = Angle_;
	Speed_ = 0;
	LateralSpeed_ = 0;
	MaxSpeed_ = 50;
	Acceleration_ = 1;

	Game_ = &game;
	Window_ = &window;
	GameState_ = &gameState;

	// Loading the vertices based on the texture 
	// rapidxml
	std::ifstream vertexStream("data/cfg//PlayerVertices.xml");
	if (vertexStream.good())
	{
		std::vector<char> buffer((std::istreambuf_iterator<char>(vertexStream)),std::istreambuf_iterator<char>());	
		buffer.push_back('\0');
		xml_document<> vertexDocument;
		vertexDocument.parse<0>(&buffer[0]);

		xml_node<>* textureNode = vertexDocument.first_node()->first_node();
		xml_node<>* verticesNode = textureNode->first_node("v");

		for (xml_node<>* nextNode = verticesNode->first_node("vertex"); nextNode;
			nextNode = nextNode->next_sibling("vertex"))
		{
			sf::Vector2f vertex;
			vertex.x = atof(nextNode->first_attribute("x")->value());
			vertex.y = atof(nextNode->first_attribute("y")->value());
			PlayerVertices_.push_back(vertex);
		}
	
		// getting the fire point
		xml_node<>* fireNode = textureNode->first_node("fire");
		FireVertex_.x = std::stof(fireNode->first_node("vertex")->first_attribute("x")->value());
		FireVertex_.y = std::stof(fireNode->first_node("vertex")->first_attribute("y")->value());
	}

	vertexStream.close();

	ActiveTexture_.loadFromFile("data//assets//Player.png");
	Sprite_.setTexture(ActiveTexture_);
	Sprite_.setOrigin(ActiveTexture_.getSize().x / 2, ActiveTexture_.getSize().y /2);
	
	PlayerShape_.setPointCount(1);
	PlayerShape_.setPoint(0,PlayerVertices_[0]);
	PlayerShape_.setOrigin(ActiveTexture_.getSize().x / 2, ActiveTexture_.getSize().y /2);

	UpdatedVertices_.resize(PlayerVertices_.size());
	
	Width_ = ActiveTexture_.getSize().x;
	Height_ = ActiveTexture_.getSize().y;

	Colliding_ = false;

	// Weapon info
	ActiveWeapon_ = 0;
	ChangeWeaponTick_ = 0;
	ChangeWeaponTime_ = 0.1;
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

		// Checking for the "thumb stick" dead zone
		if (leftThumbMagnitude > 5000)
		{
			Speed_ += Acceleration_ * deltaTime.asMilliseconds();
		}
		else
		{
			if (Speed_ > 0) Speed_ -= Acceleration_ * 2;
			else if (Speed_ < 0) Speed_ += Acceleration_ *2;
		}

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

		// checking keyboard
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			Speed_ += (Acceleration_ * deltaTime.asMilliseconds());
		}
	
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			Speed_ -= (Acceleration_ * deltaTime.asMilliseconds());
		}
		else 
		{
			if (Speed_ < 0) Speed_ += Acceleration_ * 3;
			else if (Speed_ > 0) Speed_ -= Acceleration_ * 3;

		};
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			LateralSpeed_ = - MaxSpeed_;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			LateralSpeed_ = MaxSpeed_;
		}
		else 
		{
			if (LateralSpeed_ < 0) LateralSpeed_ = 0;
			else if (LateralSpeed_ > 0) LateralSpeed_ = 0; 
		}

		PreviousAngle_ = (atan2(Heading_.x,Heading_.y) * 180 / 3.1415926) - 90;
	
		// modifying position
		PreviousPosition_.x += Speed_ * Heading_.x * deltaTime.asSeconds();
		PreviousPosition_.y += Speed_ * -Heading_.y * deltaTime.asSeconds();

	}

	// checking if speed is too much
	Speed_ = Speed_>=MaxSpeed_?MaxSpeed_:Speed_;
	Speed_ = Speed_<=-MaxSpeed_?-MaxSpeed_:Speed_;
}


/**=============================
Player::Update
=============================**/
void Player::Update(const sf::Time& deltaTime)
{
	// controlling the input
	HandleInput(deltaTime);

	// setting active weapon
	WeaponSystem_->SetActiveWeapon(ActiveWeapon_);

	// getting lateral heading
	sf::Vector2f lateralHeading(Heading_.y,Heading_.x);
	PreviousPosition_.x += LateralSpeed_ * lateralHeading.x * deltaTime.asSeconds();
	PreviousPosition_.y += LateralSpeed_ * lateralHeading.y * deltaTime.asSeconds();

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

	for (int i = 0; i < UpdatedVertices_.size();++i)
	{
		sf::CircleShape s(1);
		s.setFillColor(sf::Color(255,0,0));
		s.setPosition(UpdatedVertices_[i] + Position_);
		Window_->draw(s);
	}

}

/**=============================
Player::GetUpdatedVertices
=============================**/
std::vector<sf::Vector2f>* Player::GetUpdatedVertices()
{
	PlayerShape_.setPosition(PreviousPosition_);
	PlayerShape_.setRotation(PreviousAngle_);

	std::size_t s = PlayerVertices_.size();
	for (std::size_t i = 0; i < s; ++i)
	{
		float offsetX = PlayerShape_.getOrigin().x - PlayerVertices_[i].x;
		float offsetY = PlayerShape_.getOrigin().y - PlayerVertices_[i].y;

		float rotX = PlayerVertices_[i].x - offsetX;
		float rotY = PlayerVertices_[i].y - offsetY;

		float rotAngle = PlayerShape_.getRotation() * 3.1415 / 180;

		UpdatedVertices_[i].x = (rotX * cos(rotAngle) - rotY * sin(rotAngle));
		UpdatedVertices_[i].y = (rotY * cos(rotAngle) + rotX * sin(rotAngle));

	}
	return &UpdatedVertices_;
}