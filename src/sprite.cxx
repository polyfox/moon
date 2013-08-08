#include "sprite.hxx"

namespace Moon {
  Sprite::Sprite(std::string filename) {
    x = 0;
    y = 0;
    z = 0.0;
    opacity = 1.0;
    tone = std::make_shared<Tone>(1.0, 1.0, 1.0);
    clip = false;

    texture = Texture::load(filename);

    GLuint indices[4] = {0, 1, 3, 2}; // rendering indices

    glGenBuffers(1, &VBO);

    //Create IBO
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // If the texture exists
    if(texture->id() != 0) {
      //Texture coordinates
      GLfloat s0 = 0.f;
      GLfloat s1 = 1.f;
      GLfloat t0 = 0.f;
      GLfloat t1 = 1.f;

      //Vertex coordinates
      GLfloat quadWidth = texture->width();
      GLfloat quadHeight = texture->height();

      //Handle clipping
      /*if(clip != NULL) {
        //Texture coordinates
        s0 = clip->x / texture_width;
        s1 = (clip->x + clip->w) / texture_width;
        t0 = clip->y / texture_height;
        t1 = (clip->y + clip->h) / texture_height;

        //Vertex coordinates
        quadWidth = clip->w;
        quadHeight = clip->h;
      }*/

      vertex vertices[4] = {
        { {0.f, 0.f},              {s0, t0} },
        { {quadWidth, 0.f},        {s1, t0} },
        { {quadWidth, quadHeight}, {s1, t1} },
        { {0.f, quadHeight},       {s0, t1} }
      };

      //Update vertex buffer data
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), vertices, GL_DYNAMIC_DRAW);
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
