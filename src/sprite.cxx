#include "sprite.hxx"

namespace Moon {
  Sprite::Sprite(std::string filename) {
    x = 0;
    y = 0;
    z = 0.0;
    opacity = 1.0;
    clip = false;

    texture = Texture::load(filename);

    //Vertex data
    VertexData2D vData[4];
    GLuint iData[4] = {0, 1, 3, 2}; // rendering indices

    //Create VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData2D), vData, GL_DYNAMIC_DRAW);

    //Create IBO
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_STATIC_DRAW);

    //Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // If the texture exists
    if(texture->id() != 0) {
      //Texture coordinates
      GLfloat texTop = 0.f;
      GLfloat texBottom = 1.f;
      GLfloat texLeft = 0.f;
      GLfloat texRight = 1.f;

      //Vertex coordinates
      GLfloat quadWidth = texture->width();
      GLfloat quadHeight = texture->height();

      //Handle clipping
      /*if(clip != NULL) {
        //Texture coordinates
        texLeft = clip->x / texture_width;
        texRight = (clip->x + clip->w) / texture_width;
        texTop = clip->y / texture_height;
        texBottom = (clip->y + clip->h) / texture_height;

        //Vertex coordinates
        quadWidth = clip->w;
        quadHeight = clip->h;
      }*/

      //Set vertex data
      VertexData2D vData[4];

      //Texture coordinates
      vData[0].u =  texLeft; vData[0].v =    texTop;
      vData[1].u = texRight; vData[1].v =    texTop;
      vData[2].u = texRight; vData[2].v = texBottom;
      vData[3].u =  texLeft; vData[3].v = texBottom;

      //Vertex positions
      vData[0].x =       0.f; vData[0].y =        0.f;
      vData[1].x = quadWidth; vData[1].y =        0.f;
      vData[2].x = quadWidth; vData[2].y = quadHeight;
      vData[3].x =       0.f; vData[3].y = quadHeight;

      //Update vertex buffer data
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(VertexData2D), vData);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    };
  };

  Sprite::~Sprite() {
    //Free VBO and IBO
    if(VBO != 0) {
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &IBO);
    }
  };

  void Sprite::render() {
    //if(clip) {
    //  texture->render(x, y, z, opacity, &clip_rect);
    //} else {
      texture->render(x, y, z, opacity, tone.get(), VBO, IBO);
    //}
  };
};
