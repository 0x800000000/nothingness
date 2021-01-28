#include "logger.h"
#include <iostream>
#include <fstream>

void nothingness::Logger::add_log_to_log_file(std::string log){
	std::ofstream writer(log_file_name, std::ios::binary | std::ios::app);
	writer << log;
	writer.close();
}

nothingness::Logger::Logger(const char* log_file_name): log_file_name(log_file_name){
	std::ofstream fileCreator(log_file_name);
	fileCreator << "";
	fileCreator.close();
}

const char* log_type_To_char(nothingness::Logger::log_type lt) {
	switch (lt){
	case nothingness::Logger::LT_error:
		return "[nothingness][error]";
		break;
	case nothingness::Logger::LT_warning:
		return "[nothingness][warning]";
		break;
	case nothingness::Logger::LT_info:
		return "[nothingness][warning]";
		break;
	case nothingness::Logger::LT_success:
		return "[nothingness][warning]";
		break;
	}

	return "";
}

void nothingness::Logger::add_log(log_type lt){
	std::string final_log = log_type_To_char(lt);
	final_log += buffer.str();

	std::cout << final_log << std::endl;

	add_log_to_log_file(final_log);
}

void nothingness::Logger::clear(){
	buffer.str("");
}

nothingness::Logger::~Logger()
{
}

