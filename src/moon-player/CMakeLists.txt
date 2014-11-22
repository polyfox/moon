cmake_minimum_required(VERSION 2.8.8)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(PLAYER_NAME "moon-player")
set(PLAYER_SRC
  main.cxx
)

add_executable(${PLAYER_NAME} ${PLAYER_SRC})
target_link_libraries(${PLAYER_NAME} ${MOON_LIBRARY})
