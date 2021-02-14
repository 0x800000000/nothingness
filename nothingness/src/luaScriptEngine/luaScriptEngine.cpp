#include "luaScriptEngine.h"
#include "..//logger/logger.h"

bool nothingness::LuaScript::_execute_script(const char* path, lua_State& lua){

	if (luaL_loadfile(&lua, path) || lua_pcall(&lua, 0, 0, 0)) {
		
		std::string error_string = "cannot execute script: ";
		error_string += path;
		Throw(new nothingness::nothingness_file_load_exception(error_string.c_str(), path));
		error_string.clear();

		return false;
	}

	return true;
}

nothingness::LuaScript::LuaScript(){
	lua = luaL_newstate();
}

nothingness::LuaScript::LuaScript(const char* path) : path(path){}

bool nothingness::LuaScript::execute_script(const char* path){
	luaL_openlibs(lua);
	return this->_execute_script(path, *(lua));
}

bool nothingness::LuaScript::execute_script(const char* path, std::function<void(lua_State&)> init){
	luaL_openlibs(lua);

	init(*(lua));

	return this->_execute_script(path, *(lua));;
}

luabridge::LuaRef nothingness::LuaScript::get_function(const char* name){
	return luabridge::getGlobal(lua, name);
}
