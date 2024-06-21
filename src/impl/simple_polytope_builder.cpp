#include "gl_homework/simple_polytope_builder.hpp"

// clang-format off
// std
#include <algorithm>
#include <glm/fwd.hpp>
#include <memory>
// glm
#include <glm/glm.hpp>
// project
#include "gl_homework/impl.hpp"
#include "gl_homework/primitive_builder.hpp"
// clang-format on

namespace gl_hwk {
class SimplePolytopeBuilderImpl {
 public:
  SimplePolytopeBuilderImpl(std::shared_ptr<PrimitiveBuilder> primitive_builder)
      : primitive_builder_(primitive_builder) {}

  std::shared_ptr<PrimitiveBuilder> primitive_builder_;
};

SimplePolytopeBuilder::SimplePolytopeBuilder(std::shared_ptr<PrimitiveBuilder> primitive_builder) {
  impl_ = make_unique_impl<SimplePolytopeBuilderImpl>(primitive_builder);
}

auto SimplePolytopeBuilder::buildCube(const std::string& name, const glm::vec3& center, const glm::vec3& size,
                                      const std::vector<std::vector<float>>& other_data) -> void {
  auto vertices = std::vector<glm::vec3>{
      {-0.5F, -0.5F, -0.5F}, {-0.5F, 0.5F, -0.5F}, {0.5F, 0.5F, -0.5F}, {0.5F, -0.5F, -0.5F},
      {-0.5F, -0.5F, 0.5F},  {-0.5F, 0.5F, 0.5F},  {0.5F, 0.5F, 0.5F},  {0.5F, -0.5F, 0.5F},
  };

  auto indices = std::vector<GLsizei>{
      0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 5, 4, 7, 7, 6, 5, 4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4,
  };

  std::for_each(vertices.begin(), vertices.end(), [center, size](glm::vec3& v) { v = center + v * size; });

  impl_->primitive_builder_->buildTriangles(name, vertices, indices, other_data);
}

auto SimplePolytopeBuilder::buildTeraHedron(const std::string& name, const glm::vec3& center, const glm::vec3& size,
                                            const std::vector<std::vector<float>>& other_data) -> void {
  auto vertices = std::vector<glm::vec3>{
      {0.0F, 0.0F, 1.0F},
      {0.0F, 0.942809F, -0.333333F},
      {-0.816497F, -0.471405F, -0.333333F},
      {0.816497F, -0.471405F, -0.333333F},
  };
  auto indices = std::vector<GLsizei>{0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3};

  std::for_each(vertices.begin(), vertices.end(), [center, size](glm::vec3& v) { v = center + v * size; });

  impl_->primitive_builder_->buildTriangles(name, vertices, indices, other_data);
}

auto SimplePolytopeBuilder::buildRect(const std::string& name, const glm::vec3& p1, const glm::vec3& p2,
                                      const glm::vec3& p3, const std::vector<std::vector<float>>& other_data) -> void {
  // v04 = (v12 + v13) - v01
  glm::vec3 p4 = p2 + p3 - p1;

  auto vertices = std::vector<glm::vec3>{p1, p2, p3, p4};
  auto indices = std::vector<GLsizei>{0, 1, 2, 2, 3, 1};

  impl_->primitive_builder_->buildTriangles(name, vertices, indices, other_data);
}

}  // namespace gl_hwk