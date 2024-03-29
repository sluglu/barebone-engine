cmake_minimum_required(VERSION 3.5)
project(huegene)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

set(GUI_TYPE WIN32)

include_directories(./)

file(GLOB SOURCE_FILES "*.cpp" "*.h")

# Add an executable target
add_executable(${PROJECT_NAME} ${GUI_TYPE} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} barebone-engine)



