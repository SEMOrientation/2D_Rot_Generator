#include <fstream>
#include <sstream>
#include <iostream>
#include <errno.h>
#include <cstring>

#include "shaderClass.h"

int compileShader1(GLenum shaderType, GLuint &shader, int infoLogSize, char *infoLog, const char **src);

Shader::Shader(const char *vertexShaderPath, const char *fragShaderPath) {
  // declare variables to store shader source code
  std::string vertexSrc;
  std::string fragSrc;

  // setup file streams with exceptions
  std::ifstream vertexFile;
  std::ifstream fragFile;
  vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  // try to read the files
  try {
    std::stringstream reader;
    
    // open the vertex shader source code
    vertexFile.open(vertexShaderPath);
    // read into a stringstream
    reader << vertexFile.rdbuf();
    vertexFile.close(); // close fd
    // get as string
    vertexSrc = reader.str();
    // clear buffer
    reader.str(std::string());

    fragFile.open(fragShaderPath);
    reader << fragFile.rdbuf();
    fragFile.close();
    fragSrc = reader.str();
    // don't bother clearing reader as we won't use it again
  } catch(std::ifstream::failure e) {
    std::cerr << "ERROR::SHADER::FAILED_TO_READ_FILE: '" << e.what() << "'; errno " << errno << " '" << strerror(errno) << std::endl;
    throw e;
  }

  // convert std::string source to GL-compatible const char*
  // stream -> string -> const char* is "safer" than stream -> char*
  const char* glVertexSrc = vertexSrc.c_str();
  const char* glFragSrc = fragSrc.c_str();

  // prepare to compile the shaders
  GLuint vertShaderID;
  GLuint fragShaderID;
  char infoLog[512];

  // compile shaders
  //   vertex:
  int success = compileShader1(GL_VERTEX_SHADER, vertShaderID, sizeof(infoLog), infoLog, &glVertexSrc);
  if (!success) {
    // TODO: maybe use a throw here? Can then handle it elsewhere if desired
    std::cerr << "ERROR::SHADER::VERTEX_SHADER::COMPILATION_FAILED\nFrom shader src file '" << vertexShaderPath << "'; Info Log below:\n" << infoLog << std::endl;
    return;
  }
  //   fragment:
  success = compileShader1(GL_FRAGMENT_SHADER, fragShaderID, sizeof(infoLog), infoLog, &glFragSrc);
  if (!success) {
  // TODO: maybe use a throw here? Can then handle it elsewhere if desired
    std::cerr << "ERROR::SHADER::FRAG_SHADER::COMPILATION_FAILED\nFrom shader src file '" << fragShaderPath << "'; Info Log below:\n" << infoLog << std::endl;
    return;
  }

  // construct shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertShaderID);
  glAttachShader(ID, fragShaderID);
  glLinkProgram(ID);

  // confirm successful link
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, sizeof(infoLog), 0, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\nFrom shader src files:\n  - '" << vertexShaderPath << "'\n  - '" << fragShaderPath << "'\nInfo Log below:\n" << infoLog << std::endl;
    ID = 0;
    return;
  }

  // delete intermediate shaders
  glDeleteShader(vertShaderID);
  glDeleteShader(fragShaderID);
}

inline GLuint Shader::getUniformLocation(const std::string &name) const {
  const char* cStrName = name.c_str();
  return glGetUniformLocation(ID, cStrName);
}

void Shader::setUniform(const std::string &name, bool value) const {
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, int value) const {
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, float value) const {
  glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, float v1, float v2, float v3,
                        float v4) const {
  glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

void Shader::setUniform(const std::string &name, float v1, float v2) const {
  glUniform2f(getUniformLocation(name), v1, v2);
}

void Shader::setUniform(const std::string &name, float v1, float v2, float v3) const {
  glUniform3f(getUniformLocation(name), v1, v2, v3);
}

void Shader::use() {
  glUseProgram(ID);
}

/* Helpers */

int compileShader1(GLenum shaderType, GLuint &shader, int infoLogSize, char *infoLog, const char **src) {
  int success;
  shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, src, 0);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, infoLogSize, 0, infoLog);
  }
  return success;
}
