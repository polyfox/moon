#ifndef MOON_VERTBUFFER_H
#define MOON_VERTBUFFER_H

#include "moon.hxx"
#include "graphics.hxx"
#include <vector>

namespace Moon {
  class VertexBuffer {
  public:
    VertexBuffer(GLenum usage); // STATIC_DRAW, DYNAMIC_DRAW...
    ~VertexBuffer();

    void render(GLint ibo_id, GLint vertex_pos_attrib, GLint texcoord_attrib); // pass in IBO at the moment, later it's embedded
    void push_back(vertex v);
    void push_back(vertex *v, int count);
    void upload();
  private:
    GLuint vbo_id;
    GLenum usage;
    std::vector<vertex> vertices;
  };
};

#endif