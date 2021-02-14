#ifndef _INC_NOTHINGNESS_ERROR_HANDLER_H
#define _INC_NOTHINGNESS_ERROR_HANDLER_H

#include <string>

namespace nothingness {

	class nothingness_exception {
		std::string msg;

	public:
		nothingness_exception(const std::string& msg);

		const char* what();

		~nothingness_exception();
	};

	class nothingness_file_load_exception : public nothingness::nothingness_exception {
		std::string file_name;

	public:
		nothingness_file_load_exception(const std::string& msg, const std::string& file_name);

		const char* get_file_name();

		~nothingness_file_load_exception();
	};

	class error_handler {

		nothingness_exception* exception;

	protected:

		error_handler();

		void Throw(nothingness_exception* exception);
	public:

		error_handler(int function_result);

		nothingness_exception* Catch();

	};
}

#endif