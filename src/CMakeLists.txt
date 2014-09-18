cmake_minimum_required(VERSION 2.8.8)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif()

# YAML
set(MOON_LIBRARIES ${MOON_LIBRARIES} yaml)
# OpenGL
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${OPENGL_LIBRARIES})
# GLFW
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${GLFW_LIBRARY})
# GLEW
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${GLEW_LIBRARY})
# SOIL
set(MOON_LIBRARIES ${MOON_LIBRARIES} SOIL)
# mruby
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${MRUBY_LIBRARIES})
# gorilla audio
set(MOON_LIBRARIES ${MOON_LIBRARIES} gorilla)
# freetype2
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${FREETYPE_LIBRARY})
# freetype2
set(MOON_LIBRARIES ${MOON_LIBRARIES} freetype-gl)

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
  ext/mrb_context.cxx

  audio.cxx
  engine.cxx
  fps.cxx
  font.cxx
  input.cxx
  main.cxx
  shader.cxx
  sprite.cxx
  spritesheet.cxx
  texture.cxx
  vertex_buffer.cxx
  window.cxx
)

# mruby
add_dependencies(moon mruby)

target_link_libraries(moon ${MOON_LIBRARIES})
