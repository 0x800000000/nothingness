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
				this->Throw(new nothingness::nothingness_exception("domain not found!"));
				return;
			}

			addr.sin_addr.s_addr = ((in_addr*)host->h_addr_list[0])->s_addr;
			addr.sin_port = htons(port);
			addr.sin_family = AF_INET;
		}
		break;

		case NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN: {
			this->Throw(new nothingness::nothingness_exception("listen do not support \"NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN\"!"));
			return;
		}
		break;

	default:
		this->Throw(new nothingness::nothingness_exception("invalid input data!"));
		return;
		break;
	}

#ifdef WIN32
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
#else
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
#endif
	{
		this->Throw(new nothingness::nothingness_exception("cannot bind socket to the address!"));
		return;
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
			this->Throw(new nothingness::nothingness_exception("domain not found!"));
			return;
			return;
		}

		addr.sin_addr.s_addr = ((in_addr*)host->h_addr_list[0])->s_addr;
		addr.sin_port = htons(port);
		addr.sin_family = AF_INET;
	}

	case NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN: {
		this->Throw(new nothingness::nothingness_exception("connect do not support \"NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN\"!"));
		return;
	}
												 break;

	default:
		this->Throw(new nothingness::nothingness_exception("invalid input data!"));
		return;
		break;
	}
	
#ifdef WIN32
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
#else
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
#endif
	{
		this->Throw(new nothingness::nothingness_exception("cannot bind socket to the address!"));
		return;
	}

#ifdef WIN32
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
#else
	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) < 0)
#endif
	{
		this->Throw(new nothingness::nothingness_exception("failed connection to the server!"));
		return;
	}

	ssl = wolfSSL_new(ctx->getWolfSSLContext());
	
	if (ssl == NULL) {
		this->Throw(new nothingness::nothingness_exception("failed to create WOLFSSL object!"));
		return;
	}

	int ret;
	ret = wolfSSL_set_fd(ssl, sock);

	if (ret != WOLFSSL_SUCCESS) {
		this->Throw(new nothingness::nothingness_exception("failed to create WOLFSSL object!"));
		return;
	}

	ret = wolfSSL_connect(ssl);

	if (ret != SSL_SUCCESS) {
		this->Throw(new nothingness::nothingness_exception("failed connect to the server!(wolfssl)"));
		return;
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
		this->Throw(new nothingness::network::SSL::SSL_Socket_accept_exception("failed to accept connection!", SSL_Socket_accept_exception::type::error));
		close(tmp_sock);
		return nullptr;
	}

	WOLFSSL* tmp_ssl = wolfSSL_new(ctx->getWolfSSLContext());
	if (tmp_ssl == NULL) {
		this->Throw(new nothingness::network::SSL::SSL_Socket_accept_exception("failed to create WOLFSSL object!", SSL_Socket_accept_exception::type::error));
		wolfSSL_free(tmp_ssl);
		close(tmp_sock);
		return nullptr;
	}

	wolfSSL_set_fd(tmp_ssl, tmp_sock);

	int ret = wolfSSL_accept(tmp_ssl);

	if (ret != SSL_SUCCESS) {
		int error = wolfSSL_get_error(tmp_ssl, ret);

		std::string str_error = "wolfSSL_accept error = ";
		str_error += std::to_string(error);
		char buffer[80];

		wolfSSL_ERR_error_string(error, buffer);
		str_error += buffer;
		str_error = error;

		this->Throw(new nothingness::network::SSL::SSL_Socket_accept_exception(str_error.c_str(), SSL_Socket_accept_exception::type::warning));

		return new nothingness::network::SSL::SSL_Socket(ctx, tmp_ssl, tmp_sock);
	} 

	return new nothingness::network::SSL::SSL_Socket(ctx, tmp_ssl, tmp_sock);
}

int nothingness::network::SSL::SSL_Socket::write(const char* data, int len){
	int result;

	result = wolfSSL_write(ssl, data, len);
	
	if (result != len) {
		this->Throw(new nothingness::nothingness_exception("failed to write!"));
	}

	return result;
}

int nothingness::network::SSL::SSL_Socket::read(char* data, int len){
	int result;
	
	result = wolfSSL_read(ssl, data, len);
	
	if (result < 0) {
		this->Throw(new nothingness::nothingness_exception("failed to read!"));
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
		this->Throw(new nothingness::nothingness_exception("cannot initialize socket!"));
	}
}

void nothingness::network::SSL::SSL_Socket::_close(){
	wolfSSL_free(ssl);
	close(sock);
}

nothingness::network::SSL::SSL_Socket::~SSL_Socket(){
	
}

nothingness::network::SSL::SSL_Socket_accept_exception::SSL_Socket_accept_exception(const char* msg, type type) 
	: nothingness::nothingness_exception(msg),
	my_type(type)
{

}

nothingness::network::SSL::SSL_Socket_accept_exception::type nothingness::network::SSL::SSL_Socket_accept_exception::getType(){
	return my_type;
}
