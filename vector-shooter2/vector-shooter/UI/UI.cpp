/**
==========================================================================
								UI source
==========================================================================
**/

#include "UI.h"
#include "../Weapon/WeaponSystem.h"
#include "../Weapon/Weapon.h"
#include "../Game/Game.h"
#include "../Entity/Player.h"


/**=============================
UI::UI
=============================**/
UI::UI()
{
	ActiveWeapon_ = 0;
	SpriteWidth_ = 16;
	SpriteHeight_ = 16;
	AmmoFontSize_ = 12;
	Loaded_ = false;
}

/**=============================
UI::Init
=============================**/
void UI::Init(sf::RenderWindow& window,Player& player,WeaponSystem& weaponSystem,Game& game)
{
	// loading variables

	ActiveWeapon_ = 0;
	SpriteWidth_ = 16;
	SpriteHeight_ = 16;
	AmmoFontSize_ = 14;
	Loaded_ = false;

	NumberOfElements_ = 8;

	Window_ = &window;
	Player_ = &player;
	WeaponSystem_ = &weaponSystem;
	Game_ =	&game;

	WeaponsFilename_ = "data//assets//icons//military//output.png";

	// Initializing the UI
	WeaponSpriteSheet_.loadFromFile(WeaponsFilename_);
	int sheetWidth = WeaponSpriteSheet_.getSize().x;
	int sheetHeight = WeaponSpriteSheet_.getSize().y;

	for (int w = 0; w < sheetHeight; w += SpriteWidth_)
	{
		for (int h = 0; h < sheetWidth; h += SpriteHeight_)
		{
			TextureRects_.push_back(sf::IntRect(sf::Vector2i(h,w),sf::Vector2i(SpriteWidth_,SpriteHeight_)));
			
		}
	}

	// Loading font
	AmmoFont_.loadFromFile("data//fonts//Capture.ttf");

	// Creating elements
	float posX = 10;
	for (int i = 0; i < NumberOfElements_;++i)
	{
		UIElement el(WeaponSpriteSheet_,TextureRects_[i],sf::Vector2f(posX,10));
		AmmoIcons_.push_back(el);

		// Creating ammo text
		sf::Text t;
		t.setColor(sf::Color(0,255,0,127));
		t.setPosition(posX, 10 + 20);
		t.setCharacterSize(AmmoFontSize_);
		AmmoTexts_.push_back(t);
		
		// Creating loader text
		sf::Text lt;
		lt.setColor(sf::Color(255,125,0));
		lt.setPosition(posX, 10 + 20 + 20);
		lt.setCharacterSize(AmmoFontSize_);
		LoaderTexts_.push_back(lt);

		// increasing position
		posX += 40;
	}
	

	// Creating select box
	SelectBox_.setPointCount(4);
	SelectBox_.setPoint(0,sf::Vector2f(0,0));
	SelectBox_.setPoint(1,sf::Vector2f(20,0));
	SelectBox_.setPoint(2,sf::Vector2f(20,20));
	SelectBox_.setPoint(3,sf::Vector2f(0,20));
	SelectBox_.setFillColor(sf::Color(0,0,255,127));

}

/**=============================
UI::Update
=============================**/
void UI::Update(const sf::Time& deltaTime)
{
	// getting the active weapon
	ActiveWeapon_ = WeaponSystem_->GetActiveWeapon();

	// Drawing the selector just a square box behind the active weapon
	sf::Vector2f selectBoxPosition(10 + ActiveWeapon_ * 40 - 2,8);
	SelectBox_.setPosition(selectBoxPosition);
	Window_->draw(SelectBox_);

	// Drawing ammo icons
	std::size_t s = AmmoIcons_.size();

	if (!Loaded_)
	{
		std::size_t s = AmmoTexts_.size();
		for (std::size_t i = 0; i < s; ++i)
		{
			AmmoTexts_[i].setFont(AmmoFont_);
			LoaderTexts_[i].setFont(AmmoFont_);
		}
		Loaded_ = false;
	}

	for (std::size_t i = 0; i < s; ++i)
	{
		AmmoIcons_[i].Draw(*Window_);
	}
	 
	// Drawing ammo info & loader info
	s = AmmoTexts_.size();
	for (std::size_t i = 0; i < s; ++i)
	{
		// Updating info
		AmmoTexts_[i].setString(std::to_string(((*WeaponSystem_->GetWeapons())[i].GetAmmoLeft())));
		Window_->draw(AmmoTexts_[i]);

		LoaderTexts_[i].setString(std::to_string(((*WeaponSystem_->GetWeapons())[i].GetLoadersLeft())));
		Window_->draw(LoaderTexts_[i]);

	}
}