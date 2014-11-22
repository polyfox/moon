set(MOON_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

set(CMAKE_MODULE_PATH ${MOON_DIR}/cmake)
# for gcc >= 4.7, or c++0x for 4.6
set(OPTIMIZATION_FLAG "-O3")
set(CXX_STD_FLAG "-std=c++11")
#set(C_STD_FLAG "-std=c90")
set(WARNING_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STD_FLAG} ${WARNING_FLAGS} ${OPTIMIZATION_FLAG}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_STD_FLAG} ${WARNING_FLAGS} ${OPTIMIZATION_FLAG}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb2")

if(ANDROID)
  SET(PLATFORM_NAME "android")
else()
  SET(PLATFORM_NAME "host")
endif()

link_directories(${MOON_DIR}/lib/${PLATFORM_NAME})
include_directories(${MOON_DIR}/include)

# YAML
find_library(YAML REQUIRED)
include_directories(${YAML_INCLUDE_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

# SOIL
find_library(SOIL_LIBRARY SOIL ${MOON_DIR}/vendor/soil/)
include_directories(${MOON_DIR}/vendor/soil/src)

# mruby
include_directories(${MOON_DIR}/vendor/mruby/include)
set(MRUBY_LIBRARIES ${MOON_DIR}/vendor/mruby/build/host/lib/libmruby.a)
if(UNIX) # mrb-require needs for loading .so files
  set(MRUBY_LIBRARIES ${MRUBY_LIBRARIES} dl)
endif(UNIX)

# GLM
include_directories(${MOON_DIR}/vendor/glm)

# Gorilla Audio
include_directories(${MOON_DIR}/vendor/gorilla-audio/include)
set(GORILLA_AUDIO_LIBRARIES openal gorilla)
if(UNIX)
  set(GORILLA_AUDIO_LIBRARIES pthread ${GORILLA_AUDIO_LIBRARIES})
endif(UNIX)

# freetype2
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR})

# freetype-gl
include_directories(${MOON_DIR}/vendor/freetype-gl)

# Math
set(MOON_LIBRARIES ${MOON_LIBRARIES} m)
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
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${GORILLA_AUDIO_LIBRARIES})
# freetype2
set(MOON_LIBRARIES ${MOON_LIBRARIES} ${FREETYPE_LIBRARY})
# freetype-gl
set(MOON_LIBRARIES ${MOON_LIBRARIES} freetype-gl)

set(MOON_LIBRARY moon ${MOON_LIBRARIES})
