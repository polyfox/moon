project(moon)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

OPTION(freetype-gl_BUILD_DEMOS OFF)
OPTION(freetype-gl_BUILD_APIDOC OFF)
OPTION(GLFW_BUILD_EXAMPLES OFF)
OPTION(GLFW_BUILD_TESTS OFF)

add_subdirectory("vendor/freetype-gl")
add_subdirectory("vendor/glfw")
#add_subdirectory("vendor/glm")
add_subdirectory("vendor/gorilla-audio/build")
add_subdirectory("vendor/soil")
add_subdirectory("vendor/sil")

add_custom_target(moon-mruby ALL
  DEPENDS freetype-gl glfw gorilla SOIL SIL
  COMMAND ./minirake MRUBY_CONFIG="${CMAKE_SOURCE_DIR}/mrb_config.rb"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/mruby"
)

add_custom_target(moon-docs ALL
  COMMAND rake docs
  DEPENDS moon-mruby
)
