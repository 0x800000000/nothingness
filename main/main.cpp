#include"main.h"
#include<lua.hpp>

#include<LuaBridge/LuaBridge.h>

void printMessage(const std::string& s) {
	std::cout << s << std::endl;
}

int main(int argc, char* argv[]) {
	nothingness::Nothingness* engine = new nothingness::Nothingness(argc, argv, "log.txt");
	nothingness::luaScriptEngine* ls_engine = new nothingness::luaScriptEngine();

	ls_engine->execute_script("resources/scripts/t.lua", [engine](lua_State& lua) {
		luabridge::getGlobalNamespace(&lua)
			.addFunction("printMessage", printMessage);
	});

	delete ls_engine;
	delete engine;
}