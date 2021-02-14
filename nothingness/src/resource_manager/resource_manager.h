#ifndef _NOTHINGNESS_RESOUCE_MANAGER_H
#define _NOTHINGNESS_RESOUCE_MANAGER_H

#include "..//error_handler/error_handler.h"
#include <unordered_map>

namespace nothingness {
	class Nothingness;

	class resouce_manager : public nothingness::error_handler {

		std::unordered_map<std::string, void*> resources;

	public:
		enum resource_type {
			rt_lua_script = 1,
			rt_cert_file = 2,
			rt_key_file = 3
		};

		resouce_manager();

		void load_resources(const char* json_path);
		
		bool load_certificate_or_private_key_file(const char* name, const char* path);
		
		bool load_lua_script(const char* name, const char* path);

		void* getResource(const char* name);

	};

}

#endif