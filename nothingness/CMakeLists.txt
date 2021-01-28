cmake_minimum_required (VERSION 3.8)

project(nothingness)

add_library(nothingness "platform.h" "src/network/network.h" "src/network/network.cpp" "src/luaScriptEngine/luaScriptEngine.h" "src/luaScriptEngine/luaScriptEngine.cpp" "include/nothingness/luaScriptEngine.h")

add_subdirectory("src/logger")
add_subdirectory("src/nothingness")
add_subdirectory("src/luaScriptEngine")

include_directories(nothingness_luaScriptEngine PRIVATE "src/luaScriptEngine/external/luaBridge/Source/")

target_link_libraries(nothingness nothingness_main nothingness_logger nothingness_luaScriptEngine)

target_include_directories(nothingness PUBLIC "include/")