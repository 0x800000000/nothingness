#include "TCP_Socket.h"

#ifdef WIN32

#define close closesocket 

#endif 

nothingness::network::TCP_Socket::TCP_Socket(){
#ifdef WIN32
	
	addrlen = sizeof(addr);

#else

	addrlen = sizeof(sockaddr);

#endif
}

nothingness::network::TCP_Socket::TCP_Socket(n_socket sock){
	this->sock = sock;
}

void nothingness::network::TCP_Socket::_listen(const char* ip, uint16_t port, uint8_t input_data){
	
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
			struct hostent* host = gethostbyname(ip);

			if (host == NULL) {
				nothingness::network::last_error = "domain not found!";
				nothingness::network::last_error_code = 1;
				return;
			}

			addr.sin_addr.s_addr = ((in_addr*)host->h_addr_list[0])->s_addr;
			addr.sin_port = htons(port);
			addr.sin_family = AF_INET;
		}

		case NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN: {
			nothingness::network::last_error = "listen do not support \"NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN\"";
			nothingness::network::last_error_code = 1;
		}
		break;

	default:
		nothingness::network::last_error = "invalid input data!";
		nothingness::network::last_error_code = 1;
		break;
	}

#ifdef WIN32
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
#else
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
#endif
	{
		nothingness::network::last_error = "cannot bind socket to the address!";
		nothingness::network::last_error_code = 1;
	}

	listen(sock, SOMAXCONN);
}

void nothingness::network::TCP_Socket::_connect(const char* ip, uint16_t port, uint8_t input_data) {

	switch (input_data) {

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
		struct hostent* host = gethostbyname(ip);

		if (host == NULL) {
			nothingness::network::last_error = "domain not found!";
			nothingness::network::last_error_code = 2;
			return;
		}

		addr.sin_addr.s_addr = ((in_addr*)host->h_addr_list[0])->s_addr;
		addr.sin_port = htons(port);
		addr.sin_family = AF_INET;
	}

	case NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN: {
		nothingness::network::last_error = "comming soon!";
		nothingness::network::last_error_code = 2;
	}
	break;

	default:
		nothingness::network::last_error = "invalid input data!";
		nothingness::network::last_error_code = 2;
		break;
	}

#ifdef WIN32
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
#else
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
#endif
	{
		nothingness::network::last_error = "cannot bind socket to the address!";
		nothingness::network::last_error_code = 2;
	}

#ifdef WIN32
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) 
#else
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
#endif
	{
		nothingness::network::last_error = "failed connection to the server";
		nothingness::network::last_error_code = 2;
	}
}

nothingness::network::Socket* nothingness::network::TCP_Socket::_accept(){
	n_socket tmp_sock;
	
#ifdef WIN32

	tmp_sock = accept(sock, (struct sockaddr*)&addr, &addrlen);

#else

	tmp_sock = accept(sock, (struct sockaddr*)&addr, (socklen_t*)&addrlen);


#endif

#ifdef WIN32
	if (tmp_sock == -1)
#else
	if (tmp_sock < 0)
#endif 
	{
		nothingness::network::last_error = "failed to accept the connection!";
		nothingness::network::last_error_code = 3;
	}
	

	return new nothingness::network::TCP_Socket(tmp_sock);
}

int nothingness::network::TCP_Socket::write(const char* data, int len){
	return send(sock, data, len, NULL);
}

int nothingness::network::TCP_Socket::read(char* data, int len) {
	return recv(sock, data, len, NULL);
}

void nothingness::network::TCP_Socket::create_socket(){
	sock = socket(AF_INET, SOCK_STREAM, NULL);
#ifdef WIN32
	if (sock == -1) 
#else
	if(sock < 0)
#endif 
	{
		nothingness::network::last_error = "cannot initialize socket!";
		nothingness::network::last_error_code = 101;
	}
}

void nothingness::network::TCP_Socket::_close(){
	close(sock);
}

nothingness::network::TCP_Socket::~TCP_Socket(){
}
