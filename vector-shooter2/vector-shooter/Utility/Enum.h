/**
==========================================================================
								Enumaratos File
==========================================================================
**/


#ifndef ENUM_H_INCLUDED
#define ENUM_H_INCLUDED

static unsigned int STATES_NUMBER = 5;

// states enumerator
enum
{
	MAIN_MENU_STATE,
	GAME_RUN_STATE,
	GAME_MENU_STATE,
	HIGHSCORE_STATE,
	OPTIONS_STATE
};

// textures enumerator

static unsigned int TEXT_NUMBER = 4;

enum
{
	PLAYER_TEXT,
	ENEMY_TEXT,
	TILE_TEXT_0,
	TILE_WALL_0
};

// entity type enumerator

enum 
{
	ENTITY_TYPE_PLAYER,
	ENTITY_TYPE_ENEMY,
	ENTITY_TYPE_BULLET,
	ENTITY_TYPE_WALL
};

// different kinds of player-to-wall collision
enum
{
	COLLISION_LEFT,
	COLLISION_RIGHT,
	COLLISION_TOP,
	COLLISION_DOWN,
	COLLISION_FALSE,
	COLLISION_TRUE
};


// weapons systems

enum
{
	WEAPON_SYSTEM_PLAYER_DEFAULT,
	WEAPON_SYSTEM_ENEMY_DEFAULT
};

// different weapons

enum
{
	WEAPON_1,
	WEAPON_2,
	WEAPON_3,
	WEAPON_4,
	WEAPON_5,
	WEAPON_6,
	WEAPON_7,
	WEAPON_8
};

#endif