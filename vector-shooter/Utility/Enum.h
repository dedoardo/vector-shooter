/**
==========================================================================
								Enumarator File
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
	CREDITS_STATE
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

// game difficulty
enum 
{
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD
};

// game mode
enum
{
	GAME_MODE_DAY,
	GAME_MODE_NIGHT
};

// game time option 
enum
{
	GAME_TIME_NO_LIMIT,
	GAME_0_30,
	GAME_2_00,
	GAME_3_00,
	GAME_5_00
};

// kill options
enum
{
	GAME_KILL_NO_LIMIT,
	GAME_KILL_10,
	GAME_KILL_15,
	GAME_KILL_30
};

// goal status
enum 
{
	GOAL_STATUS_FAILED,
	GOAL_STATUS_COMPLETED,
	GOAL_STATUS_ACTIVE
};

// strategic goals
enum
{
	STRATEGIC_GOAL_EXPLORE,
	STRATEGIC_GOAL_ATTACK,
	STRATEGIC_GOAL_GET_HEALTH,
	STRATEGIC_GOAL_GET_WEAPON,
	STRATEGIC_GOAL_SEEK_TARGET
};

// item types
enum
{
	NODE_TYPE_NONE,
	NODE_TYPE_WEAPON_0,
	NODE_TYPE_WEAPON_1,
	NODE_TYPE_HEALTH,
	NODE_TYPE_POWER,
	NODE_TYPE_BOOST
};

// node types
enum
{
	NODE_NO_COST_ASSIGNED,
	NODE_VISITED,
	NODE_UNVISITED
};

// bonuses
enum
{
	BONUS_WEAPON,
	BONUS_HEALTH,
	BONUS_BOOST
};

// effect textures
enum
{
	EFFECT_TEXTURE_EXPLOSION_RED_SMALL,
	EFFECT_TEXTURE_EXPLOSION_RED_BIG,
	EFFECT_TEXTURE_EXPLOSION_BLUE_SMALL,
	EFFECT_TEXTURE_EXPLOSION_BLUE_BIG,
	EFFECT_TEXTURE_EXPLOSION_ROCKET
};

static int NODE_DOESNT_EXIST = -1;

#endif