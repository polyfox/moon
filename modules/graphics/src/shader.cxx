#include <iostream>
#include "moon/engine.hxx"
#include "moon/shader.hxx"

namespace Moon {
  glm::mat4 Shader::projection_matrix = glm::mat4(1.0f);
  glm::mat4 Shader::view_matrix = glm::mat4(1.0f);
  bool Shader::is_legacy = false;

  Shader::Shader()
  {
    m_program = 0;
  };

  Shader::~Shader() {
    glDeleteProgram(m_program);
  };

  void Shader::CreateFromString(const std::string vertexContent, const std::string fragmentContent) {
    m_program = CreateProgram(vertexContent, fragmentContent);
  };

  /**
   * Store all the file's contents in memory, useful to pass shaders
   * source code to OpenGL
   */
  std::string Shader::ReadFile(const char* filePath) {
    std::ifstream fileStream;
    fileStream.open(filePath, std::ios_base::in);

    if (fileStream) {
      std::string buffer(std::istreambuf_iterator<char>(fileStream), (std::istreambuf_iterator<char>()));
      return buffer;
    } else {
      std::cerr << "Could not read shader file" << filePath << ". File does not exist." << std::endl;
      return "";
    };
  };

  void Shader::CreateFromFile(const char* vertexFilename, const char* fragmentFilename) {
    std::string vertexContents(ReadFile(vertexFilename));
    std::string fragmentContents(ReadFile(fragmentFilename));
    CreateFromString(vertexContents, fragmentContents);
  };

  /**
   * Display compilation errors from the OpenGL shader compiler
   */
  void Shader::PrintLog(GLuint object)
  {
    GLint infoLogLength = 0;
    if (glIsShader(object))
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
    else if (glIsProgram(object))
      glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
    else {
      std::cerr << "Shader: Not a shader or program" << std::endl;
      return;
    }

    GLchar *infoLog = new GLchar[infoLogLength];

    if (glIsShader(object))
      glGetShaderInfoLog(object, infoLogLength, NULL, infoLog);
    else if (glIsProgram(object))
      glGetProgramInfoLog(object, infoLogLength, NULL, infoLog);

    fprintf(stderr, "%s", infoLog);
    delete [] infoLog;
  }

  /**
   * Compile the shader from file 'filename', with error handling
   */
  GLuint Shader::CreateShader(const std::string contents, GLenum type)
  {
    const GLchar* source = contents.c_str();
    GLuint res = glCreateShader(type);
    glShaderSource(res, 1, &source, NULL);

    glCompileShader(res);
    GLint status;
    glGetShaderiv(res, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      fprintf(stderr, "Shader::CreateShader(_, %i) error occured:", type);
      PrintLog(res);
      glDeleteShader(res);
      return 0;
    }

    return res;
  }

  GLuint Shader::CreateProgram(const std::string vertexContent, const std::string fragmentContent) {
    GLuint shader;
    GLuint program = glCreateProgram();
    assert(program);

    shader = CreateShader(vertexContent, GL_VERTEX_SHADER);
    if (!shader) return 0;
    glAttachShader(program, shader);
    glDeleteShader(shader); // http://stackoverflow.com/questions/9113154/proper-way-to-delete-glsl-shader

    shader = CreateShader(fragmentContent, GL_FRAGMENT_SHADER);
    if(!shader) return 0;
    glAttachShader(program, shader);
    glDeleteShader(shader);

    if (Shader::is_legacy) {
      glBindAttribLocation(program, 0, "vertex_pos");
      glBindAttribLocation(program, 1, "texcoord");
      glBindAttribLocation(program, 2, "color");
    }

    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
      fprintf(stderr, "glLinkProgram:");
      PrintLog(program);
      glDeleteProgram(program);
      return 0;
    }

    return program;
  }

  GLint Shader::InitAttribute(const char *name) {
    GLint attribute = glGetAttribLocation(m_program, name);
    if (attribute == -1) {
      fprintf(stderr, "Shader(%i): Could not bind attribute %s: %x\n", m_program, name, glGetError());
      abort();
    }
    m_attributeList[name] = attribute;
    return attribute;
  }

  GLint Shader::InitUniform(const char *name) {
    GLint uniform = glGetUniformLocation(m_program, name);
    if (uniform == -1) {
      fprintf(stderr, "Shader(%i): Could not bind uniform %s: %x\n", m_program, name, glGetError());
      abort();
    }
    m_uniformLocationList[name] = uniform;
    return uniform;
  }

  GLint Shader::GetAttribute(const char *name) {
    assert(m_program);
    assert(name);
    AttributeMap::iterator iter = m_attributeList.find(name);
    if (iter == m_attributeList.end()) {
      return InitAttribute(name);
    } else {
      return iter->second;
    }
  }

  GLint Shader::GetUniform(const char *name) {
    assert(m_program);
    assert(name);
    AttributeMap::iterator iter = m_uniformLocationList.find(name);
    if (iter == m_uniformLocationList.end()) {
      return InitUniform(name);
    } else {
      return iter->second;
    }
  }

  GLuint Shader::GetProgram() {
    return m_program;
  }

  void Shader::Use() {
    assert(m_program);
    glUseProgram(m_program);
  }
};
