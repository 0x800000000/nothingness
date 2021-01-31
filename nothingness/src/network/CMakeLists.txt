project(nothingness_network)

add_library(nothingness_network STATIC "network.h" "network.cpp" 
#sockets
"src/Socket/Socket.h" "src/Socket/Socket.cpp" 
"src/TCP_Socket/TCP_Socket.h" "src/TCP_Socket/TCP_Socket.cpp" 
"src/SSL_Socket/SSL_Socket.h" "src/SSL_Socket/SSL_Socket.cpp" 

#contexts
"src/SSL_Context/SSL_Context.h" "src/SSL_Context/SSL_Context.cpp" 
"src/SSL_Context_Server/SSL_Context_Server.h" "src/SSL_Context_Server/SSL_Context_Server.cpp" 
"src/SSL_Context_Client/SSL_Context_Client.h" "src/SSL_Context_Client/SSL_Context_Client.cpp"
)

set(WOLFSSL_EXAMPLES OFF)

add_subdirectory("external/wolfssl")
target_link_libraries(nothingness_network wolfssl)

if(WIN32)
  target_link_libraries(nothingness_network wsock32 ws2_32)
endif(WIN32)
