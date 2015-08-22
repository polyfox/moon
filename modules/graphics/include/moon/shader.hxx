#ifndef MOON_SHADER_H
#define MOON_SHADER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include "moon/intern.h"
#include "moon/gl.h"
#include "moon/glm.h"
#include "moon/vector1.hxx"
#include "moon/vector2.hxx"
#include "moon/vector3.hxx"
#include "moon/vector4.hxx"
#include "moon/matrix4.hxx"

namespace Moon {
  class Shader {
    typedef std::unordered_map<std::string, GLint> AttributeMap;
  public:
    static bool is_legacy;

    Shader(const std::string vertexShader, const std::string fragmentShader);
    ~Shader();
    void   Use();
    GLint  Attribute(std::string name);
    GLint  Uniform(std::string name);

    void SetUniform(std::string name, const GLint v1);
    void SetUniform(std::string name, const GLfloat v1);
    //void SetUniform(std::string name, const Moon::Vector1 &vec);
    void SetUniform(std::string name, const Moon::Vector2 &vec);
    void SetUniform(std::string name, const Moon::Vector3 &vec);
    void SetUniform(std::string name, const Moon::Vector4 &vec);
    void SetUniform(std::string name, const Moon::Matrix4 &mat);
  private:
    GLuint  m_program;
    AttributeMap m_attributeList;
    AttributeMap m_uniformLocationList;

    void   PrintLog(GLuint object);
    GLuint CreateShader(const std::string contents, GLenum type);
    GLuint CreateProgram(const std::string vertexShader, const std::string fragmentShader);
    GLint  InitAttribute(std::string name);
    GLint  InitUniform(std::string name);
  };
};
#endif
