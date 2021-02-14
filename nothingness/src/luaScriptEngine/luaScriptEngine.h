#ifndef _NOTHINGNESS_LUA_SCRIPT_ENGINE_H
#define _NOTHINGNESS_LUA_SCRIPT_ENGINE_H
#include"..//..//platform.h"

struct lua_State;

#include<functional>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include "..//error_handler/error_handler.h"

namespace nothingness {
	class LuaScript : nothingness::error_handler {
		
		lua_State* lua;

		std::function<void(lua_State&)> init;

		bool _execute_script(const char* path, lua_State& lua);

		std::string path;

	public:

		void operator=(LuaScript&) = delete;
		void operator==(LuaScript&) = delete;
		void operator!=(LuaScript&) = delete;
		void operator>=(LuaScript&) = delete;
		void operator>>(LuaScript&) = delete;
		void operator<<(LuaScript&) = delete;

		LuaScript();
		LuaScript(const char* path);

		bool execute_script(const char* path);
		bool execute_script(const char* path, std::function<void(lua_State&)> init);

		luabridge::LuaRef get_function(const char* name);
	};
}

#endif 