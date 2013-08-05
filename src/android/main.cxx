#include "engine.hxx"
#include "android_native_app_glue.h"

// TODO: https://github.com/anura-engine/anura/blob/master/src/filesystem-android.cpp

/**
 * Process the next input event.
 */
int32_t handle_input(struct android_app* app, AInputEvent* event) {
  Moon::Engine* engine = (Moon::Engine*)app->userData;
  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
    //engine->touchX = AMotionEvent_getX(event, 0);
    //engine->touchY = AMotionEvent_getY(event, 0);
    //LOGI("x %d\ty %d\n",engine->touchX,engine->touchY);
    return 1;
  }
  return 0;
}

/**
 * Process the next main command.
 */
void handle_cmd(struct android_app* app, int32_t cmd) {
  Moon::Engine* engine = (Moon::Engine*)app->userData;
  switch (cmd) {
  case APP_CMD_SAVE_STATE:
    break;
  case APP_CMD_INIT_WINDOW:
    // The window is being shown, get it ready.
    //LOGI("app address %i", app);
    //LOGI("engine->window.android address %i", engine->window.android);

    /*HAXX: for some reason in APP_CMD_INIT_WINDOW
      engine->window.android is not the same anymore.
      I can't figure out why, but it probably has something
      to do with the process call inside the update loop:
        if (source != NULL) {
          source->process(android, source);
        }

      so to tempfix, let's just reassign engine->window.android. 
    */
    engine->window.android = app;

    if (app->window != NULL) {
      engine->window.init_display();
      engine->initialize();
    }
    break;
  case APP_CMD_TERM_WINDOW:
    // The window is being hidden or closed, clean it up.
    engine->window.terminate_display();
    break;
  case APP_CMD_LOST_FOCUS:
    break;
  }
}

/**
 * Main entry point, handles events
 */

void android_main(struct android_app* state) {
  Moon::Engine *engine = NULL;
  engine = new Moon::Engine();

  state->userData = &engine;
  state->onAppCmd = handle_cmd;
  state->onInputEvent = handle_input;
  engine->window.android = state;

  LOGI("state address: %i", state);
  LOGI("engine->window.android address: %i", engine->window.android);

  moon_main(engine);

  //engine->run();
  while(1) 
  {
    engine->window.update();

  }

  if(engine) delete(engine); 
}
