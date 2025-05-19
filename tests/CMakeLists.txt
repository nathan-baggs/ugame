FetchContent_MakeAvailable(googletest)

include(GoogleTest)

mark_as_advanced(BUILD_GMOCK BUILD_GTEST gtest_hide_internal_symbols)

add_executable(unit_tests
	auto_release_tests.cpp
	camera_tests.cpp
	chain_tests.cpp
	error_tests.cpp
	frustum_plane_tests.cpp
	lua_interop_tests.cpp
	lua_script_tests.cpp
	matrix3_tests.cpp
	matrix4_tests.cpp
	message_bus_tests.cpp
	resource_cache_tests.cpp
	script_runner_tests.cpp
	shape_wireframe_renderer_tests.cpp
	tlv_tests.cpp
	vector3_tests.cpp
	vector4_tests.cpp
)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(unit_tests gmock_main gamelib)
target_compile_options(unit_tests PUBLIC /W4 /WX /Debug /Od)
gtest_discover_tests(unit_tests DISCOVERY_MODE PRE_TEST)

