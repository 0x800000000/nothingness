#ifndef NOTHINGNESS_H
#define NOTHINGNESS_H

namespace nothingness {
	class Logger;

	class Nothingness {
		Logger* logger;

		static Nothingness* instance;

	public:

		Nothingness(int argc, char* argv[], const char* log_file_name);

		Logger* getLogger()const ;

		static Nothingness* getLastInstance() ;

	};
}

#endif 