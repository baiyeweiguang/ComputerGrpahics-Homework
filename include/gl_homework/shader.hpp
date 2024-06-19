// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_SHADER_HPP_
#define GL_HOMEWORK_SHADER_HPP_

// clang-format off
// std
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
// clang-format on
#include <fmt/core.h>

namespace gl_hwk {

class Shader {
 public:
  Shader(const std::string &vertex_path, const std::string &fragment_path) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;
    // ensure ifstream objects can throw exceptions:
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      // open files
      v_shader_file.open(vertex_path);
      f_shader_file.open(fragment_path);
      std::stringstream v_shader_stream, f_shader_stream;
      // read file's buffer contents into streams
      v_shader_stream << v_shader_file.rdbuf();
      f_shader_stream << f_shader_file.rdbuf();
      // close file handlers
      v_shader_file.close();
      f_shader_file.close();
      // convert stream into string
      vertex_code = v_shader_stream.str();
      fragment_code = f_shader_stream.str();
    } catch (std::ifstream::failure e) {
      fmt::print("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ, e.what() : {}\n",
                 e.what());
    }

    const char *v_shader_code = vertex_code.c_str();
    const char *f_shader_code = fragment_code.c_str();
    // 2. compile shaders
    GLuint v_shader, f_shader;
    v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, &v_shader_code, nullptr);
    glCompileShader(v_shader);

    checkCompileErrors(v_shader, "VERTEX");

    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &f_shader_code, nullptr);
    glCompileShader(f_shader);

    checkCompileErrors(f_shader, "FRAGMENT");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, v_shader);
    glAttachShader(ID, f_shader);
    glLinkProgram(ID);

    checkCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
  }

  auto start() -> void { glUseProgram(ID); }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  void setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string &name, float x, float y, float z,
               float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }
  // ------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }

  GLuint ID;

 private:
  static auto checkCompileErrors(GLuint shader, std::string type) -> void {
    constexpr int LOG_SIZE = 1024;

    int success;
    char info_log[LOG_SIZE];

    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
        fmt::print("ERROR::SHADER_COMPILATION_ERROR of type: {}\n{}\n", type,
                   info_log);
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, nullptr, info_log);
        fmt::print("ERROR::PROGRAM_LINKING_ERROR of type: {}\n{}\n", type,
                   info_log);
      }
    }
  }
};
}  // namespace gl_hwk

#endif
