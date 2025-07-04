cmake_minimum_required (VERSION 3.13)
project (lua)

message("[nothingness][lua] lua is successfuly connected")

set(LIBSOURCES
	src/lapi.c
	src/lauxlib.c
	src/lbaselib.c
	src/lcode.c
	src/ldblib.c
	src/ldebug.c
	src/ldo.c
	src/ldump.c
	src/lfunc.c
	src/lgc.c
	src/linit.c
	src/liolib.c
	src/llex.c
	src/lmathlib.c
	src/lmem.c
	src/loadlib.c
	src/lobject.c
	src/lopcodes.c
	src/loslib.c
	src/lparser.c
	src/lstate.c
	src/lstring.c
	src/lstrlib.c
	src/ltable.c
	src/ltablib.c
	src/ltm.c
	src/lundump.c
	src/lvm.c
	src/lzio.c
	src/print.c
)

set(LIBHEADERS
	src/lapi.h
	src/lauxlib.h
	src/lcode.h
	src/ldebug.h
	src/ldo.h
	src/lfunc.h
	src/lgc.h
	src/llex.h
	src/llimits.h
	src/lmem.h
	src/lobject.h
	src/lopcodes.h
	src/lparser.h
	src/lstate.h
	src/lstring.h
	src/ltable.h
	src/ltm.h
	src/lua.h
	src/luaconf.h
	src/lualib.h
	src/lundump.h
	src/lvm.h
	src/lzio.h
)



# Lua needs to be linked dynamically on Windows and statically on *nix, so that LuaRocks work:
if (WIN32)
	add_library(lualib SHARED ${LIBSOURCES} ${LIBHEADERS})

	target_compile_definitions(lualib PUBLIC LUA_BUILD_AS_DLL)

	set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/nothingness)

	# Output the DLL into the Server subfolder, so that Cuberite can find it:
	SET_TARGET_PROPERTIES(lualib PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_BINARY_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_BINARY_DIR}/Server
	)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Server)

	if (MSVC)
		# Remove SCL warnings, we expect this library to have been tested safe
		SET_TARGET_PROPERTIES(
			lualib PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_WARNINGS"
		)
	endif()

	set_target_properties(lualib PROPERTIES OUTPUT_NAME "lua51" PREFIX "")

	# NOTE: The DLL for each configuration is stored at the same place, thus overwriting each other.
	# This is known, however such behavior is needed for LuaRocks - they always load "lua5.1.dll" or "lua51.dll"
	# We make it work by compiling to "lua51.dll" and providing a proxy-DLL "lua5.1.dll"
	# See http://lua-users.org/wiki/LuaProxyDllFour for details
else()
	add_library(lualib ${LIBSOURCES} ${LIBHEADERS})
endif()


# Tell the compiler to use C++ when compiling the library,
# so that Lua uses C++ exceptions instead of setjmp / longjmp (GH #3678):
SET_SOURCE_FILES_PROPERTIES(${SOURCE} PROPERTIES LANGUAGE CXX)

# Tell Lua what dynamic loader to use (for LuaRocks):
if (UNIX)
	add_definitions(-DLUA_USE_DLOPEN)
	target_link_libraries(lualib m ${CMAKE_DL_LIBS})
endif()

# Let Lua use additional checks on its C API. This is only compiled into Debug builds:
target_compile_definitions(lualib PRIVATE LUA_USE_APICHECK)

target_include_directories(lualib INTERFACE "${CMAKE_CURRENT_LIST_DIR}/src")


# If not crosscompiling, build a local Lua:
if (NOT(CMAKE_CROSSCOMPILING))
	add_executable(lua src/lua.c)
	target_link_libraries(lua lualib)
	# Output the EXE into the Server subfolder, so that it can find lua51.dll:
	SET_TARGET_PROPERTIES(lua PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_BINARY_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_BINARY_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_BINARY_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_BINARY_DIR}/Server
	)
	if (MSVC)
		set_target_properties(
			lua
			PROPERTIES FOLDER Support
		)
	endif()
endif()
