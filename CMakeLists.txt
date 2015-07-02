cmake_minimum_required(VERSION 2.8.8)

project(moon)

find_library(YAML yaml)

add_subdirectory("vendor/sil")
# cmake doesn't like relative paths for include dirs, so we'll need the absolute
# path instead
get_filename_component(SIL_INCLUDE_DIR "vendor/sil/include" ABSOLUTE)
add_subdirectory("vendor/soil")
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


add_custom_target(moon-mruby ALL
  DEPENDS SIL SOIL glfw freetype-gl gorilla
  COMMAND ./minirake MRUBY_CONFIG="${CMAKE_SOURCE_DIR}/mrb_config.rb"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/mruby"
)

add_custom_target(moon-docs ALL
  COMMAND rake docs
  DEPENDS moon-mruby
)
