// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_SIMPLE_POLYTOPE_BUILDER_HPP_
#define GL_HOMEWORK_SIMPLE_POLYTOPE_BUILDER_HPP_

// clang-format off
// std
#include <memory>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
// third party
#include <fmt/core.h>
// project
#include "gl_homework/impl.hpp"
#include "gl_homework/primitive_builder.hpp"
// clang-format on

namespace gl_hwk {
class SimplePolytopeBuilderImpl;
/**
 * @brief 多面体构建者，用于构建简单的多面体(立方体、四面体、平面)
 */
class SimplePolytopeBuilder {
 public:
  explicit SimplePolytopeBuilder(std::shared_ptr<PrimitiveBuilder> primitive_builder);

  /**
   * @brief 构建一个立方体，共有8个顶点，顺序为{正面左下，左上，右上，右下，背面左下，左上，右上，右下}
   * @param name 立方体的名字，作为VAO的key
   * @param center 立方体的中心
   * @param size 立方体的大小
   * @param other_data
   * 其他需要传入顶点着色器的数据，other_data[i]为传入第i个顶点数据
   */
  auto buildCube(const std::string& name, const glm::vec3& center, const glm::vec3& size,
                 const std::vector<std::vector<float>>& other_data) -> void;

  /**
   * @brief 构建一个四面体，共有4个顶点，顺序为{正面顶点，左下，右下，底面顶点}
   * @param name 四面体的名字，作为VAO的key
   * @param center 四面体的中心
   * @param size 四面体的大小
   *@param other_data
   *其他需要传入顶点着色器的数据，other_data[i]为传入第i个顶点数据
   */
  auto buildTeraHedron(const std::string& name, const glm::vec3& center, const glm::vec3& size,
                       const std::vector<std::vector<float>>& other_data) -> void;

  /**
   * @brief 构建一个平面，共有4个顶点，顺序为{p1, p2, p3, p4}, p4由p1沿着p2、p3对称得到
   * @param name 平面的名字，作为VAO的key
   * @param p1 p1
   * @param p2 p2
   * @param p3 p3
   * @param other_data
   *其他需要传入顶点着色器的数据，other_data[i]为传入第i个顶点数据
   */
  auto buildRect(const std::string& name, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
                 const std::vector<std::vector<float>>& other_data) -> void;

 private:
  unique_impl<SimplePolytopeBuilderImpl> impl_;
};
}  // namespace gl_hwk
#endif