#include "moon.hxx"
#include "engine.hxx"

void init_hola(mrb_state *mrb);
void moon_init_mrb_ext(mrb_state *mrb)
{
  init_hola(mrb);
}

int main(void)
{
  Moon::Engine *engine = new Moon::Engine();
  engine->run();
  delete(engine);
  return 0;
}