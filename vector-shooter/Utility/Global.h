



#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace lua
{
	static lua_State* GlobalLuaState;
}
#endif // global included