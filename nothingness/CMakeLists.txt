cmake_minimum_required (VERSION 3.8)

project(nothingness)

add_library(nothingness "platform.h" 
"src/network/network.h" "src/network/network.cpp" 
"src/luaScriptEngine/luaScriptEngine.h" "src/luaScriptEngine/luaScriptEngine.cpp" 
"include/nothingness/luaScriptEngine.h" "include/nothingness/network/network.h" 
"src/network/src/Socket/Socket.cpp" 
"src/network/src/TCP_Socket/TCP_Socket.h" "src/network/src/TCP_Socket/TCP_Socket.cpp"  "src/network/src/SSL_Socket/SSL_Socket.cpp")

add_subdirectory("src/logger")
add_subdirectory("src/nothingness")
add_subdirectory("src/luaScriptEngine")
add_subdirectory("src/network")

include_directories(nothingness_luaScriptEngine PRIVATE "src/luaScriptEngine/external/luaBridge/Source/")

target_link_libraries(nothingness nothingness_main nothingness_logger nothingness_luaScriptEngine nothingness_network)

target_include_directories(nothingness PUBLIC "include/")

if(WIN32)
  target_link_libraries(nothingness wsock32 ws2_32)
endif(WIN32)