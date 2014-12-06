cmake_minimum_required(VERSION 2.6.4)

# Project name
project("Moon")

include("cmake/include.cmake")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MOON_DIR}/lib/${PLATFORM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MOON_DIR}/lib/${PLATFORM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MOON_DIR}/bin/${PLATFORM_NAME})

set(MOON_TARGET moon)

# SOIL
add_subdirectory(${MOON_DIR}/vendor/soil)

# Gorilla Audio
add_subdirectory(${MOON_DIR}/vendor/gorilla-audio/build)

# freetype-gl
set(freetype-gl_BUILD_DEMOS OFF)
add_subdirectory(${MOON_DIR}/vendor/freetype-gl)

# mruby
add_custom_command(
  OUTPUT libmruby.a
  COMMAND ./minirake MRUBY_CONFIG=../../mrb_config.rb
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/mruby
)
add_custom_target(mruby DEPENDS libmruby.a)

add_subdirectory(src)
