cmake_minimum_required(VERSION 2.8.8)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Make sure the user is not executing this script directly
  message(FATAL_ERROR "Use the top-level cmake script!")
endif()

add_executable(game
  main.cxx
  ext/hola.cxx
)

add_custom_target(copy_scripts ALL
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scripts
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/core
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_HOME_DIRECTORY}/game/scripts/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scripts
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_HOME_DIRECTORY}/core/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/core
)

target_link_libraries(game moon)
