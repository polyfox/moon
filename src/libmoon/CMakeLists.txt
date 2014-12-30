cmake_minimum_required(VERSION 2.6.4)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif()

add_library(${MOON_TARGET} STATIC
  ext/mrb_context.cxx
  ext/mrb_err.cxx
  ext/mrb_font.cxx
  ext/mrb_input.cxx
  ext/mrb_music.cxx
  ext/mrb_rect.cxx
  ext/mrb_screen.cxx
  ext/mrb_sound.cxx
  ext/mrb_sprite.cxx
  ext/mrb_spritesheet.cxx
  ext/mrb_texture.cxx
  ext/mrb_transform.cxx
  ext/mrb_vector1.cxx
  ext/mrb_vector2.cxx
  ext/mrb_vector3.cxx
  ext/mrb_vector4.cxx
  ext/mrb.cxx

  audio.cxx
  engine.cxx
  font.cxx
  fps.cxx
  input.cxx
  shader.cxx
  sprite.cxx
  spritesheet.cxx
  texture.cxx
  vertex_buffer.cxx
  window.cxx
)

# mruby
add_dependencies(${MOON_TARGET} ${MOON_DEPENDENCIES})
target_link_libraries(${MOON_TARGET} ${MOON_LIBRARIES})
