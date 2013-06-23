#include "engine.hxx"

int main(void)
{
  Moon::Engine *engine = new Moon::Engine();
  engine->run();
  delete(engine);
  return 0;
}