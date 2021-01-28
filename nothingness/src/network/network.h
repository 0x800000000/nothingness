#ifndef _NOTHINGNESS_NETWORK_H
#define _NOTHINGNESS_NETWORK_H

#include"..//..//platform.h"
#include<cstdint>

#ifdef WIN32
#include<winsock2.h>
typedef SOCKET n_socket;
#else
typedef uint32_t n_socket;
#endif // WIN32
#include <string>

namespace nothingness {
	namespace network {
		// if last_error_code > 0 we have an error!
		extern uint8_t last_error_code;
		extern std::string last_error;
	}
}

#endif // _NETWORK_H