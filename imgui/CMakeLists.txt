cmake_minimum_required(VERSION 3.5)
project(imgui)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add all source files in the src folder
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}
	*.cpp
	*.c
	*.h
	*.hpp)

# Add an executable target
add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(glfw)

target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(glad)

target_link_libraries(${PROJECT_NAME} glad)

