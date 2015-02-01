#ifndef MOON_SPRITESHEET_H
#define MOON_SPRITESHEET_H

#include "moon/stdh.hxx"
#include "moon/texture.hxx"
#include "moon/shared_types.hxx"

namespace Moon {
  struct ss_render_options {
    float opacity;
    float ox;
    float oy;
    float angle;
    glm::vec4 color;
    glm::vec4 tone;
    glm::mat4 transform;
  };

  class Spritesheet {
  public:
    GLfloat tile_width;
    GLfloat tile_height;
    int total_sprites;

    Spritesheet();
    ~Spritesheet();

    void load_file(std::string filename, int tile_width, int tile_height);
    void load_texture(moon_texture texture, int tile_width, int tile_height);
    void render(const float &x, const float &y, const float &z, const int &index, const ss_render_options &render_op);
  private:
    VertexBuffer VBO;

    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<Shader>  m_shader;

    bool generate_buffers(); // use only in constructor
  };
};

#endif
