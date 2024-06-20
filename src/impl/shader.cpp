#include "gl_homework/shader.hpp"

#include <fstream>
#include <sstream>

namespace gl_hwk {
class ShaderImpl {
 public:
  ShaderImpl();
  auto checkCompileErrors(GLuint shader, const std::string &type) const -> void;
};

ShaderImpl::ShaderImpl() {}

auto ShaderImpl::checkCompileErrors(GLuint shader,
                                    const std::string &type) const -> void {
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

Shader::Shader(const std::string &vertex_path,
               const std::string &fragment_path) {
  impl_ = make_unique_impl<ShaderImpl>();

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

  impl_->checkCompileErrors(v_shader, "VERTEX");

  f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f_shader, 1, &f_shader_code, nullptr);
  glCompileShader(f_shader);

  impl_->checkCompileErrors(f_shader, "FRAGMENT");

  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, v_shader);
  glAttachShader(ID, f_shader);
  glLinkProgram(ID);

  impl_->checkCompileErrors(ID, "PROGRAM");

  // delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(v_shader);
  glDeleteShader(f_shader);
}

auto Shader::start() -> void { glUseProgram(ID); }

auto Shader::setBool(const std::string &name, bool value) const -> void {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

auto Shader::setInt(const std::string &name, int value) const -> void {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

auto Shader::setFloat(const std::string &name, float value) const -> void {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

auto Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    -> void {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

auto Shader::setVec2(const std::string &name, float x, float y) const -> void {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

auto Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    -> void {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

auto Shader::setVec3(const std::string &name, float x, float y, float z) const
    -> void {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

auto Shader::setVec4(const std::string &name, const glm::vec4 &value) const
    -> void {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

auto Shader::setVec4(const std::string &name, float x, float y, float z,
                     float w) const -> void {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

auto Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
    -> void {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

auto Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    -> void {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

auto Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    -> void {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

}  // namespace gl_hwk