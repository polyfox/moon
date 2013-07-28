# Moon and Lunar Howl

Moon is the name of the engine (libmoon), running OpenGL, GLFW and mruby.

Lunar Howl is the name of the going-to-be-built game on top of the Moon engine.

Requires GLFW3 and DevIL (il and ilu) externally installed. Manually compiles and statically links
mruby and gorilla audio.

NOTE: To define mruby C extensions, you need to include a `void moon_init_mrb_ext(mrb_state *mrb)`
function, that does the same form of initialization as an "Init_<name>" function would do. This
function needs to be declared (and just left empty) even if you don't use any mruby C code, or
the compiler will crash.

## How to build

After cloning, you need to fetch the git submodules (vendor files):

```
git submodule update --init
```

Compilation is done via the CMake build system. Use cmake in the root folder to generate your
Makefiles or (VS/CodeBlocks) projects of choice for your platform, then simply compile it.

mrb_config.rb should be edited, to use the correct toolchain for building mruby.

TODO: use a custom gembox, outside the mrbgems directory

TODO: copy over the script files EVERY time make runs

TODO: properly cleanup after mruby too (including the downloaded mrbgems)

TODO: mruby-require loads dependent on the working dir. Make a require_relative method or
something.
