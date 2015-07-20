#ifndef MOON_VERTBUFFER_H
#define MOON_VERTBUFFER_H

#include <vector>
#include <functional>
#include <algorithm>
#include "moon/intern.h"
#include "moon/gl.h"
#include "moon/graphics.hxx"
#include "moon/vector3.hxx"

namespace Moon {
  class VertexBuffer {
  public:
    VertexBuffer(GLenum usage); // STATIC_DRAW, DYNAMIC_DRAW...
    ~VertexBuffer();
    void Render(GLenum mode);
    void Render(GLenum mode, const int offset); // adds offset to the indices used (draws vertex at vbo[indice + offset])
    void PushBack(Vertex v);
    void PushBackVertices(Vertex *v, int vertex_count);
    void PushBackIndices(GLuint i[], int index_count);
    void PushBack(Vertex *v, int vertex_count, GLuint i[], int index_count); // indices need to be relative to the vertices pushed in
    Vertex GetVertex(const int index);
    GLuint GetIndex(const int index);
    GLuint GetVertexCount();
    GLuint GetIndexCount();
    void Clear();
    void Upload();
  private:
    GLuint m_vao_id;
    GLuint m_vbo_id;
    GLuint m_ibo_id;
    GLenum m_usage;
    bool m_dirty; // do we need to reupload?
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    void Setup(); // set up the VAO
  };
};

#endif
