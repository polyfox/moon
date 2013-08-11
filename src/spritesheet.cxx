#include "spritesheet.hxx"

namespace Moon {
  Spritesheet::Spritesheet(std::string filename, int tile_width, int tile_height) 
  : VBO(GL_STATIC_DRAW)
  {
    texture = Texture::load(filename);

    this->tile_height = tile_height;
    this->tile_width = tile_width;
    total_sprites = 0;
    generate_buffers();
  };

  Spritesheet::~Spritesheet() {

  };

  bool Spritesheet::generate_buffers() {
    // If there is a texture loaded and clips to make vertex data from
    if(texture->id() != 0) {
      GLfloat tiles_per_row, tiles_per_column;

      tiles_per_row = texture->width() / tile_width;
      tiles_per_column = texture->height() / tile_height;

      total_sprites = tiles_per_row * tiles_per_column;

      for(int i = 0; i < total_sprites; ++i) {
        GLfloat ox = (float)(i % (int)tiles_per_row);
        GLfloat oy = (float)(i / (int)tiles_per_row);

        float s0 = (ox) / tiles_per_row;
        float s1 = (ox + 1.0) / tiles_per_row;
        float t0 = (oy) / tiles_per_column;
        float t1 = (oy + 1.0) / tiles_per_column;

        vertex vertices[4] = {
          { {0.f, 0.f},                {s0, t0} },
          { {tile_width, 0.f},         {s1, t0} },
          { {tile_width, tile_height}, {s1, t1} },
          { {0.f, tile_height},        {s0, t1} }
        };

        VBO.push_back_vertices(vertices, 4);
      }

      GLuint indices[4] = {0, 1, 3, 2};
      VBO.push_back_indices(indices, 4);

    } else {   //Error
      if(texture->id() == 0) {
        printf("No texture to render with!\n");
      }
      return false;
    }

    return true;
  };

  void Spritesheet::render(const int &x, const int &y, const float &z, const int &index) {
    Tone tone;
    texture->render_with_offset(x, y, z, 1.0, &tone, VBO, index*4);
  };

};
