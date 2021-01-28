#ifndef _INC_NOTHINGNESS_LOGGER_H
#define _INC_NOTHINGNESS_LOGGER_H

#include<sstream>
#include<string>

namespace nothingness {
	class Logger {
		std::stringstream buffer;
		std::string log_file_name;

		void add_log_to_log_file(std::string log);
	public:

		enum log_type {
			LT_error = 0,
			LT_warning = 1,
			LT_info = 2,
			LT_success = 3
		};

		Logger(const char* log_file_name);

		template<typename T>
		Logger& operator<<(T& data);

		void add_log(log_type lt);
		void clear();

		~Logger();
	};
	template<typename T>
	inline Logger& Logger::operator<<(T& data) {
		buffer << data;
		return &this;
	}
}

#endif 