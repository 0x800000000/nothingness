#include "nothingness.h"
#include "..//logger/logger.h"
#include <string.h>

nothingness::Nothingness *nothingness::Nothingness::instance;

nothingness::Nothingness::Nothingness(int argc, char* argv[], const char* log_file_name){
	std::string file_name;

	if (argc > 1) {
		for (int i = 0; i < argc; i++){
			if (strcmp("--nothingness-config", argv[i]) == 0) {
				i++;
				if (strcmp("--log-file-name", argv[i]) == 0) {
					i++;
					file_name = argv[i];
					i++;
				}
				break;
			}
		}
	}
	else {
		file_name = log_file_name;
	}

	this->logger = new nothingness::Logger(file_name.c_str());
	nothingness::Nothingness::instance = this;
}

nothingness::Logger* nothingness::Nothingness::getLogger() const{
	return this->logger;
}

nothingness::Nothingness* nothingness::Nothingness::getLastInstance(){
	return nothingness::Nothingness::instance;
}
