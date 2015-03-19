#ifndef MOON_SHADER_LOADER_H
#define MOON_SHADER_LOADER_H

#include <stdbool.h>
#include "moon/shader.hxx"

namespace Moon {
  class ShaderLoader {
  public:
    static bool IsLegacy();
    static Shader* Load(const char *vert_basename, const char *frag_basename);
    static Shader* GetQuadShader();
    static Shader* GetTextShader();
  private:
    static Shader *s_quad_shader;
    static Shader *s_text_shader;
  };
}

#endif
