cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(nothingness_luaScriptEngine)

add_library(nothingness_luaScriptEngine STATIC "luaScriptEngine.h" "luaScriptEngine.cpp")

add_subdirectory("external/lua")
target_link_libraries(nothingness_luaScriptEngine lualib)

include_directories(nothingness_luaScriptEngine PRIVATE "external/luaBridge/Source/")
