// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_SHADER_HPP_
#define GL_HOMEWORK_SHADER_HPP_

// clang-format off
// std
#include <string>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
// third party
#include <fmt/core.h>
// project
#include "gl_homework/impl.hpp"
// clang-format on

namespace gl_hwk {

class ShaderImpl;
class Shader {
 public:
  Shader(const std::string &vertex_path, const std::string &fragment_path);
  auto start() -> void;
  auto setBool(const std::string &name, bool value) const -> void;
  auto setInt(const std::string &name, int value) const -> void;
  auto setFloat(const std::string &name, float value) const -> void;
  auto setVec2(const std::string &name, const glm::vec2 &value) const -> void;
  auto setVec2(const std::string &name, float x, float y) const -> void;
  auto setVec3(const std::string &name, const glm::vec3 &value) const -> void;
  auto setVec3(const std::string &name, float x, float y, float z) const
      -> void;
  auto setVec4(const std::string &name, const glm::vec4 &value) const -> void;
  auto setVec4(const std::string &name, float x, float y, float z,
               float w) const -> void;
  auto setMat2(const std::string &name, const glm::mat2 &mat) const -> void;
  auto setMat3(const std::string &name, const glm::mat3 &mat) const -> void;
  auto setMat4(const std::string &name, const glm::mat4 &mat) const -> void;

 public:
  GLuint ID;

 private:
  UniqueImpl<ShaderImpl> impl_;
};
}  // namespace gl_hwk

#endif
