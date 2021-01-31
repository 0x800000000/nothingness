#include "SSL_Socket.h"
#include<wolfssl/options.h>
#include <wolfssl/ssl.h>
#include "..//SSL_Context/SSL_Context.h"

#ifdef WIN32

#define close closesocket

#endif 

nothingness::network::SSL::SSL_Socket::SSL_Socket(Context* ctx) : ctx(ctx){
#ifdef WIN32

	addrlen = sizeof(addr);

#else

	addrlen = sizeof(sockaddr);

#endif
}

nothingness::network::SSL::SSL_Socket::SSL_Socket(){
#ifdef WIN32

	addrlen = sizeof(addr);

#else

	addrlen = sizeof(sockaddr);

#endif
}

nothingness::network::SSL::SSL_Socket::SSL_Socket(Context* ctx, WOLFSSL* ssl, n_socket sock) : ssl(ssl), ctx(ctx){
	sock = sock;
}


void nothingness::network::SSL::SSL_Socket::_listen(const char* ip, uint16_t port, uint8_t input_data){
	
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
				nothingness::network::last_error_code = 1;
				return;
			}

			addr.sin_addr.s_addr = ((in_addr*)host->h_addr_list[0])->s_addr;
			addr.sin_port = htons(port);
			addr.sin_family = AF_INET;
		}
														  break;

		case NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN: {
			nothingness::network::last_error = "listen do not support \"NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN\"";
			nothingness::network::last_error_code = 51;
		}
												 break;

	default:
		nothingness::network::last_error = "invalid input data!";
		nothingness::network::last_error_code = 51;
		break;
	}

#ifdef WIN32
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
#else
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
#endif
	{
		nothingness::network::last_error = "cannot bind socket to the address!";
		nothingness::network::last_error_code = 51;
	}

	listen(sock, SOMAXCONN);
}

void nothingness::network::SSL::SSL_Socket::_connect(const char* ip, uint16_t port, uint8_t input_data){
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
	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) < 0)
#endif
	{
		nothingness::network::last_error = "failed connection to the server";
		nothingness::network::last_error_code = 2;
	}

	ssl = wolfSSL_new(ctx->getWolfSSLContext());
	
	if (ssl == NULL) {
		nothingness::network::last_error = "failed to create WOLFSSL object!";
		nothingness::network::last_error_code = 51;
	}

	int ret;
	ret = wolfSSL_set_fd(ssl, sock);

	if (ret != WOLFSSL_SUCCESS) {
		nothingness::network::last_error = "Failed to set the file descriptor!";
		nothingness::network::last_error_code = 51;
	}

	ret = wolfSSL_connect(ssl);

	if (ret != SSL_SUCCESS) {
		nothingness::network::last_error = "failed to connect to wolfSSL!";
		nothingness::network::last_error_code = 51;
	}
}

nothingness::network::Socket* nothingness::network::SSL::SSL_Socket::_accept(){
	n_socket tmp_sock = 0;
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
		nothingness::network::last_error_code = 51;
		return nullptr;
	}

	WOLFSSL* tmp_ssl = wolfSSL_new(ctx->getWolfSSLContext());
	if (tmp_ssl == NULL) {
		nothingness::network::last_error = "failed to create WOLFSSL object!";
		nothingness::network::last_error_code = 513;
		return nullptr;
	}

	wolfSSL_set_fd(tmp_ssl, tmp_sock);

	int ret = wolfSSL_accept(tmp_ssl);

	if (ret != SSL_SUCCESS) {
		int error = wolfSSL_get_error(tmp_ssl, ret);

		nothingness::network::last_error = "wolfSSL_accept error = ";
		nothingness::network::last_error += std::to_string(error);
		char buffer[80];

		wolfSSL_ERR_error_string(error, buffer);
		nothingness::network::last_error += buffer;
		nothingness::network::last_error_code = error;
		return new nothingness::network::SSL::SSL_Socket(ctx, tmp_ssl, tmp_sock);;
	} 

	return new nothingness::network::SSL::SSL_Socket(ctx, tmp_ssl, tmp_sock);
}

int nothingness::network::SSL::SSL_Socket::write(const char* data, int len){
	int result;

	result = wolfSSL_write(ssl, data, len);
	if (result != len) {
		nothingness::network::last_error = "failed to write!";
		nothingness::network::last_error_code = 51;
	}
	return result;
}

int nothingness::network::SSL::SSL_Socket::read(char* data, int len){
	int result;
	
	result = wolfSSL_read(ssl, data, len);
	
	if (result < 0) {
		nothingness::network::last_error = "failed to read!";
		nothingness::network::last_error_code = 51;
	}

	return result;
}

void nothingness::network::SSL::SSL_Socket::create_socket() {
	sock = socket(AF_INET, SOCK_STREAM, 0);
#ifdef WIN32
	if (sock == -1)
#else
	if (sock < 0)
#endif 
	{
		nothingness::network::last_error = "cannot initialize socket!";
		nothingness::network::last_error_code = 51;
	}
}

void nothingness::network::SSL::SSL_Socket::_close(){
	wolfSSL_free(ssl);
	close(sock);
}

nothingness::network::SSL::SSL_Socket::~SSL_Socket(){
	
}
