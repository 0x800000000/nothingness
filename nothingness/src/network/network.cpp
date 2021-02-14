#include "network.h"
#include "src/TCP_Socket/TCP_Socket.h"
#include "src/SSL_Context/SSL_Context.h"
#include "src/SSL_Context_Client/SSL_Context_Client.h"
#include "src/SSL_Context_Server/SSL_Context_Server.h"
#include "src/SSL_Socket/SSL_Socket.h"

#include<wolfssl/ssl.h>

int nothingness::network::initalize_network(){
#ifdef WIN32
	int iResult;

	WSAData wsaData;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		return NOTHINGNESS_WSA_STARTUP_FAILED;
	}

#endif // WIN32

	return 0;
}

void nothingness::network::shutdown_network(){
#ifdef WIN32

	WSACleanup();

#endif 
}

nothingness::network::SSL::Context* nothingness::network::create_SSL_server_context(){
	return new nothingness::network::SSL::Context_Server();
}

nothingness::network::SSL::Context* nothingness::network::create_SSL_client_context(){
	return new nothingness::network::SSL::Context_client();
}

void nothingness::network::initalize_network_ssl(){
	wolfSSL_Init();
}

void nothingness::network::shutdown_network_ssl(){
	wolfSSL_Cleanup();
}

nothingness::network::Socket* nothingness::network::create_tcp_socket(){
	nothingness::network::TCP_Socket* tmp_sock = new nothingness::network::TCP_Socket();
	tmp_sock->create_socket();

	return tmp_sock;
}

nothingness::network::Socket* nothingness::network::create_ssl_socket(nothingness::network::SSL::Context* ctx){
	nothingness::network::SSL::SSL_Socket* tmp_sock = new nothingness::network::SSL::SSL_Socket(ctx);
	tmp_sock->create_socket();
	
	return tmp_sock;
}
