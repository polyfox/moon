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
set(freetype-gl_BUILD_DEMOS OFF)
set(freetype-gl_BUILD_APIDOC OFF)
add_subdirectory("vendor/freetype-gl")

option(BUILD_SHARED_LIBS ON)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS ON)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory("vendor/glfw")
