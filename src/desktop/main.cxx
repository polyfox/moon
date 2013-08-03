#include "engine.hxx"

int main(void)
{
  Moon::Engine *engine = NULL;
  engine = new Moon::Engine();

  moon_main(engine);

  engine->run();
  if(engine) delete(engine); 

  return 0;
}