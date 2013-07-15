cmake_minimum_required(VERSION 2.8.8)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../build/")

add_executable(game
  main.cxx
  ext/hola.cxx
)

target_link_libraries(game moon)