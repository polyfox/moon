cmake_minimum_required(VERSION 2.6.4)

# Project name
project("Moon")

set(MOON_DIR ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_MODULE_PATH ${MOON_DIR}/cmake)
# for gcc >= 4.7, or c++0x for 4.6
set(OPTIMIZATION_FLAG "-O3")
set(CXX_STD_FLAG "-std=c++0x")
#set(C_STD_FLAG "-std=c90")
set(WARNING_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STD_FLAG} ${WARNING_FLAGS} ${OPTIMIZATION_FLAG}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_STD_FLAG} ${WARNING_FLAGS} ${OPTIMIZATION_FLAG}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb2")

if(ANDROID)
  SET(PLATFORM_NAME "android")
else()
  SET(PLATFORM_NAME "host")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MOON_DIR}/lib/${PLATFORM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MOON_DIR}/lib/${PLATFORM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MOON_DIR}/bin/${PLATFORM_NAME})

set(MOON_TARGET moon)

# moon
link_directories(${MOON_DIR}/lib/${PLATFORM_NAME})
include_directories(${MOON_DIR}/include)

# YAML
find_library(YAML REQUIRED)
include_directories(${YAML_INCLUDE_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW3
#find_package(GLFW3 REQUIRED)
#include_directories(${GLFW3_INCLUDE_DIR})

# freetype2
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR})

# GLM
include_directories(${MOON_DIR}/vendor/glm)

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

# GLFW - 3.0.4
add_subdirectory(${MOON_DIR}/vendor/glfw)
include_directories(${MOON_DIR}/vendor/glfw/include)

# SOIL
add_subdirectory(${MOON_DIR}/vendor/soil)
find_library(SOIL_LIBRARY SOIL ${MOON_DIR}/vendor/soil/)
include_directories(${MOON_DIR}/vendor/soil/src)

# Gorilla Audio
add_subdirectory(${MOON_DIR}/vendor/gorilla-audio/build)
include_directories(${MOON_DIR}/vendor/gorilla-audio/include)
set(GORILLA_AUDIO_LIBRARIES openal gorilla)
if(UNIX)
  set(GORILLA_AUDIO_LIBRARIES pthread ${GORILLA_AUDIO_LIBRARIES})
endif(UNIX)

# freetype-gl
option(freetype-gl_BUILD_DEMOS "freetype-gl Demos" OFF)
add_subdirectory(${MOON_DIR}/vendor/freetype-gl)
include_directories(${MOON_DIR}/vendor/freetype-gl)

# mruby
add_custom_command(
  OUTPUT libmruby.a
  COMMAND ./minirake MRUBY_CONFIG=../../mrb_config.rb
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/mruby
)
add_custom_target(mruby DEPENDS libmruby.a)
include_directories(${MOON_DIR}/vendor/mruby/include)
set(MRUBY_LIBRARIES ${MOON_DIR}/vendor/mruby/build/host/lib/libmruby.a)
if(UNIX) # mrb-require needs for loading .so files
  set(MRUBY_LIBRARIES ${MRUBY_LIBRARIES} dl)
endif(UNIX)

# Libraries
# mruby
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${MRUBY_LIBRARIES})
# gorilla audio
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${GORILLA_AUDIO_LIBRARIES})
# freetype-gl
set(MOON_LIBRARIES ${MOON_LIBRARIES} freetype-gl)
# freetype2
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${FREETYPE_LIBRARY})
# SOIL
set(MOON_LIBRARIES ${MOON_LIBRARIES} SOIL)
# GLFW3
set(MOON_LIBRARIES ${MOON_LIBRARIES} glfw ${GLFW_LIBRARIES})
# GLEW
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${GLEW_LIBRARIES})
# OpenGL
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${OPENGL_LIBRARIES})
# YAML
set(MOON_LIBRARIES ${MOON_LIBRARIES} yaml)
# Math
set(MOON_LIBRARIES ${MOON_LIBRARIES} m)

set(MOON_LIBRARY moon ${MOON_LIBRARIES})

add_subdirectory(src)
