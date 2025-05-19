add_executable(resource_packer
	main.cpp
)

target_include_directories(resource_packer PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/tools/resource_packer)

target_link_libraries(resource_packer gamelib)

