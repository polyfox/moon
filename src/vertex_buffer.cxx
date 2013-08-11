#include "vertex_buffer.hxx"

namespace Moon {
  VertexBuffer::VertexBuffer(GLenum usage) {
    this->usage = usage;
    glGenBuffers(1, &vbo_id);
    glGenBuffers(1, &ibo_id);
    dirty = false;
  }

  VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &vbo_id);
    glDeleteBuffers(1, &ibo_id);
  }

  void VertexBuffer::push_back(vertex v) {
    vertices.push_back(v);
    indices.push_back(indices.size());
    dirty = true;
  }

  void VertexBuffer::push_back_vertices(vertex *v, int vertex_count) {
    vertices.reserve(vertex_count);
    std::copy(v, v+vertex_count, std::back_inserter(vertices));
    dirty = true;
  }

  void VertexBuffer::push_back_indices(GLuint i[], int index_count) {
    indices.reserve(index_count);
    std::copy(i, i+index_count, std::back_inserter(indices));
    dirty = true;
  }

  void VertexBuffer::push_back(vertex *v, int vertex_count, GLuint i[], int index_count) {
    int size = vertices.size();

    push_back_vertices(v, vertex_count);
    push_back_indices(i, index_count);

    // update the indices we've just copied over, since the old values
    // were something like (0,1,2,3) - relative to the vertex that was
    // pushed, and we want them to be set on a global range.
    std::transform(indices.end()-index_count, indices.end(), indices.end()-index_count,
          std::bind2nd(std::plus<GLuint>(), size));

    dirty = true;
  }

  void VertexBuffer::clear() {
    vertices.clear();
    indices.clear();
    upload();
  }

  // upload the buffer to the GPU
  void VertexBuffer::upload() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices.front(), usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), usage);
    dirty = false;
  }

  void VertexBuffer::render(GLenum mode) {
    if(dirty) upload(); // update the VBO and IBO if dirty

    //Enable vertex and texture coordinate arrays
    glEnableVertexAttribArray(0); // location=0 --> vertex position
    glEnableVertexAttribArray(1); // location=1 --> texture coordinates
    glEnableVertexAttribArray(2); // location=2 --> color

      //Bind vertex buffer
      glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

      glVertexAttribPointer(
        0,             // attribute
        2,             // number of elements per vertex, here (x,y)
        GL_FLOAT,      // the type of each element
        GL_FALSE,      // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, pos)   // offset of first element
      );

      glVertexAttribPointer(
        1,             // attribute
        2,             // number of elements per vertex, here (x,y)
        GL_FLOAT,      // the type of each element
        GL_FALSE,      // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, tex_coord)     // offset of first element
      );

      glVertexAttribPointer(
        2,             // attribute
        4,             // number of elements per vertex, here (x,y)
        GL_FLOAT,      // the type of each element
        GL_FALSE,      // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, color)   // offset of first element
      );

      //Draw quad using vertex data and index data
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
      glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, NULL);

    //Disable vertex and texture coordinate arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }

  void VertexBuffer::render_with_offset(GLenum mode, const int &offset) {
    if(dirty) upload(); // update the VBO and IBO if dirty

    //Enable vertex and texture coordinate arrays
    glEnableVertexAttribArray(0); // location=0 --> vertex position
    glEnableVertexAttribArray(1); // location=1 --> texture coordinates
    glEnableVertexAttribArray(2); // location=2 --> color

      //Bind vertex buffer
      glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

      glVertexAttribPointer(
        0,             // attribute
        2,             // number of elements per vertex, here (x,y)
        GL_FLOAT,      // the type of each element
        GL_FALSE,      // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, pos)   // offset of first element
      );

      glVertexAttribPointer(
        1,             // attribute
        2,             // number of elements per vertex, here (x,y)
        GL_FLOAT,      // the type of each element
        GL_FALSE,      // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, tex_coord)     // offset of first element
      );

      glVertexAttribPointer(
        2,             // attribute
        4,             // number of elements per vertex, here (x,y)
        GL_FLOAT,      // the type of each element
        GL_FALSE,      // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, color)   // offset of first element
      );

      //Draw quad using vertex data and index data
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
      glDrawElementsBaseVertex(mode, indices.size(), GL_UNSIGNED_INT, NULL, offset);

    //Disable vertex and texture coordinate arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
}