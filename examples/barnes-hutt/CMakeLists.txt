cmake_minimum_required(VERSION 3.5)
project(barnes-hutt)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

set(GUI_TYPE WIN32)

# Include directories
include_directories(include)

# Add all header files in the include folder
file(GLOB HEADER_FILES include/*.h)

# Add all shader files in the shader folder
file(GLOB SHADER_FILES shader/*.comp)

# Add all source files in the src folder
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Add an executable target
add_executable(${PROJECT_NAME} ${GUI_TYPE} ${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

target_link_libraries(${PROJECT_NAME} barebone-engine)

