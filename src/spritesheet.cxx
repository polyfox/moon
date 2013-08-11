#include "spritesheet.hxx"

namespace Moon {
  Spritesheet::Spritesheet(std::string filename, int tile_width, int tile_height) {
    texture = Texture::load(filename);

    this->tile_height = tile_height;
    this->tile_width = tile_width;
    total_sprites = 0;
    generate_buffers();
  };

  Spritesheet::~Spritesheet() {
    glDeleteBuffers(1, &VBO);

    //Clear index buffers
    if( index_buffers != 0) {
      glDeleteBuffers(total_sprites, index_buffers);
      delete[] index_buffers;
    }
  };

  bool Spritesheet::generate_buffers() {
    // If there is a texture loaded and clips to make vertex data from
    if(texture->id() != 0) {
      GLfloat tiles_per_row, tiles_per_column;

      tiles_per_row = texture->width() / tile_width;
      tiles_per_column = texture->height() / tile_height;

      total_sprites = tiles_per_row * tiles_per_column;

      // Allocate vertex buffer data
      vertex* vertices = new vertex[total_sprites * 4];
      index_buffers = new GLuint[total_sprites];

      // Allocate vertex data buffer name
      glGenBuffers(1, &VBO);

      // Allocate index buffers names
      glGenBuffers(total_sprites, index_buffers);

      // Go through clips
      GLuint indices[4] = {0, 0, 0, 0};

      for(int i = 0; i < total_sprites; ++i) {
        //Initialize indices
        indices[0] = i * 4 + 0;
        indices[1] = i * 4 + 1;
        indices[2] = i * 4 + 2;
        indices[3] = i * 4 + 3;

        GLfloat ox = (float)(i % (int)tiles_per_row);
        GLfloat oy = (float)(i / (int)tiles_per_row);

        // Top left
        vertices[ indices[0] ].pos.x = 0;
        vertices[ indices[0] ].pos.y = 0;

        vertices[ indices[0] ].tex_coord.u =  (ox) / tiles_per_row;
        vertices[ indices[0] ].tex_coord.v =  (oy) / tiles_per_column;

        // Top right
        vertices[ indices[1] ].pos.x = tile_width;
        vertices[ indices[1] ].pos.y = 0;

        vertices[ indices[1] ].tex_coord.u =  (ox + 1.0) / tiles_per_row;
        vertices[ indices[1] ].tex_coord.v =  (oy) / tiles_per_column;

        // Bottom right
        vertices[ indices[3] ].pos.x = tile_width;
        vertices[ indices[3] ].pos.y = tile_height;

        vertices[ indices[3] ].tex_coord.u =  (ox + 1.0) / tiles_per_row;
        vertices[ indices[3] ].tex_coord.v =  (oy + 1.0) / tiles_per_column;

        // Bottom left
        vertices[ indices[2] ].pos.x = 0;
        vertices[ indices[2] ].pos.y = tile_height;

        vertices[ indices[2] ].tex_coord.u =  (ox) / tiles_per_row;
        vertices[ indices[2] ].tex_coord.v =  (oy + 1.0) / tiles_per_column;

        //Bind sprite index buffer data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffers[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indices, GL_STATIC_DRAW);
      }

      //Bind vertex data
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, total_sprites * 4 * sizeof(vertex), vertices, GL_STATIC_DRAW);

      //Deallocate vertex data
      delete[] vertices;
    } else {   //Error
      if(texture->id() == 0) {
        printf("No texture to render with!\n");
      }
      return false;
    }

    return true;
  };

  void Spritesheet::render(const int &x, const int &y, const float &z, const int &index) {
    //Sprite sheet data exists
    if(VBO != 0) {
      Tone tone;
      texture->render(x, y, z, 1.0, &tone, VBO, index_buffers[index]);
    };
  };

};
