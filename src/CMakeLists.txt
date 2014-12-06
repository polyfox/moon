cmake_minimum_required(VERSION 2.6.4)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif()

add_subdirectory(libmoon)
add_subdirectory(moon-player)
