#include "moon.hxx"
#include "engine.hxx"

void init_hola(mrb_state *mrb);
void moon_init_mrb_ext(mrb_state *mrb)
{
  init_hola(mrb);
}

int moon_main(Moon::Engine *engine)
{
  app_dummy();
  return 0;
}