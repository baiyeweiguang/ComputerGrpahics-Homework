#include "gl_homework/primitive_builder.hpp"

#include <cstdint>
#include <unordered_map>
#include <optional>

namespace gl_hwk {

struct Primitive {
  GLuint vao;
  GLuint vbo;
  std::optional<GLuint> ebo;
  GLenum type;
  GLsizei size;
  GLuint other_data_num;
};

class PrimitiveBuilderImpl {
 public:
  explicit PrimitiveBuilderImpl(std::shared_ptr<Shader> shader)
      : shader_(shader) {
  }

  auto setShader(std::shared_ptr<Shader> shader) -> void {
    shader_ = shader;
  }

  auto buildPrimitvie(GLenum type, const std::string& name,
                      const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices,
                      const std::vector<std::vector<float>>& other_data,
                      const glm::mat4& model) -> void {
    assert(positions.size() >= other_data.size());
    assert(!name.empty());
    shader_->start();
    auto draw = [this](const Primitive& info, const glm::mat4& model) {
      shader_->setMat4("model", model);

      glBindVertexArray(info.vao);
      glBindBuffer(GL_ARRAY_BUFFER, info.vbo);
      uint32_t vertex_total_data_num = 3 + info.other_data_num * 3;
      // Position
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_total_data_num * sizeof(float),
                            (void*)0);
      glEnableVertexAttribArray(0);
      // other data
      for (uint32_t i = 0; i < info.other_data_num; ++i) {
        glVertexAttribPointer(1 + i, 3, GL_FLOAT, GL_FALSE, vertex_total_data_num * sizeof(float),
                              (void*)((3 + i * 3) * sizeof(float)));
        glEnableVertexAttribArray(1 + i);
      }

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

      uint32_t other_data_num =
          other_data.empty() ? 0 : other_data.front().size();
      other_data_num = static_cast<uint32_t>(other_data_num / 3);
      infos_[name].other_data_num = other_data_num;

      std::vector<GLfloat> vertices_data;
      for (size_t i = 0; i < positions.size(); ++i) {
        vertices_data.emplace_back(positions[i].x);
        vertices_data.emplace_back(positions[i].y);
        vertices_data.emplace_back(positions[i].z);

        for (size_t j = 0; j < other_data_num * 3; ++j) {
          vertices_data.emplace_back(other_data[i][j]);
        }
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

auto PrimitiveBuilder::buildPoints(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_POINTS, name, positions, {}, other_data, model);
}

auto PrimitiveBuilder::buildPoint(const std::string& name,
                                  const glm::vec3& position,
                                  const std::vector<float>& other_data,
                                  const glm::mat4& model) -> void {
  auto positions = std::vector<glm::vec3>{position};
  impl_->buildPrimitvie(GL_POINTS, name, positions, {}, {other_data}, model);
}

auto PrimitiveBuilder::buildLines(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<GLsizei>& indices,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_LINES, name, positions, indices, other_data, model);
}

auto PrimitiveBuilder::buildLine(const std::string& name,
                                 const glm::vec3& start, const glm::vec3& end,
                                 const std::vector<float>& other_data,
                                 const glm::mat4& model) -> void {
  auto positions = std::vector<glm::vec3>{start, end};
  impl_->buildPrimitvie(GL_LINES, name, positions, {}, {other_data}, model);
}

auto PrimitiveBuilder::buildLineStrip(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_LINE_STRIP, name, positions, {}, other_data, model);
}

auto PrimitiveBuilder::buildLineStrip(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<GLsizei>& indices,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_LINE_STRIP, name, positions, indices, other_data,
                        model);
}

auto PrimitiveBuilder::buildLineLoop(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_LINE_LOOP, name, positions, {}, other_data, model);
}

auto PrimitiveBuilder::buildLineLoop(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<GLsizei>& indices,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_LINE_LOOP, name, positions, indices, other_data,
                        model);
}

auto PrimitiveBuilder::buildTriangles(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<GLsizei>& indices,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_TRIANGLES, name, positions, indices, other_data,
                        model);
}

auto PrimitiveBuilder::buildTriangle(const std::string& name,
                                     const glm::vec3& p1, const glm::vec3& p2,
                                     const glm::vec3& p3,
                                     const std::vector<float>& other_data)
    -> void {
  auto positions = std::vector<glm::vec3>{p1, p2, p3};
  impl_->buildPrimitvie(GL_TRIANGLES, name, positions, {}, {other_data},
                        glm::mat4(1.0f));
}

auto PrimitiveBuilder::buildTriangleStrip(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<GLsizei>& indices,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, indices, other_data,
                        model);
}

auto PrimitiveBuilder::buildTriangleStrip(
    const std::string& name, const std::vector<glm::vec3>& positions,
    const std::vector<std::vector<float>>& other_data, const glm::mat4& model)
    -> void {
  impl_->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, {}, other_data,
                        model);
}
}  // namespace gl_hwk