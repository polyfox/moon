#include "texture.hxx"
#include <SOIL.h>

namespace Moon {
  Texture::Texture(std::string filename)
  : CacheObject(filename),
  shader("resources/shaders/quad.vert", "resources/shaders/quad.frag")
  {
    shader.add_attribute("texcoord");
    shader.add_attribute("vertex_pos");
    shader.add_uniform("model_matrix");
    shader.add_uniform("projection_matrix");
    shader.add_uniform("opacity");

    unsigned char* pixels;
    int channels;

    pixels = SOIL_load_image(filename.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_RGBA);
    texture_id = SOIL_create_OGL_texture(pixels, texture_width, texture_height, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
  };

  Texture::~Texture() {
    //Delete texture
    if(texture_id != 0) {
      glDeleteTextures(1, &texture_id);
    }
  };

  GLuint Texture::width() {
    return texture_width;
  };

  GLuint Texture::height() {
    return texture_height;
  };

  GLuint Texture::id() {
    return texture_id;
  };

  void Texture::render(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &opacity, const GLuint &vboID, const GLuint &iboID) {
    if(texture_id != 0) {
      shader.use();

      //model matrix - move it to the correct position in the world
      glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
      glUniformMatrix4fv(shader.get_uniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

      //projection matrix
      glUniformMatrix4fv(shader.get_uniform("projection_matrix"), 1, GL_FALSE, glm::value_ptr(Shader::projection_matrix));

      //Set texture ID
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glUniform1i(shader.get_uniform("texture"), /*GL_TEXTURE*/0);

      //Enable vertex and texture coordinate arrays
      glEnableVertexAttribArray(shader.get_attribute("vertex_pos"));
      glEnableVertexAttribArray(shader.get_attribute("texcoord"));

        //Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        glVertexAttribPointer(
          shader.get_attribute("texcoord"), // attribute
          2,                  // number of elements per vertex, here (x,y)
          GL_FLOAT,           // the type of each element
          GL_FALSE,           // take our values as-is
          sizeof(VertexData2D),                  // stride
          (GLvoid*)offsetof(VertexData2D, u)     // offset of first element
        );

        glVertexAttribPointer(
          shader.get_attribute("vertex_pos"), // attribute
          2,                  // number of elements per vertex, here (x,y)
          GL_FLOAT,           // the type of each element
          GL_FALSE,           // take our values as-is
          sizeof(VertexData2D),                  // stride
          (GLvoid*)offsetof(VertexData2D, x)     // offset of first element
        );

        glUniform1f(shader.get_uniform("opacity"), opacity); // opacity TODO: tone

        //Draw quad using vertex data and index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);

      //Disable vertex and texture coordinate arrays
      glDisableVertexAttribArray(shader.get_attribute("vertex_pos"));
      glDisableVertexAttribArray(shader.get_attribute("texcoord"));
    };
  };

  std::shared_ptr<Texture> Texture::load(std::string filename) {
    auto const it = CacheObject::_cache.find(filename);
    if (it == CacheObject::_cache.end()) {
      //std::cout << "cache miss!" << std::endl;
      std::shared_ptr<Texture> ptr = std::shared_ptr<Texture>(new Texture(filename));
      // std::shared_ptr<Texture> ptr = std::make_shared<Texture>(filename);
      // is supposedly faster, we just need to get around the private constructor issue
      CacheObject::_cache[filename] = ptr;
      return ptr;
    }
    //std::cout << "cache hit!" << std::endl;
    return std::static_pointer_cast<Texture>(it->second.lock());
  }

  std::unordered_map<std::string, std::weak_ptr<CacheObject>> CacheObject::_cache;

  CacheObject::CacheObject(std::string const& key) {
    std::cout << "added " << key << " to cache" << std::endl;
    this->_key = key;
    //CacheObject::_cache[key] = this->shared_from_this(); // Note: override previous resource of same key, if any
    // this doesn't work because the object wasn't constructed yet and at least one shared_ptr must exist
  };

  CacheObject::~CacheObject() {
    std::cout << "removed " << _key << " from cache" << std::endl;
    CacheObject::_cache.erase(_key);
  };

  /*std::shared_ptr<CacheObject> CacheObject::get(std::string const& key) const {
    auto const it = _cache.find(key);
    if (it == _cache.end()) { return std::shared_ptr<CacheObject>(); }

    return it->second.lock();
  };*/

}