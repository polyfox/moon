cmake_minimum_required(VERSION 2.8.8)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif()

# YAML
set(moonLibraries ${moonLibraries} yaml)
# OpenGL
set(moonLibraries ${moonLibraries} ${OPENGL_LIBRARIES})
# GLFW
set(moonLibraries ${moonLibraries} ${GLFW_LIBRARY})
# GLEW
set(moonLibraries ${moonLibraries} ${GLEW_LIBRARY})
# SOIL
set(moonLibraries ${moonLibraries} SOIL)
# mruby
set(moonLibraries ${moonLibraries} ${MRUBY_LIBRARIES})
# gorilla audio
set(moonLibraries ${moonLibraries} gorilla)
# freetype2
#set(moonLibraries ${moonLibraries} ${FREETYPE_LIBRARY})
# freetype2
set(moonLibraries ${moonLibraries} freetype-gl)

add_library(moon STATIC
  ext/mrb.cxx
  ext/mrb_rect.cxx
  ext/mrb_vector2.cxx
  ext/mrb_vector3.cxx
  ext/mrb_vector4.cxx
  ext/mrb_transform.cxx
  ext/mrb_input.cxx
  ext/mrb_music.cxx
  ext/mrb_sound.cxx
  ext/mrb_screen.cxx
  ext/mrb_texture.cxx
  ext/mrb_font.cxx
  ext/mrb_sprite.cxx
  ext/mrb_spritesheet.cxx
  ext/mrb_err.cxx

  audio.cxx
  engine.cxx
  fps.cxx
  font.cxx
  input.cxx
  main.cxx
  music.cxx
  shader.cxx
  sound.cxx
  sprite.cxx
  spritesheet.cxx
  texture.cxx
  vertex_buffer.cxx
  window.cxx
)

# mruby
add_dependencies(moon mruby)

target_link_libraries(moon ${moonLibraries})