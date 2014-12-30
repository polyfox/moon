cmake_minimum_required(VERSION 2.6.4)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(MOON_PLAYER_NAME "moon-player")
set(MOON_PLAYER_SRC
  main.cxx
)

add_executable(${MOON_PLAYER_NAME} ${MOON_PLAYER_SRC})
add_dependencies(${MOON_PLAYER_NAME} ${MOON_DEPENDENCY})
target_link_libraries(${MOON_PLAYER_NAME} ${MOON_LIBRARY})
