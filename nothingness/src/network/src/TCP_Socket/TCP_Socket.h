#ifndef _NOTHINGNESS_TCP_SOCKET_H
#define _NOTHINGNESS_TCP_SOCKET_H

#include "..//Socket/Socket.h"

namespace nothingness {
	namespace network {
		class TCP_Socket : public Socket {

			int addrlen;

		public:

			TCP_Socket();
			TCP_Socket(n_socket sock);

			void _listen(const char* ip, uint16_t port, uint8_t input_data) override;
			void _connect(const char* ip, uint16_t port, uint8_t input_data) override;
			
			Socket* _accept();

			int write(const char* data, int len) override;
			int read(char* data, int len) override;

			void create_socket() override;

			void _close()  override;

			~TCP_Socket();
		};
	}
}

#endif