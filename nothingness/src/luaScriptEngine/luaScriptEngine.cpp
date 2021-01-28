#include "luaScriptEngine.h"
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include "..//nothingness/nothingness.h"
#include "..//logger/logger.h"

bool nothingness::luaScriptEngine::_execute_script(const char* path, lua_State& lua){

	if (luaL_loadfile(&lua, path) || lua_pcall(&lua, 0, 0, 0)) {
		nothingness::Logger* logger = nothingness::Nothingness::getLastInstance()->getLogger();
		
		*logger << "error cannot execute script: \"" << path << "\" !\n";
		logger->add_log(nothingness::Logger::log_type::LT_error);

		return false;
	}

	return false;
}

nothingness::luaScriptEngine::luaScriptEngine(){
	lua = luaL_newstate();
}

bool nothingness::luaScriptEngine::execute_script(const char* path){
	luaL_openlibs(lua);
	return this->_execute_script(path, *(lua));
}

bool nothingness::luaScriptEngine::execute_script(const char* path, std::function<void(lua_State&)> init){
	luaL_openlibs(lua);

	init(*(lua));

	return this->_execute_script(path, *(lua));;
}
