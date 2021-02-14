#ifndef _NOTHINGNESS_NETWORK_H
#define _NOTHINGNESS_NETWORK_H

#include<cstdint>



#ifdef WIN32
#define NOTHINGNESS_WSA_STARTUP_FAILED 0x1
#include<winsock2.h>
typedef SOCKET n_socket;
#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
typedef uint32_t n_socket;
#endif // WIN32
#include <string>

struct sockaddr_in;
namespace nothingness {
	namespace network {
		namespace SSL {
			class Context;
		}

		class Socket;

		int initalize_network();
		void shutdown_network();

		nothingness::network::SSL::Context* create_SSL_server_context();
		nothingness::network::SSL::Context* create_SSL_client_context();

		void initalize_network_ssl();
		void shutdown_network_ssl();

		Socket* create_tcp_socket();
		Socket* create_ssl_socket(nothingness::network::SSL::Context* ctx);

	}
}

#endif // _NETWORK_H