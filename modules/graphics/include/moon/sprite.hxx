#ifndef MOON_SPRITE_H
#define MOON_SPRITE_H

#include "moon.hxx"
#include "texture.hxx"
#include "vertex_buffer.hxx"
#include "graphics.hxx"
#include "shared_types.hxx"
#include <glm/glm.hpp>

namespace Moon {
  class Sprite {
  public:
    Sprite();
    ~Sprite();

    float opacity;
    float angle;
    int ox;
    int oy;
    moon_vec4 color;
    moon_vec4 tone;

    void load_file(std::string filename);
    void load_texture(moon_texture texture);

    void render(const float &x, const float &y, const float &z);
    std::shared_ptr<Texture> getTexture();
    void setTexture(moon_texture tex);

    std::shared_ptr<Rect> getClip();
    void setClip(moon_rect clip);
  private:
    bool generate_buffers();

    moon_rect m_clip_rect; // default constructor nullptr
    moon_shader m_shader;
    moon_texture m_texture;
    VertexBuffer m_VBO;
  };
};

#endif
