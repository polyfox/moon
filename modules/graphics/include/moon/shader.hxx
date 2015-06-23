#ifndef MOON_SHADER_H
#define MOON_SHADER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include "moon/intern.h"
#include "moon/gl.h"
#include "moon/glm.h"
#include "moon/cache.hxx"

namespace Moon {
  class Shader {
    typedef std::unordered_map<const char *, GLint> AttributeMap;
  public:
    static glm::mat4 projection_matrix; // TEMPORARY LOCATION, TODO MOVE TO RENDERER
    static glm::mat4 view_matrix; // camera. TEMPORARY LOCATION, TODO MOVE TO RENDERER
    static bool is_legacy;

    Shader(const std::string vertexShader, const std::string fragmentShader);
    ~Shader();
    void   Use();
    GLuint GetProgram();
    GLint  GetAttribute(const char *name);
    GLint  GetUniform(const char *name);
  private:
    GLuint  m_program;
    AttributeMap m_attributeList;
    AttributeMap m_uniformLocationList;

    void   PrintLog(GLuint object);
    GLuint CreateShader(const std::string contents, GLenum type);
    GLuint CreateProgram(const std::string vertexShader, const std::string fragmentShader);
    GLint  InitAttribute(const char *name);
    GLint  InitUniform(const char *name);
  };
};
#endif
