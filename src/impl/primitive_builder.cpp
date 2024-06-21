#include "gl_homework/primitive_builder.hpp"

#include <cstdint>
#include <optional>
#include <unordered_map>

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
  explicit PrimitiveBuilderImpl() {}

  auto draw(const Primitive& info) {
    glBindVertexArray(info.vao);
    glBindBuffer(GL_ARRAY_BUFFER, info.vbo);
    uint32_t vertex_total_data_num = 3 + info.other_data_num * 3;
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_total_data_num * sizeof(float), (void*)0);
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
  }

  auto buildPrimitvie(GLenum type, const std::string& name, const std::vector<glm::vec3>& positions,
                      const std::vector<GLsizei>& indices, const std::vector<std::vector<float>>& other_data) -> void {
    assert(positions.size() >= other_data.size());
    assert(!name.empty());

    if (infos_.find(name) != infos_.end()) {
      auto& info = infos_[name];
      assert(info.type == type);
      draw(info);
    } else {
      GLuint vao, vbo;
      glGenVertexArrays(1, &vao);
      glGenBuffers(1, &vbo);
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);

      infos_[name] = {vao, vbo, std::nullopt, type, static_cast<GLsizei>(positions.size())};

      uint32_t other_data_num = other_data.empty() ? 0 : other_data.front().size();
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

      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices_data.size(), vertices_data.data(), GL_STATIC_DRAW);

      if (!indices.empty()) {
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLsizei) * indices.size(), indices.data(), GL_STATIC_DRAW);
        infos_[name].size = indices.size();
        infos_[name].ebo = ebo;
      }

      draw(infos_[name]);
    }
  }

 private:
  std::unordered_map<std::string, Primitive> infos_;
};

PrimitiveBuilder::PrimitiveBuilder() { impl_ = make_unique_impl<PrimitiveBuilderImpl>(); }

auto PrimitiveBuilder::buildPoints(const std::string& name, const std::vector<glm::vec3>& positions,
                                   const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_POINTS, name, positions, {}, other_data);
}

auto PrimitiveBuilder::buildPoint(const std::string& name, const glm::vec3& position,
                                  const std::vector<float>& other_data) -> void {
  auto positions = std::vector<glm::vec3>{position};
  impl_->buildPrimitvie(GL_POINTS, name, positions, {}, {other_data});
}

auto PrimitiveBuilder::buildLines(const std::string& name, const std::vector<glm::vec3>& positions,
                                  const std::vector<GLsizei>& indices,
                                  const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_LINES, name, positions, indices, other_data);
}

auto PrimitiveBuilder::buildLine(const std::string& name, const glm::vec3& start, const glm::vec3& end,
                                 const std::vector<float>& other_data) -> void {
  auto positions = std::vector<glm::vec3>{start, end};
  impl_->buildPrimitvie(GL_LINES, name, positions, {}, {other_data});
}

auto PrimitiveBuilder::buildLineStrip(const std::string& name, const std::vector<glm::vec3>& positions,
                                      const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_LINE_STRIP, name, positions, {}, other_data);
}

auto PrimitiveBuilder::buildLineStrip(const std::string& name, const std::vector<glm::vec3>& positions,
                                      const std::vector<GLsizei>& indices,
                                      const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_LINE_STRIP, name, positions, indices, other_data);
}

auto PrimitiveBuilder::buildLineLoop(const std::string& name, const std::vector<glm::vec3>& positions,
                                     const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_LINE_LOOP, name, positions, {}, other_data);
}

auto PrimitiveBuilder::buildLineLoop(const std::string& name, const std::vector<glm::vec3>& positions,
                                     const std::vector<GLsizei>& indices,
                                     const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_LINE_LOOP, name, positions, indices, other_data);
}

auto PrimitiveBuilder::buildTriangles(const std::string& name, const std::vector<glm::vec3>& positions,
                                      const std::vector<GLsizei>& indices,
                                      const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_TRIANGLES, name, positions, indices, other_data);
}

auto PrimitiveBuilder::buildTriangle(const std::string& name, const glm::vec3& p1, const glm::vec3& p2,
                                     const glm::vec3& p3, const std::vector<float>& other_data) -> void {
  auto positions = std::vector<glm::vec3>{p1, p2, p3};
  impl_->buildPrimitvie(GL_TRIANGLES, name, positions, {}, {other_data});
}

auto PrimitiveBuilder::buildTriangleStrip(const std::string& name, const std::vector<glm::vec3>& positions,
                                          const std::vector<GLsizei>& indices,
                                          const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, indices, other_data);
}

auto PrimitiveBuilder::buildTriangleStrip(const std::string& name, const std::vector<glm::vec3>& positions,
                                          const std::vector<std::vector<float>>& other_data) -> void {
  impl_->buildPrimitvie(GL_TRIANGLE_STRIP, name, positions, {}, other_data);
}
}  // namespace gl_hwk