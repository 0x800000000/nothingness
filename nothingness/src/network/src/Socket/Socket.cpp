#include "Socket.h"

#ifndef WIN32

#define closesocket close

#endif 


nothingness::network::Socket::Socket(){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	

}

nothingness::network::Socket::~Socket(){
	if (!isClose)
		closesocket(sock);
}
