#ifndef MOON_SPRITE_H
#define MOON_SPRITE_H

#include "moon/intern.h"
#include "moon/glm.h"
#include "moon/graphics.hxx"
#include "moon/rect.hxx"
#include "moon/texture.hxx"
#include "moon/shader.hxx"
#include "moon/vector4.hxx"
#include "moon/vertex_buffer.hxx"

namespace Moon {
  class Sprite {
  public:
    bool use_clip;
    float opacity;
    float angle;
    Vector2 origin;
    Vector4 color;
    Vector4 tone;
    Shader *shader;

    Sprite();
    ~Sprite();
    void LoadTexture(Texture *texture);
    Texture* GetTexture();
    void SetTexture(Texture *tex);
    IntRect GetClipRect();
    void SetClipRect(IntRect clip);
    void Render(const float x, const float y, const float z);
  private:
    IntRect m_clip_rect;
    Texture *m_texture;
    VertexBuffer m_vbo;
    bool GenerateBuffers();
  };
};

#endif
