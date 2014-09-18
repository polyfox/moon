#ifndef MOON_SHADER_H
#define MOON_SHADER_H

#include "moon/stdh.hxx"
//#include <utility> // std::pair
#include <fstream>
#include "moon/cache.hxx"

namespace Moon {

  class Shader: public Cache<Shader, std::pair<const char*, const char*>> {
  public:
    ~Shader();

    void   use();
    GLuint get_program();
    GLint  get_attribute(const char *name);
    GLint  get_uniform(const char *name);

    void   bind_attribute(GLuint location, const char *name);

    static glm::mat4 projection_matrix; // TEMPORARY LOCATION, TODO MOVE TO RENDERER
    static glm::mat4 view_matrix; // camera. TEMPORARY LOCATION, TODO MOVE TO RENDERER
  private:
    Shader(const char *vertexfile, const char *fragmentfile);

    std::string file_read(const char *filePath);
    void   print_log(GLuint object);
    GLuint create_shader(const char* filename, GLenum type);
    GLuint create_program(const char *vertexfile, const char *fragmentfile);

    GLuint  _program;
    std::unordered_map<const char *,GLuint> _attributeList;
    std::unordered_map<const char *,GLuint> _uniformLocationList;
  friend class Cache<Shader, std::pair<const char*, const char*>>;
  };
};

#endif
