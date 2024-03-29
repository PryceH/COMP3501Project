cmake_minimum_required(VERSION 2.6)

# Name of project
set(PROJ_NAME ScreenSpaceDemo)
project(${PROJ_NAME})

# Specify project files: header files and source files
set(HDRS
    asteroid.h camera.h game.h model_loader.h resource.h resource_manager.h scene_graph.h scene_node.h sky.h tree.h light.h box.h
)
 
set(SRCS
    light.cpp tree.cpp sky.cpp asteroid.cpp camera.cpp game.cpp main.cpp resource.cpp resource_manager.cpp scene_graph.cpp scene_node.cpp box.cpp
    shader/material_fp.glsl shader/material_vp.glsl shader/metal_fp.glsl shader/metal_vp.glsl shader/plastic_fp.glsl shader/plastic_vp.glsl
    shader/textured_material_fp.glsl shader/textured_material_vp.glsl shader/three-term_shiny_blue_fp.glsl shader/three-term_shiny_blue_vp.glsl 
    shader/normal_map_vp.glsl shader/normal_map_fp.glsl shader/screen_space_vp.glsl shader/screen_space_fp.glsl shader/fire_fp.glsl shader/fire_vp.glsl shader/fire_gp.glsl
    shader/magic_fp.glsl shader/magic_vp.glsl shader/magic_gp.glsl shader/screen_space_magic_fp.glsl shader/screen_space_magic_vp.glsl shader/lit_fp.glsl shader/lit_vp.glsl
)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH D:\SchoolProject\COMP\3501\Libraries)
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
