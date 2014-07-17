#include "shader.hxx"

namespace Moon {
  glm::mat4 Shader::projection_matrix = glm::mat4(1.0f);
  glm::mat4 Shader::view_matrix = glm::mat4(1.0f);

  /**
   * Store all the file's contents in memory, useful to pass shaders
   * source code to OpenGL
   */
  std::string Shader::file_read(const char *filePath) {
    std::ifstream fileStream;
    fileStream.open(filePath, std::ios_base::in);

    if(fileStream) {     
      std::string buffer(std::istreambuf_iterator<char>(fileStream), (std::istreambuf_iterator<char>()));
      return buffer;
    } else {
      fprintf(stderr, "Could not read shader file %s. File does not exist.\n", filePath);
      return "";
    };
  };

  /**
   * Display compilation errors from the OpenGL shader compiler
   */
  void Shader::print_log(GLuint object)
  {
    GLint infoLogLength = 0;
    if (glIsShader(object))
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
    else if (glIsProgram(object))
      glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
    else {
      fprintf(stderr, "printlog: Not a shader or a program\n");
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
  GLuint Shader::create_shader(const char* filename, GLenum type)
  {
    std::string contents = file_read(filename);
    const GLchar* source = contents.c_str();

    if (source == NULL) {
      fprintf(stderr, "Error opening %s: \n", filename);
      return 0;
    }
    GLuint res = glCreateShader(type);
    glShaderSource(res, 1, &source, NULL);

    glCompileShader(res);
    GLint status;
    glGetShaderiv(res, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      fprintf(stderr, "%s:", filename);
      print_log(res);
      glDeleteShader(res);
      return 0;
    }

    return res;
  }

  GLuint Shader::create_program(const char *vertexfile, const char *fragmentfile) {
    GLuint program = glCreateProgram();
    GLuint shader;

    if(vertexfile) {
      shader = create_shader(vertexfile, GL_VERTEX_SHADER);
      if(!shader)
        return 0;
      glAttachShader(program, shader);
      glDeleteShader(shader); // http://stackoverflow.com/questions/9113154/proper-way-to-delete-glsl-shader
    }

    if(fragmentfile) {
      shader = create_shader(fragmentfile, GL_FRAGMENT_SHADER);
      if(!shader)
        return 0;
      glAttachShader(program, shader);
      glDeleteShader(shader);
    }

    if (LEGACY_GL) {
      this->bind_attribute(0, "vertex_pos");
      this->bind_attribute(1, "texcoord");
      this->bind_attribute(2, "color");
    }

    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
      fprintf(stderr, "glLinkProgram:");
      print_log(program);
      glDeleteProgram(program);
      return 0;
    }

    return program;
  }

  Shader::Shader(const char *vertexfile, const char *fragmentfile)
  : Cache(std::make_pair(vertexfile, fragmentfile))
  {
    _program = create_program(vertexfile, fragmentfile);
  };

  Shader::~Shader() {
    glDeleteProgram(_program);
  };

  GLint Shader::get_attribute(const char *name) {
    try {
      return _attributeList.at(name);
    }
    catch (const std::out_of_range& oor) { // attribute not found, load it!
      GLint attribute = glGetAttribLocation(_program, name);
      if(attribute == -1)
        fprintf(stderr, "Could not bind attribute %s\n", name);
      _attributeList[name] = attribute;
      return attribute;
    }
  }

  GLint Shader::get_uniform(const char *name) {
    try {
      return _uniformLocationList.at(name);
    }
    catch (const std::out_of_range& oor) { // uniform not found, load it!
      GLint uniform = glGetUniformLocation(_program, name);
      if(uniform == -1)
        fprintf(stderr, "Could not bind uniform %s\n", name);
      _uniformLocationList[name] = uniform;
      return uniform;
    }
  }
  
  void Shader::bind_attribute(GLuint location, const char *name) {
    glBindAttribLocation(_program, location, name);
  }

  GLuint Shader::get_program() {
    return _program;
  }

  void Shader::use() {
    glUseProgram(_program);
  }
};