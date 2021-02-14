cmake_minimum_required (VERSION 3.8)

project(nothingness)

add_library(nothingness STATIC "platform.h" 

"src/logger/logger.h"
"src/logger/logger.cpp"

"src/resource_manager/resource_manager.h" 
"src/resource_manager/resouce_manager.cpp"

"src/luaScriptEngine/luaScriptEngine.h" "src/luaScriptEngine/luaScriptEngine.cpp" 
"include/nothingness/luaScriptEngine.h" 
 "src/error_handler/error_handler.h" "src/error_handler/error_handler.cpp" "src/error_handler/error_handler.cpp")

add_subdirectory("src/network")

#lua
add_subdirectory("src/luaScriptEngine/external/lua")
include_directories(nothingness public "src/luaScriptEngine/external/luaBridge/Source/")
include_directories(nothingness public "src/resource_manager/external/")

target_link_libraries(nothingness lualib nothingness_network)

target_include_directories(nothingness PUBLIC "include/")
