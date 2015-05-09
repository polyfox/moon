# [![Moon](https://raw.githubusercontent.com/polyfox/moon/master/moon-logo.png)](https://raw.githubusercontent.com/polyfox/moon/master/moon-logo.png) Moon
[![Build Status](https://travis-ci.org/polyfox/moon.svg?branch=master)](https://travis-ci.org/polyfox/moon)

## Introduction
Moon is a experimental prototyping 2D Game Engine written in C++ and scripted using
mruby.

## Projects using Moon
Lunar Howl (MMO RPG)
es-moon (Roguelike RPG)
Lunar Metal (2D Top-Down RTS)

## Dependencies
Moon ships with most of its dependencies in the vendor directory
```
freetype-gl
glfw (3)
glm
gorilla-audio
mruby
soil
```

## Building
If you are downloading moon for the first time from git you can use:
```
git clone --recursive https://github.com/archSeer/moon.git
```
This will grab moon and all its submodules.





__ORIGINAL_README__
## Moon and Lunar Howl

Moon is the name of the engine (libmoon), running OpenGL, GLFW and mruby.

Lunar Howl is the name of the going-to-be-built game on top of the Moon engine.

Requires GLFW3 externally installed. Manually compiles and statically links
mruby, gorilla audio, GLM and soil.

NOTE: To define mruby C extensions, you need to include a `void moon_init_mrb_ext(mrb_state *mrb)`
function, that does the same form of initialization as an "Init_<name>" function would do. This
function needs to be declared (and just left empty) even if you don't use any mruby C code, or
the compiler will crash.

## Support

We support graphics cards with OpenGL 2.1 support and the following extensions:

- GL_ARB_draw_elements_base_vertex
- GL_ARB_vertex_array_object

Cards that support OpenGL 3.0+ naturally support these extensions, because they
are a part of the OpenGL 3.x series. (We basically use a 3.0 context with only
the things we need that are widely supported on 2.1 cards so we have wider support).

## How to build

After cloning, you need to fetch the git submodules (vendor files):

```
git submodule update --init
```

GLFW 3 is required to be externally installed at the moment. If you are using Windows, you should
define an environment variable called GLFW_ROOT which points to the root of the glfw library
installation. This is pretty useful on a Windows platform.

The above also applies for freetype and GLEW.

Compilation is done via the CMake build system. Use cmake in the build folder to generate your
Makefiles or (VS/CodeBlocks) projects of choice for your platform:

```
cd build
cmake ..
```

Then simply compile it inside the build directory. Compiled binaries and scripts will be placed into
`bin/` and compiled libraries into `lib/` (not the `build/` folder! that one is just where you run
the compiler and where cmake will store it's files).

Note that mruby will not be placed into the `lib/` folder as it contains it's own build system,
and is still built into place.

mrb_config.rb should be edited, to use the correct toolchain for building mruby.

TODO: use a custom gembox, outside the mrbgems directory

