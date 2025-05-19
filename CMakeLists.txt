cmake_minimum_required(VERSION 3.22)

project(
	game
	VERSION "0.0.1"
	DESCRIPTION "a sweet game"
	LANGUAGES C CXX
)

include(FetchContent)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.15.2)
FetchContent_GetProperties(googletest)

FetchContent_Declare(
	stb_lib
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG 5c205738c191bcb0abc65c4febfa9bd25ff35234)
FetchContent_MakeAvailable(stb_lib)

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.91.6)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
	imguizmo
	GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo
	GIT_TAG b10e91756d32395f5c1fefd417899b657ed7cb88)
FetchContent_MakeAvailable(imguizmo)

FetchContent_Declare(
	lua
	GIT_REPOSITORY https://github.com/lua/lua
	GIT_TAG v5.4.7)
FetchContent_MakeAvailable(lua)

set(USE_STATIC_MSVC_RUNTIME_LIBRARY ON CACHE BOOL "" FORCE)
FetchContent_Declare(
	jolt
	GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics
	GIT_TAG v5.2.0
	SOURCE_SUBDIR Build)
FetchContent_MakeAvailable(jolt)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp
	GIT_TAG v5.4.3)
FetchContent_MakeAvailable(assimp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET FALSE)

add_library(lua STATIC ${lua_SOURCE_DIR}/onelua.c)
target_compile_definitions(lua PRIVATE MAKE_LIB)

add_subdirectory(src)
add_subdirectory(tools)

enable_testing()
include(CTest)
add_subdirectory(tests)

