cmake_minimum_required(VERSION 2.8.8)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

# OpenGL
set(moonLibraries ${moonLibraries} ${OPENGL_LIBRARIES})
# GLFW
set(moonLibraries ${moonLibraries} ${GLFW_LIBRARY})
# DevIL
set(moonLibraries ${moonLibraries} ${IL_LIBRARY} ${ILU_LIBRARY})
# mruby
set(moonLibraries ${moonLibraries} libmruby.a)
# gorilla audio
set(moonLibraries ${moonLibraries} gorilla)

add_library(moon STATIC
  ext/mrb.cxx
  ext/mrb_sound.cxx
  ext/mrb_sprite.cxx
  ext/mrb_spritesheet.cxx
  audio.cxx
  audio.hxx
  engine.cxx
  engine.hxx
  fps.cxx
  fps.hxx
  moon.hxx
  music.cxx
  music.hxx
  sound.cxx
  sound.hxx
  sprite.cxx
  sprite.hxx
  spritesheet.cxx
  spritesheet.hxx
  texture.cxx
  texture.hxx
)

# mruby
add_dependencies(moon mruby)

target_link_libraries(moon ${moonLibraries})