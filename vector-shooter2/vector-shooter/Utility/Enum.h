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

#endif