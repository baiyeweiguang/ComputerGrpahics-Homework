#include "gl_homework/opengl_application.hpp"

#include "gl_homework/impl.hpp"

namespace gl_hwk {

class OpenGLApplicationImpl {
 public:
  OpenGLApplicationImpl() : init_(false), window_(0) {}

  ~OpenGLApplicationImpl() = default;

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

OpenGLApplication::OpenGLApplication() {
  impl_ = make_unique_impl<OpenGLApplicationImpl>();
}

auto OpenGLApplication::instance() -> OpenGLApplication& {
  static OpenGLApplication instance;
  return instance;
}

auto OpenGLApplication::init(int argc, char** argv,
                             const WindowOptions& options) -> void {
  if (!impl_->init_) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(options.width, options.height);
    impl_->window_ = glutCreateWindow(options.name.c_str());
    if (glewInit()) {
      fmt::print("Error: glewInit failed\n");
      std::abort();
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  }
  fmt::print("OpenGL Application Init!\n");
  impl_->init_ = true;
}

auto OpenGLApplication::run() -> void {
  if (impl_->init_) {
    // 注册回调函数
    glutDisplayFunc(OpenGLApplicationImpl::display);
    glutKeyboardFunc(OpenGLApplicationImpl::keyboardCallback);
    glutMouseFunc(OpenGLApplicationImpl::mouseButtonCallback);
    glutMotionFunc(OpenGLApplicationImpl::mouseMoveCallback);
    glutTimerFunc(33, OpenGLApplicationImpl::timerProc, 1);
    // 进入主循环
    glutMainLoop();
  }
}

auto OpenGLApplication::onKeyboardPress(
    std::function<void(unsigned char key, int x, int y)>&& func) -> void {
  impl_->keyboard_callback_ = std::move(func);
}

auto OpenGLApplication::onDisplay(std::function<void()>&& func) -> void {
  impl_->render_callback_ = std::move(func);
}

auto OpenGLApplication::onMouseButtonPress(
    std::function<void(int button, int state, int x, int y)>&& func) -> void {
  impl_->mouse_button_callback_ = std::move(func);
}

auto OpenGLApplication::onMouseMove(std::function<void(int x, int y)>&& func)
    -> void {
  impl_->mouse_move_callback_ = std::move(func);
}

// 定义静态成员变量
std::function<void()> OpenGLApplicationImpl::render_callback_;
std::function<void(unsigned char key, int x, int y)>
    OpenGLApplicationImpl::keyboard_callback_;
std::function<void(int button, int state, int x, int y)>
    OpenGLApplicationImpl::mouse_button_callback_;
std::function<void(int x, int y)> OpenGLApplicationImpl::mouse_move_callback_;

}  // namespace gl_hwk