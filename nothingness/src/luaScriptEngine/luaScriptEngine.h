#ifndef _NOTHINGNESS_LUA_SCRIPT_ENGINE_H
#define _NOTHINGNESS_LUA_SCRIPT_ENGINE_H

#include"..//..//platform.h"

struct lua_State;

#include<functional>
namespace nothingness {
	class luaScriptEngine {
		
		lua_State* lua;

		std::function<void(lua_State&)> init;

		bool _execute_script(const char* path, lua_State& lua);

	public:

		luaScriptEngine();

		bool execute_script(const char* path);
		bool execute_script(const char* path, std::function<void(lua_State&)> init);
	};
}

#endif 