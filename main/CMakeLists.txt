cmake_minimum_required(VERSION 3.8)

include_directories("../nothingness/src/luaScriptEngine/external/luaBridge/Source/")

add_executable(nothingness_server main.h main.cpp)
target_link_libraries(nothingness_server nothingness)

add_custom_command(TARGET nothingness_server POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_CURRENT_LIST_DIR}/resources $<TARGET_FILE_DIR:nothingness_server>/resources)
