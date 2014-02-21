cmake_minimum_required(VERSION 2.8.8)

# Project name
project("Moon")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # for gcc >= 4.7, or c++0x for 4.6

if(ANDROID)
  SET(PLATFORM_NAME "android")
else()
  SET(PLATFORM_NAME "host")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib/${PLATFORM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib/${PLATFORM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin/${PLATFORM_NAME})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

# SOIL
add_subdirectory(vendor/soil)
include_directories(vendor/soil/src)

# mruby
add_custom_command(
  OUTPUT libmruby.a
  COMMAND ./minirake MRUBY_CONFIG=../../mrb_config.rb
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/mruby
)
add_custom_target(mruby DEPENDS libmruby.a)
include_directories(vendor/mruby/include)
set(MRUBY_LIBRARIES ${CMAKE_SOURCE_DIR}/vendor/mruby/build/host/lib/libmruby.a)
if(UNIX) # mrb-require needs for loading .so files
  set(MRUBY_LIBRARIES ${MRUBY_LIBRARIES} dl)
endif(UNIX)

# GLM
include_directories(vendor/glm)

# Gorilla Audio
add_subdirectory(vendor/gorilla-audio/build)
include_directories(vendor/gorilla-audio/include)

# freetype2
#find_package(Freetype REQUIRED)
#include_directories(${FREETYPE_INCLUDE_DIR})

# freetype-gl
add_subdirectory(vendor/freetype-gl)
include_directories(vendor/freetype-gl)

include_directories(src/include)

add_subdirectory(src)
add_subdirectory(game)
