
#include"..//..//platform.h"
#include<cstdint>

#ifdef WIN32
#include<winsock2.h>
typedef SOCKET n_socket;
#else
typedef uint32_t n_socket;
#endif // WIN32


namespace nothingness {

	class NetWorkApi {

	};
}