add_library(gamelib STATIC)

add_subdirectory(events)
add_subdirectory(game)
add_subdirectory(graphics)
add_subdirectory(maths)
add_subdirectory(messaging)
add_subdirectory(physics)
add_subdirectory(resources)
add_subdirectory(scripting)
add_subdirectory(tlv)
add_subdirectory(utils)

add_library(imguilib STATIC
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
 	${imgui_SOURCE_DIR}/imgui_demo.cpp
 	${imgui_SOURCE_DIR}/imgui_draw.cpp
 	${imgui_SOURCE_DIR}/imgui_tables.cpp
 	${imgui_SOURCE_DIR}/imgui_widgets.cpp

	${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
)

add_executable(game
	main.cpp
)

target_include_directories(imguilib PUBLIC ${imgui_SOURCE_DIR} ${imguizmo_SOURCE_DIR})

target_include_directories(gamelib PUBLIC ${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/src ${stb_lib_SOURCE_DIR} ${lua_SOURCE_DIR})
target_link_libraries(gamelib PUBLIC imguilib assimp Jolt opengl32 lua)
target_compile_features(gamelib PUBLIC cxx_std_23)
target_compile_definitions(gamelib PUBLIC -DNOMINMAX)
target_compile_options(gamelib PUBLIC /W4 /WX /Debug /Od)

target_link_libraries(game PUBLIC gamelib)
