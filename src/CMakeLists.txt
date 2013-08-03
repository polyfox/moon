cmake_minimum_required(VERSION 2.8.8)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif()

if(ANDROID)
  set(moonLibraries ${moonLibraries} log android EGL GLESv2)
else()
  # OpenGL
  set(moonLibraries ${moonLibraries} ${OPENGL_LIBRARIES})
  # GLFW
  set(moonLibraries ${moonLibraries} ${GLFW_LIBRARY})
endif()

# SOIL
set(moonLibraries ${moonLibraries} SOIL)
# mruby
set(moonLibraries ${moonLibraries} ${MRUBY_LIBRARIES})

if(NOT ANDROID)
# gorilla audio
set(moonLibraries ${moonLibraries} gorilla)
endif()

set(moonSrc
  ${moonSrc}

  ext/mrb.cxx
  ext/mrb_sprite.cxx
  ext/mrb_spritesheet.cxx
  ext/mrb_window.cxx

  engine.cxx
  shader.cxx
  sprite.cxx
  spritesheet.cxx
  texture.cxx
)

if(NOT ANDROID)
  set(moonSrc
    ${moonSrc}
    ext/mrb_input.cxx
    ext/mrb_music.cxx
    ext/mrb_sound.cxx

    desktop/audio.cxx
    desktop/input.cxx
    desktop/fps.cxx
    desktop/main.cxx
    desktop/music.cxx
    desktop/sound.cxx
    desktop/window.cxx
  )
else()
  set(moonSrc
    ${moonSrc}
    android/android_native_app_glue.c
    android/main.cxx
    android/window.cxx
  )
endif()

add_library(moon STATIC ${moonSrc})

# mruby
add_dependencies(moon mruby)

target_link_libraries(moon ${moonLibraries})