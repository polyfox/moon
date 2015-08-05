#include "moon/vertex_buffer.hxx"
#include "moon/gl.h"
#include "moon/glm.h"

namespace Moon {
  VertexBuffer::VertexBuffer(GLenum usage) {
    m_usage = usage;
    glGenVertexArrays(1, &m_vao_id);
    glGenBuffers(1, &m_vbo_id);
    glGenBuffers(1, &m_ibo_id);
    m_dirty = false;
    Setup();
  }

  VertexBuffer::~VertexBuffer() {
    glDeleteVertexArrays(1, &m_vao_id);
    glDeleteBuffers(1, &m_vbo_id);
    glDeleteBuffers(1, &m_ibo_id);
  }

  void VertexBuffer::Setup() {
    glBindVertexArray(m_vao_id);

    //Enable vertex and texture coordinate arrays
    glEnableVertexAttribArray(0); // location=0 --> vertex position
    glEnableVertexAttribArray(1); // location=1 --> texture coordinates
    glEnableVertexAttribArray(2); // location=2 --> color

    // Bind vertex buffer and index buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);

    glVertexAttribPointer(
      0,             // attribute
      2,             // number of elements per vertex, here (x,y)
      GL_FLOAT,      // the type of each element
      GL_FALSE,      // take our values as-is
      sizeof(Vertex),                  // stride
      (GLvoid*)offsetof(Vertex, pos)   // offset of first element
    );

    glVertexAttribPointer(
      1,             // attribute
      2,             // number of elements per vertex, here (x,y)
      GL_FLOAT,      // the type of each element
      GL_FALSE,      // take our values as-is
      sizeof(Vertex),                  // stride
      (GLvoid*)offsetof(Vertex, tex_coord)     // offset of first element
    );

    glVertexAttribPointer(
      2,             // attribute
      4,             // number of elements per vertex, here (x,y)
      GL_FLOAT,      // the type of each element
      GL_FALSE,      // take our values as-is
      sizeof(Vertex),                  // stride
      (GLvoid*)offsetof(Vertex, color)   // offset of first element
    );

    glBindVertexArray(0);
  };

  void VertexBuffer::PushBack(Vertex v) {
    m_vertices.push_back(v);
    m_indices.push_back(m_indices.size());
    m_dirty = true;
  }

  void VertexBuffer::PushBackVertices(Vertex *v, int vertex_count) {
    m_vertices.reserve(vertex_count);
    std::copy(v, v+vertex_count, std::back_inserter(m_vertices));
    m_dirty = true;
  }

  void VertexBuffer::PushBackIndices(GLuint i[], int index_count) {
    m_indices.reserve(index_count);
    std::copy(i, i+index_count, std::back_inserter(m_indices));
    m_dirty = true;
  }

  void VertexBuffer::PushBack(Vertex *v, int vertex_count, GLuint i[], int index_count) {
    int size = m_vertices.size();
    PushBackVertices(v, vertex_count);
    PushBackIndices(i, index_count);
    // update the indices we've just copied over, since the old values
    // were something like (0,1,2,3) - relative to the vertex that was
    // pushed, and we want them to be set on a global range.
    std::transform(m_indices.end()-index_count, m_indices.end(), m_indices.end()-index_count,
          std::bind2nd(std::plus<GLuint>(), size));
    m_dirty = true;
  }

  GLuint VertexBuffer::GetVertexCount() {
    return m_vertices.size();
  }

  GLuint VertexBuffer::GetIndexCount() {
    return m_indices.size();
  }

  Vertex VertexBuffer::GetVertex(const int index) {
    // we should probably handle errors more graciously...
    assert(index < (int)m_vertices.size());
    assert(index >= 0);
    return m_vertices[index];
  }

  GLuint VertexBuffer::GetIndex(const int index) {
    // we should probably handle errors more graciously...
    assert(index < (int)m_indices.size());
    assert(index >= 0);
    return m_indices[index];
  }

  void VertexBuffer::Clear() {
    m_vertices.clear();
    m_indices.clear();
    Upload();
  }

  // upload the buffer to the GPU
  void VertexBuffer::Upload() {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices.front(), m_usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices.front(), m_usage);
    m_dirty = false;
  }

  void VertexBuffer::Render(GLenum mode) {
    /*if (m_dirty) Upload(); // update the VBO and IBO if dirty
    glBindVertexArray(m_vao_id);
    glDrawElements(mode, m_indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);*/
    Render(mode, 0);
  }

  void VertexBuffer::Render(GLenum mode, const int offset) {
    if (m_dirty) Upload(); // update the VBO and IBO if dirty
    glBindVertexArray(m_vao_id);
    glDrawElementsBaseVertex(mode, m_indices.size(), GL_UNSIGNED_INT, NULL, offset);
    glBindVertexArray(0);
  }
}
