#include "vertex_buffer.hxx"

namespace Moon {
  VertexBuffer::VertexBuffer(GLenum usage) {
    this->usage = usage;
    glGenBuffers(1, &vbo_id);
  }

  VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &vbo_id);
  }

  void VertexBuffer::push_back(vertex v) {
    vertices.push_back(v);
  }

  void VertexBuffer::push_back(vertex *v, int count) {
    vertices.reserve(count);
    std::copy(v, v+count, std::back_inserter(vertices));
  }

  // upload the buffer to the GPU
  void VertexBuffer::upload() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices.front(), usage);
  }

  void VertexBuffer::render(GLint ibo_id, GLint vertex_pos, GLint texcoord) {
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
      glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL); // change 4 to indices.count()

    //Disable vertex and texture coordinate arrays
    glDisableVertexAttribArray(vertex_pos);
    glDisableVertexAttribArray(texcoord);
    // glDisableVertexAttribArray(color);
  }
}