#ifndef _NOTHINGNESS_SSL_SOCKET_
#define _NOTHINGNESS_SSL_SOCKET_

#include"..//Socket/Socket.h"

struct WOLFSSL;

namespace nothingness {
	namespace network {
		namespace SSL {
			class Context;

			class SSL_Socket : public Socket {

				Context* ctx;

				WOLFSSL* ssl;
				int addrlen;

			public:
				SSL_Socket(Context* ctx);
				SSL_Socket();
				SSL_Socket(Context* ctx, WOLFSSL* ssl, n_socket sock);

				void _listen(const char* ip, uint16_t port, uint8_t input_data) override;
				void _connect(const char* ip, uint16_t port, uint8_t input_data) override;

				Socket* _accept() override;

				int write(const char* data, int len) override;
				int read(char* data, int len) override;

				void create_socket() override;

				void _close()  override;

				~SSL_Socket();

			};
		}
	}
}
#endif