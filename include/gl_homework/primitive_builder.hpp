// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_PRIMITIVE_BUILDER_HPP_
#define GL_HOMEWORK_PRIMITIVE_BUILDER_HPP_

// clang-format off
// std
#include <vector>
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

class PrimitiveBuilderImpl;
class PrimitiveBuilder {
 public:
  explicit PrimitiveBuilder();

  auto buildPoints(const std::string& name, const std::vector<glm::vec3>& positions,
                   const std::vector<std::vector<float>>& other_data) -> void;

  auto buildPoint(const std::string& name, const glm::vec3& position, const std::vector<float>& other_data) -> void;

  auto buildLines(const std::string& name, const std::vector<glm::vec3>& positions, const std::vector<GLsizei>& indices,
                  const std::vector<std::vector<float>>& other_data) -> void;

  auto buildLine(const std::string& name, const glm::vec3& start, const glm::vec3& end,
                 const std::vector<float>& other_data) -> void;

  auto buildLineStrip(const std::string& name, const std::vector<glm::vec3>& positions,
                      const std::vector<std::vector<float>>& other_data) -> void;

  auto buildLineStrip(const std::string& name, const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices, const std::vector<std::vector<float>>& other_data) -> void;

  auto buildLineLoop(const std::string& name, const std::vector<glm::vec3>& positions,
                     const std::vector<std::vector<float>>& other_data) -> void;

  auto buildLineLoop(const std::string& name, const std::vector<glm::vec3>& positions,
                     const std::vector<GLsizei>& indices, const std::vector<std::vector<float>>& other_data) -> void;

  auto buildTriangles(const std::string& name, const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices, const std::vector<std::vector<float>>& other_data) -> void;

  auto buildTriangle(const std::string& name, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
                     const std::vector<float>& other_data) -> void;

  auto buildTriangleStrip(const std::string& name, const std::vector<glm::vec3>& positions,
                          const std::vector<GLsizei>& indices, const std::vector<std::vector<float>>& other_data)
      -> void;

  auto buildTriangleStrip(const std::string& name, const std::vector<glm::vec3>& positions,
                          const std::vector<std::vector<float>>& other_data) -> void;

 private:
  // 隐藏实现
  unique_impl<PrimitiveBuilderImpl> impl_;
};
}  // namespace gl_hwk
#endif
