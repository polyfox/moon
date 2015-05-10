#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "moon/engine.hxx"
#include "moon/shader.hxx"
#include "moon/shader_loader.hxx"

namespace Moon {
  Shader *ShaderLoader::s_quad_shader = NULL;
  Shader *ShaderLoader::s_text_shader = NULL;

  bool ShaderLoader::IsLegacy()
  {
    const GLubyte *shader_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    assert(shader_version);
    return strcmp("1.50", (char*)shader_version) >= 0;
  }

  Shader* ShaderLoader::Load(const char *vert_basename, const char *frag_basename)
  {
    assert(vert_basename);
    assert(frag_basename);
    printf("ShaderLoader::load(%s, %s)\n", vert_basename, frag_basename);
    return new Shader(vert_basename, frag_basename);
  };

  Shader* ShaderLoader::GetQuadShader() {
    if (!s_quad_shader) {
      if (IsLegacy()) {
        s_quad_shader = Load("resources/shaders/120/quad.vert", "resources/shaders/120/quad.frag");
      } else {
        s_quad_shader = Load("resources/shaders/330/quad.vert", "resources/shaders/330/quad.frag");
      }
    }
    return s_quad_shader;
  }

  Shader* ShaderLoader::GetTextShader() {
    if (!s_text_shader) {
      if (IsLegacy()) {
        s_text_shader = Load("resources/shaders/120/text.vert", "resources/shaders/120/text.frag");
      } else {
        s_text_shader = Load("resources/shaders/330/text.vert", "resources/shaders/330/text.frag");
      }
    }
    return s_text_shader;
  }
}
