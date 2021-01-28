#include "..//Socket/Socket.h"

namespace nothingness {
	namespace network {
		class TCP_Socket : public Socket {

		public:

			TCP_Socket();

			void listen(const char* ip, uint16_t port, uint8_t input_data)override;

			int write(const char* data, int len)override;
			int read(char* data, int len)override;

			void _close() override;

			~TCP_Socket();
		};
	}
}