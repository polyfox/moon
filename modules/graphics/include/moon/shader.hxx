#ifndef MOON_SHADER_H
#define MOON_SHADER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include "moon/intern.h"
#include "moon/gl.h"
#include "moon/glm.h"
#include "moon/vector4.hxx"

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

    void SetUniform(const char *name, const GLint v1);
    void SetUniform(const char *name, const GLfloat v1);
    void SetUniform(const char *name, const Moon::Vector4 &vec);
    void SetUniform(const char *name, const glm::mat4 &mat);
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
