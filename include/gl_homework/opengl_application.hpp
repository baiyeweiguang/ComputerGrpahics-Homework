// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_OPENGL_APPLICATION_HPP_
#define GL_HOMEWORK_OPENGL_APPLICATION_HPP_

// clang-format off
// std
#include <functional>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
// clang-format on
#include <fmt/core.h>

namespace gl_hwk {

// Singleton
class OpenGLApplication {
 public:
  static auto instance() -> OpenGLApplication& {
    static OpenGLApplication instance;
    return instance;
  }

  auto init(int argc, char** argv) -> void {
    if (!init_) {
      glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
      glutInitWindowSize(512, 512);
      window_ = glutCreateWindow(argv[0]);
      if (glewInit()) {
        fmt::print("Error: glewInit failed\n");
        std::abort();
      }
      glEnable(GL_DEPTH_TEST);
    }
    fmt::print("OpenGL Application Init!\n");
    init_ = true;
  }

  auto run() -> void {
    if (init_) {
      glutDisplayFunc(OpenGLApplication::display);
      glutKeyboardFunc(OpenGLApplication::keyboardCallback);

      glutTimerFunc(33, OpenGLApplication::timerProc, 1);

      glutMainLoop();
    }
  }

  auto setKeyboardCallback(
      std::function<void(unsigned char key, int x, int y)>&& func) -> void {
    keyboard_func_ = func;
  }

  auto setRenderFunc(std::function<void()>&& func) -> void {
    render_func_ = func;
  }

 private:
  OpenGLApplication() : init_(false), window_(0) {}
  ~OpenGLApplication() = default;

  // Disable copy and move
  OpenGLApplication(const OpenGLApplication&) = delete;
  OpenGLApplication& operator=(const OpenGLApplication&) = delete;
  OpenGLApplication(OpenGLApplication&&) = delete;
  OpenGLApplication& operator=(OpenGLApplication&&) = delete;

  static auto timerProc(int value) -> void {
    glutPostRedisplay();
    glutTimerFunc(33, timerProc, 1);
  }

  static auto display() -> void {
    if (render_func_) {
      render_func_();
    } else {
      fmt::print("render func is not available\n");
    }
  }

  static auto keyboardCallback(unsigned char key, int x, int y) -> void {
    if (keyboard_func_) {
      keyboard_func_(key, x, y);
    } else {
      fmt::print("keyboard func is not available\n");
    }
  }

  bool init_;
  GLuint window_;
  static std::function<void()> render_func_;
  static std::function<void(unsigned char key, int x, int y)> keyboard_func_;
};

// Define static members
inline std::function<void()> OpenGLApplication::render_func_;
inline std::function<void(unsigned char key, int x, int y)>
    OpenGLApplication::keyboard_func_;

}  // namespace gl_hwk
#endif
