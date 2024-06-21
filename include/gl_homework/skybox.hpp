// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_SKYBOX_HPP_
#define GL_HOMEWORK_SKYBOX_HPP_

// clang-format off
// std
#include <string>
#include <memory>
#include <vector>
// project
#include "gl_homework/camera.hpp"
#include "gl_homework/impl.hpp"
#include "gl_homework/shader.hpp"
// clang-format on

namespace gl_hwk {

class SkyBoxImpl;
class SkyBox {
 public:
  explicit SkyBox(const std::vector<std::string> &cubemap_path, std::shared_ptr<Shader> shader,
                  std::shared_ptr<Camera> camera);

  auto draw() -> void;

  auto setShader(std::shared_ptr<Shader> shader) -> void;

 private:
  unique_impl<SkyBoxImpl> impl_;
};
}  // namespace gl_hwk
#endif