#include "moon/engine.hxx"

void
moon_load_user_scripts(Moon::Engine *engine)
{
  engine->load_mrb_file("./scripts/load.rb");
}

void
moon_config(Moon::Engine *engine)
{
  /* Affects default shader path */
  //engine->set_resource_path("media");
}

int
moon_start(Moon::Engine *engine)
{
  //
  return 0;
}

void
moon_step(mrb_state *mrb, double delta)
{
// Get the ruby object containing the state manager
  mrb_value state_class = mrb_obj_value(mrb_class_get(mrb, "State"));
  mrb_funcall(mrb, state_class, "step", 1, mrb_float_value(mrb, delta));
}

int
main(int argc, char **argv)
{
  Moon::Engine *engine = NULL;
  engine = new Moon::Engine();

  moon_start(engine);

  engine->run();
  if(engine) delete(engine);

  return 0;
}
