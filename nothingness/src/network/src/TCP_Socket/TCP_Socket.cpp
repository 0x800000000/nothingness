#include "TCP_Socket.h"

#ifndef WIN32

#define closesocket close

#endif 

nothingness::network::TCP_Socket::TCP_Socket(){
}

void nothingness::network::TCP_Socket::listen(const char* ip, uint16_t port, uint8_t input_data){
	struct sockaddr_in addr;
	
	switch (input_data){

		case NOTHINGNESS_SOCKET_INPUT_DATA_USE_IP_PORT: {
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr(ip);
			addr.sin_port = htons(port);
		}
		break;
	
		case NOTHINGNESS_SOCKET_INPUT_DATA_USE_ANY_IP_PORT: {
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = INADDR_ANY;
			addr.sin_port = htons(port);
		}
		break;
		
		case NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN_PORT: {

		}

		case NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN: {
			nothingness::network::last_error = "listen do not support \"NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN\"";
			nothingness::network::last_error_code = 1;
		}
		break;

	default:

		/*
		TO DO:
		error!
		*/
		break;
	}
}

int nothingness::network::TCP_Socket::write(const char* data, int len){
	return send(sock, data, len, NULL);
}

int nothingness::network::TCP_Socket::read(char* data, int len) {
	return recv(sock, data, len, NULL);
}

void nothingness::network::TCP_Socket::_close(){
	closesocket(sock);
}

nothingness::network::TCP_Socket::~TCP_Socket(){
}
