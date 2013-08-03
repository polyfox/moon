#include "window.hxx"

namespace Moon {
  /**
   * Initialize an EGL context for the current display.
   * TODO tidy this up, currently it's mostly Google example code
   */
  Window::Window(int w, int h, const char* name) { // values not used, but desktop version needs it
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    window_width = 0;
    window_height = 0;
  };

  int Window::init_display() {
    // Setup OpenGL ES 2
    // http://stackoverflow.com/questions/11478957/how-do-i-create-an-opengl-es-2-context-in-a-native-activity

    const EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //important
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };

    EGLint attribList[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(android->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, android->window, NULL);

    context = eglCreateContext(display, config, NULL, attribList);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
      LOGW("Unable to eglMakeCurrent");
      return -1;
    }

    // Grab the width and height of the surface
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    this->display = display;
    this->context = context;
    this->surface = surface;
    this->window_width = w;
    this->window_height = h;

    return 0;
  };

  Window::~Window() {
    terminate_display();
  };

  void Window::terminate_display() {
    if (display != EGL_NO_DISPLAY) {
      eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
      if (context != EGL_NO_CONTEXT) {
        eglDestroyContext(display, context);
      }
      if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(display, surface);
      }
      eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    LOGI("EGL display terminated");
  }

  void Window::update() {
    int ident;
    int events;
    struct android_poll_source* source;

    while ((ident=ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0) {

      // Process this event.
      if (source != NULL) {
        source->process(android, source);
      }

      // Check if we are exiting.
      if (should_close()) {
        terminate_display();
        return;
      }
    }

    // Draw the current frame
    draw_frame();

    eglSwapBuffers(display, surface);
  };

  bool Window::should_close() {
    return android->destroyRequested;
  };

  /**
   * Just the current frame in the display.
   */
  void Window::draw_frame() {
    // No display.
    if (display == NULL) {
      return;
    }
  };

  int Window::width() {
    return window_width;
  }

  int Window::height() {
    return window_height;
  }
}