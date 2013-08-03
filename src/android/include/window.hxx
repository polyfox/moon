#ifndef MOON_WINDOW_H
#define MOON_WINDOW_H

#include "moon.hxx"
#include <EGL/egl.h>

#include <android/log.h>
//#include <android/sensor.h>
#include "android_native_app_glue.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

void android_main(struct android_app* state);
void handle_cmd(struct android_app* app, int32_t cmd);

namespace Moon {
  class Window {
  public:
    Window(int width, int height, const char* name); // values not used, but desktop version needs it
    ~Window();

    int init_display();
    void terminate_display();

    void update();
    void draw_frame();
    bool should_close();

    int width();
    int height();
  private:
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t window_width;
    int32_t window_height;
    android_app* android;

  friend void ::handle_cmd(struct android_app* app, int32_t cmd);
  friend void ::android_main(struct android_app* state);
  };
};

#endif