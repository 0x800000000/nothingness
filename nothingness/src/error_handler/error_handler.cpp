#include "error_handler.h"
#include "..//network/network.h"

nothingness::error_handler::error_handler(){

}

void nothingness::error_handler::Throw(nothingness_exception* exception){
	if (this->exception)
		delete exception;

	this->exception = exception;
}

nothingness::error_handler::error_handler(int function_result){
	if (function_result > 0) {
		switch (function_result){
#ifdef WIN32
		case NOTHINGNESS_WSA_STARTUP_FAILED:
			Throw(new nothingness::nothingness_exception("WSAStartup failed!"));
			break;
#endif
		}
	}
}

nothingness::nothingness_exception* nothingness::error_handler::Catch(){
	return exception;
}

nothingness::nothingness_exception::nothingness_exception(const std::string& msg): msg(msg){}

const char* nothingness::nothingness_exception::what(){
	return msg.c_str();
}

nothingness::nothingness_exception::~nothingness_exception(){
	msg.clear();
}


nothingness::nothingness_file_load_exception::nothingness_file_load_exception(const std::string& msg, const std::string& file_name)
: nothingness::nothingness_exception(msg), file_name(file_name) {}

const char* nothingness::nothingness_file_load_exception::get_file_name(){
	return file_name.c_str();
}

nothingness::nothingness_file_load_exception::~nothingness_file_load_exception(){
	file_name.clear();
}
