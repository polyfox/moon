#ifndef MOON_VERTBUFFER_H
#define MOON_VERTBUFFER_H

#include "moon.hxx"
#include "graphics.hxx"
#include <vector>
#include <algorithm>

namespace Moon {
  class VertexBuffer {
  public:
    VertexBuffer(GLenum usage); // STATIC_DRAW, DYNAMIC_DRAW...
    ~VertexBuffer();

    void render(GLint vertex_pos_attrib, GLint texcoord_attrib); // pass in IBO at the moment, later it's embedded
    void push_back(vertex v);
    void push_back(vertex *v, int vertex_count, GLuint i[], int index_count);
    void upload();
  private:
    GLuint vbo_id;
    GLuint ibo_id;

    GLenum usage;

    std::vector<vertex> vertices;
    std::vector<GLuint> indices;
  };
};

#endif