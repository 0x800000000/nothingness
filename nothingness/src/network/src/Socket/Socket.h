#ifndef _NOTHINGNESS_SOCKET_H
#define _NOTHINGNESS_SOCKET_H
#include"..//..//network.h"

#include "..//..//..//error_handler/error_handler.h"

#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_IP_PORT 0
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN_PORT 1
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN 2
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_ANY_IP_PORT 3

namespace nothingness {
	namespace network {
		class Socket : public nothingness::error_handler {
		protected:

			n_socket sock;
			sockaddr_in addr;

			bool isClose = false;

		public:

			Socket();

			virtual void _listen(const char* ip, uint16_t port, uint8_t input_data) = 0;
			virtual void _connect(const char* ip, uint16_t port, uint8_t input_data) = 0;

			virtual int write(const char* data, int len) = 0;
			virtual int read(char* data, int len) = 0;

			virtual void create_socket() = 0;

			virtual void _close() = 0;

			virtual Socket* _accept() = 0;

			virtual void operator=(Socket&) = delete;

			~Socket();

		};
	}
}
#endif