// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_PRIMITIVE_BUILDER_HPP_
#define GL_HOMEWORK_PRIMITIVE_BUILDER_HPP_

// clang-format off
// std
#include <cassert>
#include <cstddef>
#include <glm/fwd.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
#include <functional>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
// clang-format on
#include <fmt/core.h>
// project
#include "gl_homework/shader.hpp"

namespace gl_hwk {

struct Primitive {
  GLuint vao;
  GLuint vbo;
  std::optional<GLuint> ebo;
  GLenum type;
  GLsizei size;
};

class PrimitiveBuilder {
 public:
  explicit PrimitiveBuilder(std::shared_ptr<Shader> shader) : shader_(shader) {
    shader_->start();
  }
  ~PrimitiveBuilder() = default;

  auto setShader(std::shared_ptr<Shader> shader) {
    shader_ = shader;
    shader_->start();
  }

  auto buildPrimitvie(GLenum type, const std::string& name,
                      const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices,
                      const std::vector<glm::vec3>& colors,
                      const glm::mat4& model = glm::mat4(1.0f)) {
    assert(poses.size() >= colors.size());
    assert(!name.empty());

    auto draw = [this](const Primitive& info, const glm::mat4& model) {
      // 设置模型系到世界系的变换
      shader_->setMat4("model", model);

      glBindVertexArray(info.vao);
      glBindBuffer(GL_ARRAY_BUFFER, info.vbo);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                            (void*)0);
      glEnableVertexAttribArray(0);
      // texture coord attribute
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                            (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);

      if (info.ebo.has_value()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.ebo.value());
        glDrawElements(info.type, info.size, GL_UNSIGNED_INT, 0);
      } else {
        glDrawArrays(info.type, 0, info.size);
      }
    };

    if (infos_.find(name) != infos_.end()) {
      auto info = infos_[name];
      assert(info.type == type);
      draw(info, model);
    } else {
      GLuint vao, vbo;
      glGenVertexArrays(1, &vao);
      glGenBuffers(1, &vbo);
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);

      infos_[name] = {vao, vbo, std::nullopt, type,
                      static_cast<GLsizei>(positions.size())};

      std::vector<GLfloat> vertices_data;
      for (size_t i = 0; i < positions.size(); ++i) {
        vertices_data.emplace_back(positions[i].x);
        vertices_data.emplace_back(positions[i].y);
        vertices_data.emplace_back(positions[i].z);
        vertices_data.emplace_back(colors[i].x);
        vertices_data.emplace_back(colors[i].y);
        vertices_data.emplace_back(colors[i].z);
      }

      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices_data.size(),
                   vertices_data.data(), GL_STATIC_DRAW);

      // 设置ebo
      if (!indices.empty()) {
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLsizei) * indices.size(),
                     indices.data(), GL_STATIC_DRAW);
        infos_[name].size = indices.size();
        infos_[name].ebo = ebo;
      }

      // 绘制
      draw(infos_[name], model);
    }
  }

  auto buildPoints(const std::string& name,
                   const std::vector<glm::vec3>& positions,
                   const glm::vec3& color,
                   const glm::mat4& model = glm::mat4(1.0f)) -> void {
    auto colors = std::vector<glm::vec3>(positions.size(), color);
    this->buildPrimitvie(GL_POINTS, name, positions, {}, colors, model);
  }

  auto buildPoints(const std::string& name,
                   const std::vector<glm::vec3>& positions,
                   const std::vector<glm::vec3>& colors,
                   const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_POINTS, name, positions, {}, colors, model);
  }

  auto buildPoint(const std::string& name, const glm::vec3& position,
                  const glm::vec3& color,
                  const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPoints(name, {position}, {color}, model);
  }

  auto buildLines(const std::string& name,
                  const std::vector<glm::vec3>& positions,
                  const std::vector<GLsizei>& indices, const glm::vec3& color,
                  const glm::mat4& model = glm::mat4(1.0f)) -> void {
    auto colors = std::vector<glm::vec3>(positions.size(), color);
    this->buildPrimitvie(GL_LINES, name, positions, indices, colors, model);
  }

  auto buildLines(const std::string& name,
                  const std::vector<glm::vec3>& positions,
                  const std::vector<GLsizei>& indices,
                  const std::vector<glm::vec3>& colors,
                  const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_LINES, name, positions, indices, colors, model);
  }

  auto buildLine(const std::string& name, const glm::vec3& start,
                 const glm::vec3& end, const glm::vec3& color,
                 const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildLines(name, {start, end}, {}, {color}, model);
  }

  auto buildLineStrip(const std::string& name,
                      const std::vector<glm::vec3>& positions,
                      const std::vector<glm::vec3>& colors,
                      const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_LINE_STRIP, name, positions, {}, colors, model);
  }

  auto buildLineStrip(const std::string& name,
                      const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices,
                      const std::vector<glm::vec3>& colors,
                      const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_LINE_STRIP, name, positions, indices, colors,
                         model);
  }

  auto buildLineLoop(const std::string& name,
                     const std::vector<glm::vec3>& positions,
                     const std::vector<glm::vec3>& colors,
                     const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_LINE_LOOP, name, positions, {}, colors, model);
  }

  auto buildLineLoop(const std::string& name,
                     const std::vector<glm::vec3>& positions,
                     const std::vector<GLsizei>& indices,
                     const std::vector<glm::vec3>& colors,
                     const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_LINE_LOOP, name, positions, indices, colors, model);
  }

  auto buildTriangles(const std::string& name,
                      const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices,
                      const glm::vec3& color,
                      const glm::mat4& model = glm::mat4(1.0f)) -> void {
    auto colors = std::vector<glm::vec3>(positions.size(), color);
    this->buildPrimitvie(GL_TRIANGLES, name, positions, indices, colors, model);
  }

  auto buildTriangles(const std::string& name,
                      const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices,
                      const std::vector<glm::vec3>& colors,
                      const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_TRIANGLES, name, positions, indices, colors, model);
  }

  auto buildTriangle(const std::string& name, const glm::vec3& p1,
                     const glm::vec3& p2, const glm::vec3& p3,
                     const glm::vec3& color) -> void {
    this->buildTriangles(name, {p1, p2, p3}, {}, {color});
  }

  auto buildTriangleStrip(const std::string& name,
                          const std::vector<glm::vec3>& positions,
                          const std::vector<GLsizei>& indices,
                          const glm::vec3& color,
                          const glm::mat4& model = glm::mat4(1.0f)) -> void {
    auto colors = std::vector<glm::vec3>(positions.size(), color);
    this->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, indices, colors,
                         model);
  }

  auto buildTriangleStrip(const std::string& name,
                          const std::vector<glm::vec3>& positions,
                          const std::vector<GLsizei>& indices,
                          const std::vector<glm::vec3>& colors,
                          const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, indices, colors,
                         model);
  }

  auto buildTriangleStrip(const std::string& name,
                          const std::vector<glm::vec3>& positions,
                          const std::vector<glm::vec3>& colors,
                          const glm::mat4& model = glm::mat4(1.0f)) -> void {
    this->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, {}, colors, model);
  }

 private:
  std::shared_ptr<Shader> shader_;
  std::unordered_map<std::string, Primitive> infos_;
};

}  // namespace gl_hwk
#endif
