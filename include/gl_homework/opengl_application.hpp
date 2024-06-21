// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_OPENGL_APPLICATION_HPP_
#define GL_HOMEWORK_OPENGL_APPLICATION_HPP_

// clang-format off
// std
#include <functional>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
// third party
#include <fmt/core.h>
// project
#include "gl_homework/impl.hpp"
// clang-format on

namespace gl_hwk {

struct WindowOptions {
  std::string name = "window1";
  uint32_t width = 512;
  uint32_t height = 512;
  // 背景颜色
  float r = 0.0;
  float b = 0.0;
  float g = 0.0;
};

class OpenGLApplicationImpl;
// Singleton
class OpenGLApplication {
 public:
  static auto instance() -> OpenGLApplication&;

  auto init(int argc, char** argv, const WindowOptions& options = WindowOptions()) -> void;
  auto run() -> void;

  auto setDepthTest(bool enable) -> void;

  auto onKeyboardPress(std::function<void(unsigned char key, int x, int y)>&& func) -> void;
  auto onDisplay(std::function<void()>&& func) -> void;
  auto onMouseButtonPress(std::function<void(int button, int state, int x, int y)>&& func) -> void;
  auto onMouseMove(std::function<void(int x, int y)>&& func) -> void;

 private:
  OpenGLApplication();

  // Disable copy and move
  OpenGLApplication(const OpenGLApplication&) = delete;
  OpenGLApplication& operator=(const OpenGLApplication&) = delete;
  OpenGLApplication(OpenGLApplication&&) = delete;
  OpenGLApplication& operator=(OpenGLApplication&&) = delete;

  // Pimpl idiom
  unique_impl<OpenGLApplicationImpl> impl_;
};

}  // namespace gl_hwk
#endif
