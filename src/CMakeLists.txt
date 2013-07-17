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
  ext/mrb_music.cxx
  ext/mrb_sound.cxx
  ext/mrb_sprite.cxx
  ext/mrb_spritesheet.cxx

  include/moon.hxx
  include/audio.hxx
  include/engine.hxx
  include/fps.hxx
  include/music.hxx
  include/sound.hxx
  include/sprite.hxx
  include/spritesheet.hxx
  include/texture.hxx
  
  audio.cxx
  engine.cxx
  fps.cxx
  music.cxx
  sound.cxx
  sprite.cxx
  spritesheet.cxx
  texture.cxx
)

# mruby
add_dependencies(moon mruby)

target_link_libraries(moon ${moonLibraries})