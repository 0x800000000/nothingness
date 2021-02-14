#include "resource_manager.h"
#include "..//luaScriptEngine/luaScriptEngine.h"
#include "json.hpp"
#include <fstream>

nothingness::resouce_manager::resouce_manager(){

}

void nothingness::resouce_manager::load_resources(const char* json_path){
	nlohmann::json JSONdocument;
	
	//getting json document:
	std::ifstream reader(json_path);
	if (!reader.is_open()) {
		Throw(new nothingness::nothingness_file_load_exception("cannot open file:", json_path));
		return;
	}

	reader >> JSONdocument;
	reader.close();

	std::string resources_directory;

	if (!JSONdocument["nothingness"]["resource_directory"].is_null()) {
		resources_directory = JSONdocument["nothingness"]["resource_directory"];
	}
	else {
		resources_directory = "";
	}


	nlohmann::json resources = JSONdocument["nothingness"]["resources"];

	for (int i = 0; i < resources.size(); i++) {
		nlohmann::json resource = resources[i];

		int type = resources[i]["type"];

		
		if (type > 1 && type < 4) {
			std::string final_file_name = resources_directory;
			final_file_name += resources[i]["file_name"];
			
			if (!load_certificate_or_private_key_file(resources[i]["name"].get<std::string>().c_str(), final_file_name.c_str())) {
				Throw(new nothingness::nothingness_file_load_exception("cannot load certificate file:", final_file_name));
				return;
			}
		}

		switch (static_cast<nothingness::resouce_manager::resource_type>(type)){
		case nothingness::resouce_manager::resource_type::rt_lua_script:
			
			break;
		}

	}


}

bool nothingness::resouce_manager::load_certificate_or_private_key_file(const char* name, const char* path){
	resources[name] = (void*)path;
	return true;
}

bool nothingness::resouce_manager::load_lua_script(const char* name, const char* path){
	nothingness::LuaScript* script = new nothingness::LuaScript(path);
	resources[name] = (void*)script;

	return true;
}

void* nothingness::resouce_manager::getResource(const char* name){
	return resources[name];
}
