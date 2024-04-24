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

# Linking ImGui
file(GLOB ImGuiSourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/*.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/*.h
			   ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.h
			   ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.h
			   ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.cpp)
add_library(imgui ${ImGuiSourceFiles})
target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/)
target_link_libraries(${PROJECT_NAME} imgui)
set_property(TARGET imgui PROPERTY FOLDER "libs")

# Linking LodePNG
file(GLOB LodePNGSourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/libs/lodepng/*.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/libs/lodepng/*.h)
add_library(lodepng ${LodePNGSourceFiles})
target_include_directories(${PROJECT_NAME} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/libs/lodepng/)
target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/lodepng/)
target_link_libraries(${PROJECT_NAME} lodepng)
set_property(TARGET lodepng PROPERTY FOLDER "libs")

# Linking GLFW
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw)
set_property(TARGET glfw PROPERTY FOLDER "libs")

# Linking GLM
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glm)
set_property(TARGET glm PROPERTY FOLDER "libs")


file(GLOB examples LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/examples/*)

foreach(subdir ${examples})
    if(IS_DIRECTORY ${subdir})
        add_subdirectory(${subdir})
	get_filename_component(target_name ${subdir} NAME_WE)
	set_property(TARGET ${target_name} PROPERTY FOLDER "Examples")
    endif()
endforeach()
