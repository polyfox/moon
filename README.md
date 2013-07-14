# Moon and Lunar Howl

Moon is the name of the engine (libmoon), running OpenGL, GLFW and mruby.

Lunar Howl is the name of the going-to-be-built game on top of the Moon engine.

Requires GLFW and OpenGL externally installed. Manually compiles and statically links
mruby. Required DevIL image library (il and maybe ilu too?)

NOTE: To define mruby C extensions, you need to include a `void moon_init_mrb_ext(mrb_state *mrb)`
function, that does the same form of initialization as an "Init_<name>" function would do. This
function needs to be declared (and just left empty) even if you don't use any mruby C code, or
the compiler will crash.

NOTE: Makefiles are messed up, to link to Gorilla, we need to link to OpenAL