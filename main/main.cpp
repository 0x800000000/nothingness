#include"main.h"
#include<lua.hpp>

#include<LuaBridge/LuaBridge.h>
#include<wolfssl/ssl.h>

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

	nothingness::network::initalize_network();
	nothingness::network::initalize_network_ssl();

	nothingness::network::SSL::Context* context = nothingness::network::create_SSL_server_context();

	if (!context->use_certificate_file("resources/certs/server-cert.pem", SSL_FILETYPE_PEM)) {
		return -1;
	}

	if (!context->use_PrivateKey_file("resources/certs/server-key.pem", SSL_FILETYPE_PEM)) {
		
		return -1;
	}

	nothingness::network::Socket* connection = nothingness::network::create_ssl_socket(context);
	if (nothingness::network::last_error_code > 0) {
		nothingness::Logger* logger = engine->getLogger();
		*logger << "cannot create socket:\n" << nothingness::network::last_error << "\n";
		logger->add_log(nothingness::Logger::log_type::LT_error);
		return -1;
	}
	
	nothingness::network::Socket* working_client;

	char request[1024];

	connection->_listen("127.0.0.1", 1435, NOTHINGNESS_SOCKET_INPUT_DATA_USE_IP_PORT);

	std::stringstream response; 
	std::stringstream response_body; 

	while (true) {
		working_client = connection->_accept();
		if (nothingness::network::last_error_code > 0 && nothingness::network::last_error_code != -313 && nothingness::network::last_error_code != -308) {

			nothingness::Logger* logger = engine->getLogger();
			*logger << "accept error:\n" << nothingness::network::last_error << "\n";
			logger->add_log(nothingness::Logger::log_type::LT_error);
			//return -1;
		}
		if (working_client != nullptr) {
			working_client->read(request, 1024);

			std::cout << request << std::endl;

			response_body << "<title>Test C++ HTTP Server</title>\n"
				<< "<h1>Test page</h1>\n"
				<< "<p>This is body of the test page...</p>\n"
				<< "<h2>Request headers</h2>\n"
				<< "<pre>" << request << "</pre>\n"
				<< "<em><small>Test C++ Http Server</small></em>\n";

			response << "HTTP/1.1 200 OK\r\n"
				<< "Version: HTTP/1.1\r\n"
				<< "Content-Type: text/html; charset=utf-8\r\n"
				<< "Content-Length: " << response_body.str().length()
				<< "\r\n\r\n"
				<< response_body.str();

			working_client->write(response.str().c_str(), response.str().length());

			working_client->_close();
		}
	}

	nothingness::network::shutdown_network_ssl();
	nothingness::network::shutdown_network();

	delete ls_engine;
	delete engine;
}