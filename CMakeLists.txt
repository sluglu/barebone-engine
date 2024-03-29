cmake_minimum_required(VERSION 3.5)
project(barebone-engine)

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

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(imgui)

target_link_libraries(${PROJECT_NAME} imgui)

add_subdirectory(glm)

target_link_libraries(${PROJECT_NAME} glm)

add_subdirectory(lodepng)

target_link_libraries(${PROJECT_NAME} lodepng)

file(GLOB examples LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/examples/*)

foreach(subdir ${examples})
    if(IS_DIRECTORY ${subdir})
        add_subdirectory(${subdir})
	get_filename_component(target_name ${subdir} NAME_WE)
	set_property(TARGET ${target_name} PROPERTY FOLDER "Examples")
    endif()
endforeach()
