#include "gl_homework/primitive_builder.hpp"

#include <unordered_map>

namespace gl_hwk {
class PrimitiveBuilderImpl {
 public:
  explicit PrimitiveBuilderImpl(std::shared_ptr<Shader> shader)
      : shader_(shader) {
    shader_->start();
  }

  auto setShader(std::shared_ptr<Shader> shader) -> void {
    shader_ = shader;
    shader_->start();
  }

  auto buildPrimitvie(GLenum type, const std::string& name,
                      const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices,
                      const std::vector<glm::vec3>& colors,
                      const glm::mat4& model) -> void {
    assert(positions.size() >= colors.size());
    assert(!name.empty());

    auto draw = [this](const Primitive& info, const glm::mat4& model) {
      shader_->setMat4("model", model);

      glBindVertexArray(info.vao);
      glBindBuffer(GL_ARRAY_BUFFER, info.vbo);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                            (void*)0);
      glEnableVertexAttribArray(0);
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
      auto& info = infos_[name];
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

      if (!indices.empty()) {
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLsizei) * indices.size(),
                     indices.data(), GL_STATIC_DRAW);
        infos_[name].size = indices.size();
        infos_[name].ebo = ebo;
      }

      draw(infos_[name], model);
    }
  }

 private:
  std::shared_ptr<Shader> shader_;
  std::unordered_map<std::string, Primitive> infos_;
};

PrimitiveBuilder::PrimitiveBuilder(std::shared_ptr<Shader> shader) {
  impl_ = make_unique_impl<PrimitiveBuilderImpl>(shader);
}

auto PrimitiveBuilder::setShader(std::shared_ptr<Shader> shader) -> void {
  impl_->setShader(shader);
}

auto PrimitiveBuilder::buildPoints(const std::string& name,
                                   const std::vector<glm::vec3>& positions,
                                   const glm::vec3& color,
                                   const glm::mat4& model) -> void {
  auto colors = std::vector<glm::vec3>(positions.size(), color);
  impl_->buildPrimitvie(GL_POINTS, name, positions, {}, colors, model);
}

auto PrimitiveBuilder::buildPoints(const std::string& name,
                                   const std::vector<glm::vec3>& positions,
                                   const std::vector<glm::vec3>& colors,
                                   const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_POINTS, name, positions, {}, colors, model);
}

auto PrimitiveBuilder::buildPoint(const std::string& name,
                                  const glm::vec3& position,
                                  const glm::vec3& color,
                                  const glm::mat4& model) -> void {
  auto positions = std::vector<glm::vec3>{position};
  auto colors = std::vector<glm::vec3>{color};
  impl_->buildPrimitvie(GL_POINTS, name, positions, {}, colors, model);
}

auto PrimitiveBuilder::buildLines(const std::string& name,
                                  const std::vector<glm::vec3>& positions,
                                  const std::vector<GLsizei>& indices,
                                  const glm::vec3& color,
                                  const glm::mat4& model) -> void {
  auto colors = std::vector<glm::vec3>(positions.size(), color);
  impl_->buildPrimitvie(GL_LINES, name, positions, indices, colors, model);
}

auto PrimitiveBuilder::buildLines(const std::string& name,
                                  const std::vector<glm::vec3>& positions,
                                  const std::vector<GLsizei>& indices,
                                  const std::vector<glm::vec3>& colors,
                                  const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_LINES, name, positions, indices, colors, model);
}

auto PrimitiveBuilder::buildLine(const std::string& name,
                                 const glm::vec3& start, const glm::vec3& end,
                                 const glm::vec3& color, const glm::mat4& model)
    -> void {
  auto positions = std::vector<glm::vec3>{start, end};
  auto colors = std::vector<glm::vec3>{color, color};
  impl_->buildPrimitvie(GL_LINES, name, positions, {}, colors, model);
}

auto PrimitiveBuilder::buildLineStrip(const std::string& name,
                                      const std::vector<glm::vec3>& positions,
                                      const std::vector<glm::vec3>& colors,
                                      const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_LINE_STRIP, name, positions, {}, colors, model);
}

auto PrimitiveBuilder::buildLineStrip(const std::string& name,
                                      const std::vector<glm::vec3>& positions,
                                      const std::vector<GLsizei>& indices,
                                      const std::vector<glm::vec3>& colors,
                                      const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_LINE_STRIP, name, positions, indices, colors, model);
}

auto PrimitiveBuilder::buildLineLoop(const std::string& name,
                                     const std::vector<glm::vec3>& positions,
                                     const std::vector<glm::vec3>& colors,
                                     const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_LINE_LOOP, name, positions, {}, colors, model);
}

auto PrimitiveBuilder::buildLineLoop(const std::string& name,
                                     const std::vector<glm::vec3>& positions,
                                     const std::vector<GLsizei>& indices,
                                     const std::vector<glm::vec3>& colors,
                                     const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_LINE_LOOP, name, positions, indices, colors, model);
}

auto PrimitiveBuilder::buildTriangles(const std::string& name,
                                      const std::vector<glm::vec3>& positions,
                                      const std::vector<GLsizei>& indices,
                                      const glm::vec3& color,
                                      const glm::mat4& model) -> void {
  auto colors = std::vector<glm::vec3>(positions.size(), color);
  impl_->buildPrimitvie(GL_TRIANGLES, name, positions, indices, colors, model);
}

auto PrimitiveBuilder::buildTriangles(const std::string& name,
                                      const std::vector<glm::vec3>& positions,
                                      const std::vector<GLsizei>& indices,
                                      const std::vector<glm::vec3>& colors,
                                      const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_TRIANGLES, name, positions, indices, colors, model);
}

auto PrimitiveBuilder::buildTriangle(const std::string& name,
                                     const glm::vec3& p1, const glm::vec3& p2,
                                     const glm::vec3& p3,
                                     const glm::vec3& color) -> void {
  auto positions = std::vector<glm::vec3>{p1, p2, p3};
  auto colors = std::vector<glm::vec3>{color, color, color};
  impl_->buildPrimitvie(GL_TRIANGLES, name, positions, {}, colors,
                        glm::mat4(1.0f));
}

auto PrimitiveBuilder::buildTriangleStrip(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<GLsizei>& indices, const glm::vec3& color,
    const glm::mat4& model) -> void {
  auto colors = std::vector<glm::vec3>(positions.size(), color);
  impl_->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, indices, colors,
                        model);
}

auto PrimitiveBuilder::buildTriangleStrip(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<GLsizei>& indices, const std::vector<glm::vec3>& colors,
    const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, indices, colors,
                        model);
}

auto PrimitiveBuilder::buildTriangleStrip(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& colors, const glm::mat4& model) -> void {
  assert(positions.size() == colors.size());
  impl_->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, {}, colors, model);
}
}  // namespace gl_hwk