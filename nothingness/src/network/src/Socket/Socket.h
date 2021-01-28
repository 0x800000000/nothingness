#include"..//..//network.h"

#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_IP_PORT 0
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN_PORT 1
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN 2
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_ANY_IP_PORT 3

namespace nothingness {
	namespace network {
		class Socket {
		protected:

			n_socket sock;

			bool isClose = false;

		public:

			Socket();

			virtual void listen(const char* ip, uint16_t port, uint8_t input_data) = 0;

			virtual int write(const char* data, int len) = 0;
			virtual int read(char* data, int len) = 0;

			virtual void _close() = 0;

			~Socket();

		};
	}
}
