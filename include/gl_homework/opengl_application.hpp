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

struct WindowOptions {
  std::string name = "window1";
  uint32_t width = 512;
  uint32_t height = 512;
};

// Singleton
class OpenGLApplication {
 public:
  static auto instance() -> OpenGLApplication& {
    static OpenGLApplication instance;
    return instance;
  }

  auto init(int argc, char** argv,
            const WindowOptions& options = WindowOptions()) -> void {
    if (!init_) {
      glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
      glutInitWindowSize(options.width, options.height);
      window_ = glutCreateWindow(options.name.c_str());
      if (glewInit()) {
        fmt::print("Error: glewInit failed\n");
        std::abort();
      }
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
    }
    fmt::print("OpenGL Application Init!\n");
    init_ = true;
  }

  auto run() -> void {
    if (init_) {
      // 注册回调函数
      glutDisplayFunc(OpenGLApplication::display);
      glutKeyboardFunc(OpenGLApplication::keyboardCallback);
      glutMouseFunc(OpenGLApplication::mouseButtonCallback);
      glutMotionFunc(OpenGLApplication::mouseMoveCallback);
      glutTimerFunc(33, OpenGLApplication::timerProc, 1);
      // 进入主循环
      glutMainLoop();
    }
  }

  auto onKeyboardPress(
      std::function<void(unsigned char key, int x, int y)>&& func) -> void {
    keyboard_callback_ = func;
  }

  auto onDisplay(std::function<void()>&& func) -> void {
    render_callback_ = func;
  }

  auto onMouseButtonPress(
      std::function<void(int button, int state, int x, int y)>&& func) -> void {
    mouse_button_callback_ = func;
  }

  auto onMouseMove(std::function<void(int x, int y)>&& func) -> void {
    mouse_move_callback_ = func;
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
    if (render_callback_) {
      render_callback_();
    } else {
      fmt::print("render func is not available\n");
    }
  }

  static auto keyboardCallback(unsigned char key, int x, int y) -> void {
    if (keyboard_callback_) {
      keyboard_callback_(key, x, y);
    }
  }

  static auto mouseButtonCallback(int button, int state, int x, int y) -> void {
    if (mouse_button_callback_) {
      mouse_button_callback_(button, state, x, y);
    }
  }

  static auto mouseMoveCallback(int x, int y) -> void {
    if (mouse_move_callback_) {
      mouse_move_callback_(x, y);
    }
  }

  bool init_;
  GLuint window_;
  static std::function<void()> render_callback_;
  static std::function<void(unsigned char key, int x, int y)>
      keyboard_callback_;
  static std::function<void(int button, int state, int x, int y)>
      mouse_button_callback_;
  static std::function<void(int x, int y)> mouse_move_callback_;
};

// Define static members
inline std::function<void()> OpenGLApplication::render_callback_;
inline std::function<void(unsigned char key, int x, int y)>
    OpenGLApplication::keyboard_callback_;
inline std::function<void(int button, int state, int x, int y)>
    OpenGLApplication::mouse_button_callback_;
inline std::function<void(int x, int y)>
    OpenGLApplication::mouse_move_callback_;

}  // namespace gl_hwk
#endif
