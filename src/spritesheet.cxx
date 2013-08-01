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
    if(mVertexDataBuffer != NULL) {
      glDeleteBuffers(1, &mVertexDataBuffer );
      mVertexDataBuffer = NULL;
    }

    //Clear index buffers
    if( mIndexBuffers != NULL ) {
      glDeleteBuffers(totalSprites, mIndexBuffers);
      delete[] mIndexBuffers;
      mIndexBuffers = NULL;
    }
  };

  bool Spritesheet::generate_buffers() {
    // If there is a texture loaded and clips to make vertex data from
    if(texture->id() != 0) {
      GLfloat tW = texture->width();
      GLfloat tH = texture->height();
      totalSprites = (tW/tileWidth)*(tH/tileHeight);

      // Allocate vertex buffer data
      VertexData2D* vertexData = new VertexData2D[ totalSprites * 4 ];
      mIndexBuffers = new GLuint[ totalSprites ];

      // Allocate vertex data buffer name
      glGenBuffers(1, &mVertexDataBuffer);

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
        vertexData[ spriteIndices[0] ].x = 0;
        vertexData[ spriteIndices[0] ].y = 0;

        vertexData[ spriteIndices[0] ].u =  (ox) / tiles_per_row;
        vertexData[ spriteIndices[0] ].v =  (oy) / tiles_per_column;

        // Top right
        vertexData[ spriteIndices[1] ].x = tileWidth;
        vertexData[ spriteIndices[1] ].y = 0;

        vertexData[ spriteIndices[1] ].u =  (ox + 1.0) / tiles_per_row;
        vertexData[ spriteIndices[1] ].v =  (oy) / tiles_per_column;

        // Bottom right
        vertexData[ spriteIndices[3] ].x = tileWidth;
        vertexData[ spriteIndices[3] ].y = tileHeight;

        vertexData[ spriteIndices[3] ].u =  (ox + 1.0) / tiles_per_row;
        vertexData[ spriteIndices[3] ].v =  (oy + 1.0) / tiles_per_column;

        // Bottom left
        vertexData[ spriteIndices[2] ].x = 0;
        vertexData[ spriteIndices[2] ].y = tileHeight;

        vertexData[ spriteIndices[2] ].u =  (ox) / tiles_per_row;
        vertexData[ spriteIndices[2] ].v =  (oy + 1.0) / tiles_per_column;

        //Bind sprite index buffer data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[ i ]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), spriteIndices, GL_STATIC_DRAW);
      }

      //Bind vertex data
      glBindBuffer(GL_ARRAY_BUFFER, mVertexDataBuffer);
      glBufferData(GL_ARRAY_BUFFER, totalSprites * 4 * sizeof(VertexData2D), vertexData, GL_STATIC_DRAW);

      //Deallocate vertex data
      delete[] vertexData;
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
    if(mVertexDataBuffer != NULL) {
      texture->render(x, y, z, 1.0, mVertexDataBuffer, mIndexBuffers[index]);
    };

  };
};
