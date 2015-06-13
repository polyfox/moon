project(moon)

cmake_minimum_required(VERSION 2.8.5)


add_subdirectory("vendor/gorilla-audio/build")
#add_subdirectory("vendor/glm")

option(freetype-gl_BUILD_DEMOS OFF)
option(freetype-gl_BUILD_APIDOC OFF)
add_subdirectory("vendor/freetype-gl")

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(BUILD_SHARED_LIBS ON)
set(BUILD_SHARED_LIBS ON)
add_subdirectory("vendor/glfw")

add_subdirectory("vendor/sil")
add_subdirectory("vendor/soil")

find_library(YAML yaml)

add_custom_target(moon-mruby ALL
  DEPENDS freetype-gl glfw gorilla SOIL SIL
  COMMAND ./minirake MRUBY_CONFIG="${CMAKE_SOURCE_DIR}/mrb_config.rb"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/mruby"
)

add_custom_target(moon-docs ALL
  COMMAND rake docs
  DEPENDS moon-mruby
)
