#ifndef MOON_SPRITESHEET_H
#define MOON_SPRITESHEET_H

#include "moon/intern.h"
#include "moon/gl.h"
#include "moon/glm.h"
#include "moon/texture.hxx"
#include "moon/shader.hxx"
#include "moon/vector4.hxx"
#include "moon/transform.hxx"

namespace Moon {
  class Spritesheet {
  public:
    struct RenderState {
      GLfloat opacity;
      GLfloat angle;
      Vector2 origin;
      Vector4 color;
      Vector4 tone;
      Transform transform;

      RenderState() :
        opacity(1.0),
        angle(0.0),
        origin(0.0, 0.0),
        color(1.0, 1.0, 1.0, 1.0),
        tone(0.0, 0.0, 0.0, 1.0) {};
    };

    GLfloat tile_width;
    GLfloat tile_height;
    int total_sprites;

    Spritesheet();
    ~Spritesheet();
    void LoadTexture(Texture *texture, int tile_width, int tile_height);
    void Render(const float x, const float y, const float z, const int index, const struct RenderState &render_op);
  private:
    VertexBuffer m_vbo;
    Texture *m_texture;
    Shader  *m_shader;
    bool GenerateBuffers(); // use only in constructor
  };
};

#endif
