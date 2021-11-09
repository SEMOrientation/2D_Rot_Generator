#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H 1

#include <string>
#include <glad/glad.h>

class Shader {
 private:
  GLuint getUniformLocation(const std::string &name) const;
 public:
  GLuint ID; // shader program ID
  
  Shader(const char* vertexShaderPath, const char* fragShaderPath);
  void use();

  // utilities
  void setUniform(const std::string &name, bool value) const;
  void setUniform(const std::string &name, int value) const;
  void setUniform(const std::string &name, float value) const;
  void setUniform(const std::string &name, float v1, float v2, float v3, float v4) const;
  void setUniform(const std::string &name, float v1, float v2) const;
  void setUniform(const std::string &name, float v1, float v2, float v3) const;

  // utility aliases for convenience
  void setBool(const std::string &name, bool value) const {
    setUniform(name, (bool)value);
  }
  void setInt(const std::string &name, int value) const {
    setUniform(name, (int)value);
  }
  void setFloat(const std::string &name, float value) const {
    setUniform(name, (float)value);
  }
  void set4Vec(const std::string &name, float v1, float v2, float v3,
               float v4) const {
    setUniform(name, v1, v2, v3, v4);
  }
  void set2Vec(const std::string &name, float v1, float v2) const {
    setUniform(name, v1, v2);
  }
  void set3Vec(const std::string &name, float v1, float v2, float v3) const {
    setUniform(name, v1, v2, v3);
  }
};
#endif
