#include "moon.hxx"
#include "engine.hxx"

void init_hola(mrb_state *mrb);
void moon_init_mrb_ext(mrb_state *mrb)
{
  init_hola(mrb);
}

void moon_load_user_scripts(Moon::Engine *engine)
{
  engine->load_mrb_file("./scripts/load.rb");
}

void moon_config(Moon::Engine *engine)
{
  /* Affects default shader path */
  //engine->set_resource_path("media");
}

void moon_main(Moon::Engine *engine)
{
  //
}