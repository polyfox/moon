#include "spritesheet.hxx"

namespace Moon {
  Spritesheet::Spritesheet(std::string filename, int tile_width, int tile_height) {
    texture = Texture::load(filename);

    tileHeight = tile_height;
    tileWidth = tile_width;
    totalSprites = 0;
    generate_buffers();
  };

  Spritesheet::~Spritesheet() {
    //Clear vertex buffer
    if(VBO != 0) {
      glDeleteBuffers(1, &VBO);
    }

    //Clear index buffers
    if( mIndexBuffers != 0) {
      glDeleteBuffers(totalSprites, mIndexBuffers);
      delete[] mIndexBuffers;
    }
  };

  bool Spritesheet::generate_buffers() {
    // If there is a texture loaded and clips to make vertex data from
    if(texture->id() != 0) {
      GLfloat tW = texture->width();
      GLfloat tH = texture->height();
      totalSprites = (tW/tileWidth)*(tH/tileHeight);

      // Allocate vertex buffer data
      vertex* vertices = new vertex[ totalSprites * 4 ];
      mIndexBuffers = new GLuint[ totalSprites ];

      // Allocate vertex data buffer name
      glGenBuffers(1, &VBO);

      // Allocate index buffers names
      glGenBuffers(totalSprites, mIndexBuffers);

      // Go through clips
      GLuint spriteIndices[4] = { 0, 0, 0, 0 };

      for(int i = 0; i < totalSprites; ++i) {
        //Initialize indices
        spriteIndices[0] = i * 4 + 0;
        spriteIndices[1] = i * 4 + 1;
        spriteIndices[2] = i * 4 + 2;
        spriteIndices[3] = i * 4 + 3;

        GLfloat ox;
        GLfloat oy;
        GLfloat tiles_per_row;
        GLfloat tiles_per_column;

        tiles_per_row = tW/tileWidth;
        tiles_per_column = tH/tileHeight;
        ox = (float)(i % (int)tiles_per_row);
        oy = (float)(i / (int)tiles_per_row);

        // Top left
        vertices[ spriteIndices[0] ].pos.x = 0;
        vertices[ spriteIndices[0] ].pos.y = 0;

        vertices[ spriteIndices[0] ].tex_coord.u =  (ox) / tiles_per_row;
        vertices[ spriteIndices[0] ].tex_coord.v =  (oy) / tiles_per_column;

        // Top right
        vertices[ spriteIndices[1] ].pos.x = tileWidth;
        vertices[ spriteIndices[1] ].pos.y = 0;

        vertices[ spriteIndices[1] ].tex_coord.u =  (ox + 1.0) / tiles_per_row;
        vertices[ spriteIndices[1] ].tex_coord.v =  (oy) / tiles_per_column;

        // Bottom right
        vertices[ spriteIndices[3] ].pos.x = tileWidth;
        vertices[ spriteIndices[3] ].pos.y = tileHeight;

        vertices[ spriteIndices[3] ].tex_coord.u =  (ox + 1.0) / tiles_per_row;
        vertices[ spriteIndices[3] ].tex_coord.v =  (oy + 1.0) / tiles_per_column;

        // Bottom left
        vertices[ spriteIndices[2] ].pos.x = 0;
        vertices[ spriteIndices[2] ].pos.y = tileHeight;

        vertices[ spriteIndices[2] ].tex_coord.u =  (ox) / tiles_per_row;
        vertices[ spriteIndices[2] ].tex_coord.v =  (oy + 1.0) / tiles_per_column;

        //Bind sprite index buffer data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[ i ]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), spriteIndices, GL_STATIC_DRAW);
      }

      //Bind vertex data
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, totalSprites * 4 * sizeof(vertex), vertices, GL_STATIC_DRAW);

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
      texture->render(x, y, z, 1.0, &tone, VBO, mIndexBuffers[index]);
    };

  };
};
