cmake_minimum_required(VERSION 2.8.8)

# Project name
project("Moon")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # for gcc >= 4.7, or c++0x for 4.6
if(ANDROID)
  set(PLATFORM_NAME "android")

  set(ANDROID_TOOLCHAIN_NAME "arm-linux-androideabi-4.8")
  set(ANDROID_NATIVE_API_LEVEL "android-18")

  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/android.toolchain.cmake)
else()
  set(PLATFORM_NAME "host")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib/${PLATFORM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib/${PLATFORM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin/${PLATFORM_NAME})

message(STATUS "PLATFORM_NAME: " ${PLATFORM_NAME})

if(ANDROID)
  # Android auto includes ES 2.0 and EGL
else()
  # OpenGL
  find_package(OpenGL REQUIRED)
  include_directories(${OPENGL_INCLUDE_DIR})

  # GLFW
  find_package(GLFW REQUIRED)
  include_directories(${GLFW_INCLUDE_DIR})

  # Gorilla Audio
  add_subdirectory(vendor/gorilla-audio/build)
  include_directories(vendor/gorilla-audio/include)
endif()

# SOIL
add_subdirectory(vendor/soil)
include_directories(vendor/soil/src)

# mruby
add_custom_command(
  OUTPUT libmruby.a
  COMMAND ./minirake MRUBY_CONFIG=../../mrb_config.rb
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/mruby
)
add_custom_target(mruby DEPENDS libmruby.a)
include_directories(vendor/mruby/include)
if(ANDROID)
  set(MRUBY_LIBRARIES -Wl,--whole-archive ${CMAKE_SOURCE_DIR}/vendor/mruby/build/androideabi/lib/libmruby.a -Wl,--no-whole-archive)
else()
  set(MRUBY_LIBRARIES ${CMAKE_SOURCE_DIR}/vendor/mruby/build/host/lib/libmruby.a)
endif()
if(UNIX) # mrb-require needs for loading .so files
  set(MRUBY_LIBRARIES ${MRUBY_LIBRARIES} dl)
endif(UNIX)

# NOTE: get -Wl,--whole-archive <lib> -Wl,--no-whole-archive
# working for libmoon too. 

# GLM
include_directories(vendor/glm)

include_directories(src/include)

if(ANDROID)
  include_directories(src/android/include)
else()
  include_directories(src/desktop/include)
endif()

add_subdirectory(src)
add_subdirectory(game)
