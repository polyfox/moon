#include "input.hxx"

namespace Moon {

  GLFWwindow* Input::window = NULL;
  mrb_state* Input::mrb = NULL;

  void Input::initialize(GLFWwindow* var_window, mrb_state* var_mrb) {
    window = var_window;
    mrb = var_mrb;
    glfwSetKeyCallback(window, Input::update_key);
    glfwSetMouseButtonCallback(window, Input::update_button);
    glfwSetCharCallback(window, Input::on_type);
  }

  /* Callbacks */

  static mrb_value
  glfw_state_to_mrb_symbol(mrb_state *mrb, int state) {
    switch (state) {
    case GLFW_PRESS:
      return mrb_symbol_value(mrb_intern_cstr(mrb, "press"));
    case GLFW_RELEASE:
      return mrb_symbol_value(mrb_intern_cstr(mrb, "release"));
    case GLFW_REPEAT:
      return mrb_symbol_value(mrb_intern_cstr(mrb, "repeat"));
    }
    return mrb_nil_value();
  }

  void Input::on_type(GLFWwindow* window, unsigned int utf8_char) {
    mrb_value klass = mrb_obj_value(mrb_module_get_under(mrb, moon_module, "Input"));
    std::wstring ws;
    ws += utf8_char;
    /* TODO: convert int directly to a UTF8 mruby string */
    mrb_funcall(mrb, klass, "on_type", 1,
                mrb_funcall(mrb, mrb_fixnum_value(utf8_char), "chr", 0));
  }

  void Input::update_key(GLFWwindow* window, int key_id, int scancode, int action, int mods) {
    mrb_value klass = mrb_obj_value(mrb_module_get_under(mrb, moon_module, "Input"));
    mrb_funcall(mrb, klass, "on_key", 4,
      mrb_fixnum_value(key_id),
      mrb_fixnum_value(scancode),
      glfw_state_to_mrb_symbol(mrb, action),
      mrb_fixnum_value(mods));
  }

  void Input::update_button(GLFWwindow* window, int button_id, int action, int mods) {
    mrb_value klass = mrb_obj_value(mrb_module_get_under(mrb, moon_module, "Input"));
    mrb_funcall(mrb, klass, "on_button", 3,
      mrb_fixnum_value(button_id),
      glfw_state_to_mrb_symbol(mrb, action),
      mrb_fixnum_value(mods));
  }

  /* Mouse functions */

  int Input::Mouse::x() {
    double x;
    glfwGetCursorPos(window, &x, NULL);
    return floor(x);
  }

  int Input::Mouse::y() {
    double y;
    glfwGetCursorPos(window, NULL, &y);
    return floor(y);
  }

  std::array<int, 2> Input::Mouse::pos() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {(int)floor(x), (int)floor(y)};
  }
}
