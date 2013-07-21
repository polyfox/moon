cmake_minimum_required(VERSION 2.8.8)

#    Project name
project("Moon")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # for gcc >= 4.7, or c++0x for 4.6

set(DebugDefines "_DEBUG")

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

# DevIL
find_package(DevIL REQUIRED)
include_directories(${IL_INCLUDE_DIR})

# mruby
add_custom_command(
  OUTPUT libmruby.a
  COMMAND ./minirake MRUBY_CONFIG=../../mrb_config.rb
  WORKING_DIRECTORY vendor/mruby
)
add_custom_target(mruby DEPENDS libmruby.a)
include_directories(vendor/mruby/include)
link_directories(vendor/mruby/build/host/lib)

# Gorilla Audio
add_subdirectory(vendor/gorilla-audio/build)
include_directories(vendor/gorilla-audio/include)
#link_directories(vendor/gorilla-audio/build)

include_directories(src/include)

add_subdirectory(src)
add_subdirectory(game)
