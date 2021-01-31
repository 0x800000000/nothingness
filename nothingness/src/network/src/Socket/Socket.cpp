#include "Socket.h"

#ifndef WIN32

#define closesocket close

#endif 


nothingness::network::Socket::Socket(){
}

nothingness::network::Socket::~Socket(){
}
