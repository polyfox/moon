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

  void VertexBuffer::push_back(vertex *v, int vertex_count, GLuint i[], int index_count) {
    int size = vertices.size();

    vertices.reserve(vertex_count);
    std::copy(v, v+vertex_count, std::back_inserter(vertices));

    indices.reserve(index_count);
    std::copy(i, i+index_count, std::back_inserter(indices));

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

  void VertexBuffer::render(GLenum mode, GLint vertex_pos, GLint texcoord) {
    if(dirty) upload(); // update the VBO and IBO if dirty

    //Enable vertex and texture coordinate arrays
    glEnableVertexAttribArray(vertex_pos);
    glEnableVertexAttribArray(texcoord);
    //glEnableVertexAttribArray(color);

      //Bind vertex buffer
      glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

      glVertexAttribPointer(
        vertex_pos,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, pos)   // offset of first element
      );

      glVertexAttribPointer(
        texcoord,    // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, tex_coord)     // offset of first element
      );

      /*glVertexAttribPointer(
        color,              // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        sizeof(vertex),                  // stride
        (GLvoid*)offsetof(vertex, color)   // offset of first element
      );*/

      //Draw quad using vertex data and index data
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
      glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, NULL);

    //Disable vertex and texture coordinate arrays
    glDisableVertexAttribArray(vertex_pos);
    glDisableVertexAttribArray(texcoord);
    // glDisableVertexAttribArray(color);
  }
}